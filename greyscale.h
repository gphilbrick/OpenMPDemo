#ifndef GREYSCALE_H
#define GREYSCALE_H

#include <vector>

///  A 2D image of floating-point values in [0,1].
template< typename Scalar,
		  std::enable_if_t< std::is_floating_point< Scalar >::value, int > = 0 >
class Greyscale
{
public:
	///  'width' > 0, 'height' > 0
	Greyscale( int width, int height )
		: _width( width )
		, _height( height )
	{
		_values.resize (width * height, Scalar( 0. ) );
	}

	int width() const
	{
		return _width;
	}

	int height() const
	{
		return _height;
	}

	bool inRange( int x, int y ) const
	{
		return x >= 0 && x < _width && y >= 0 && y < _height;
	}

	/// 'x' in [0, width), 'y' in [0,height).
	Scalar get( int x, int y ) const
	{
		return _values[ valueIndex( x, y ) ];
	}

	/// 'value' in [0,1].
	void set( int x, int y, Scalar value )
	{
		_values[ valueIndex( x, y ) ] = value;
	}

private:
	int valueIndex( int x, int y ) const
	{
		return x + _width * y;
	}

	const int _width;
	const int _height;
	std::vector< Scalar > _values;
};

#endif // #include