#include <convolve.h>
#include <greyscale.h>
#include <savepgm.h>

#include <iostream>

using Scalar = double;
using Image = Greyscale< Scalar >;

Image circleImage( int width )
{
	Image img( width, width );

	const Scalar cx = Scalar( width ) / 2.;
	const Scalar cy = Scalar( width ) / 2.;
	const Scalar rad = Scalar( width ) / 2.;

	for ( int x = 0; x < width; x++ ) {
		for (int y = 0; y < width; y++) {
			const auto distToCenter = std::sqrt( std::pow( Scalar( x ) - cx, 2. ) + std::pow( Scalar( y ) - cy, 2. ) );
			const Scalar setVal = distToCenter <= rad ? 0. : 1.;
			img.set( x, y, setVal );
		}
	}
	return img;
}

Image convolutionKernel()
{
	const int width = 5;
	Image kernel( width, width );
	const Scalar setVal = 1. / static_cast< Scalar >( width * width );
	for ( int x = 0; x < width; x++ ) {
		for ( int y = 0; y < width; y++ ) {
			kernel.set( x, y, setVal );
		}
	}
	return kernel;
}

int main()
{
	const int pixelsWide = 50;
		
	const auto input = circleImage( pixelsWide );

	const char* const inputPath = "input.pgm";
	std::cout << "Saving input image as '" << inputPath << "'." << std::endl;
	saveAsPgm( input, inputPath );

	// Perform convolution using OpenMP.
	const auto kernel = convolutionKernel();
	const auto convolved = convolve< Scalar >( input, kernel );
	const char* const outputPath = "convolved.pgm";
	std::cout << "Saving convolved image as '" << outputPath << "'." << std::endl;
	saveAsPgm( convolved, outputPath );
}