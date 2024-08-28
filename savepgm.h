#ifndef SAVEPGM_H
#define SAVEPGM_H

#include <greyscale.h>

#include <fstream>

///  Write 'image' as a PGM file using run location-relative 'filepath'.
template< typename Scalar >
void saveAsPgm( const Greyscale< Scalar >& image, const char* const filepath )
{
	// Using "plain" PGM description at https://netpbm.sourceforge.net/doc/pgm.html.

	std::ofstream file;
	file.open( filepath, std::ios_base::out );

	// Header information
	const size_t maxGreyLevel = 255;
	file << "P2 " << image.width() << " " << image.height() << " " << maxGreyLevel << "\n";
	for ( int y = 0; y < image.height(); y++ ) {
		for (int x = 0; x < image.width(); x++) {
			const auto scalar = image.get( x, y );
			// [0,255]
			const auto integral = static_cast< int >( scalar * static_cast< Scalar >( maxGreyLevel ) );
			file << integral << " ";
		}
		file << "\n";
	}

	file.close();
}

#endif // #include