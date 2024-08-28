#ifndef CONVOLVE_H
#define CONVOLVE_H

#include <greyscale.h>

#include <omp.h>

#include <iostream>

/// Return the neighborhood of 'toConvolve'--centered at 'x', 'y'--convolved by 'kernel'.
/// 'kernel' has dimensions > 1.
template< typename Scalar >
Scalar convolvedValue( 
	const Greyscale< Scalar >& toConvolve, 
	int x,
	int y,
	const Greyscale< Scalar >& kernel )
{
	const auto kernelWidth = kernel.width();

	// Compute a weighted sum of the neighborhood in 'toConvolve' around ( 'x_conv', 'y_conv' ).
	Scalar sum = 0.;
	Scalar weightSum = 0.;
	for ( int x_k = 0; x_k < kernel.width(); x_k++ ) {
		for ( int y_k = 0; y_k < kernel.height(); y_k++ ) {
			const int x_toConv = x + x_k - kernelWidth / 2;
			const int y_toConv = y + y_k - kernelWidth / 2;
			if ( toConvolve.inRange( x_toConv, y_toConv ) ) {
				const auto val_kernel = kernel.get( x_k, y_k );
				const auto val_toConvolve = toConvolve.get( x_toConv, y_toConv );
				sum += val_toConvolve * val_kernel;
				weightSum += val_kernel;
			}
		}
	}
	// 'weightSum' guaranteed > 0.
	return sum / weightSum;
}

///	Return 'toConvolve' convolved by 'kernel'. All the values in 'kernel' must sum to 1.
template< typename Scalar >
Greyscale< Scalar > convolve( const Greyscale< Scalar >& toConvolve, const Greyscale< Scalar >& kernel )
{
	const auto width = toConvolve.width();
	const auto height = toConvolve.height();
	Greyscale< Scalar > convolved( width, height );

	// Make sure that OpenMP isn't using more threads than there are rows in 'toConvolve'.
	//omp_set_num_threads( std::min( omp_get_num_threads(), toConvolve.height() ) );
	const auto numThreads = std::min( height, omp_get_max_threads() );
	std::cout << "OpenMP number of threads: " << numThreads << std::endl;
	const auto rowsPerThread = height / numThreads;	

	// Create way for each OMP thread to announce information via 'std::cout' w/out causing
	// race condition.
	omp_lock_t consoleOutputLock;
	omp_init_lock( &consoleOutputLock );

	// This splitting-up-the-rows approach would be more succinctly done with "#pragma omp parallel for",
	// but I want to show the use of an OMP lock letting each thread announce itself via 'std::cout'.
	#pragma omp parallel num_threads( numThreads )
	{
		const auto threadId = omp_get_thread_num();
		const auto rowStart = threadId * rowsPerThread;
		const auto rowEnd = threadId == omp_get_num_threads() - 1
			? height - 1
			: rowStart + rowsPerThread - 1;

		// Lock access to 'std::cout'.
		omp_set_lock( &consoleOutputLock );
		{ 
			std::cout << "OpenMP thread #" << threadId << " handling rows " << rowStart << " to " << rowEnd << "." << std::endl;
		}
		omp_unset_lock( &consoleOutputLock );

		for ( int y_conv = rowStart; y_conv <= rowEnd; y_conv++ ) {
			for ( int x_conv = 0; x_conv < width; x_conv++ ) {
				// In valid kernel operation, 'weightSum' guaranteed > 0.
				convolved.set( x_conv, y_conv, convolvedValue( toConvolve, x_conv, y_conv, kernel ) );
			}
		}
	}

	return convolved;
}

#endif // #include