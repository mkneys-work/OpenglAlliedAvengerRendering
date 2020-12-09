#include "pch.h"
#include "texture.h"
#include "mymath.h"

Texture::Texture( const char * file_name )
{
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	FIBITMAP * fitBitMap =  nullptr;
	fif = FreeImage_GetFileType( file_name, 0 );

	if ( fif == FIF_UNKNOWN )
	{
		fif = FreeImage_GetFIFFromFilename( file_name );
	}

	if ( fif != FIF_UNKNOWN )
	{
		if ( FreeImage_FIFSupportsReading( fif ) )
		{
			fitBitMap = FreeImage_Load( fif, file_name );
		}

		if ( fitBitMap )
		{			
			_width = int( FreeImage_GetWidth( fitBitMap ) );
			_height = int( FreeImage_GetHeight( fitBitMap ) );

			if ( ( _width != 0 ) && ( _height != 0 ) )
			{				
				_scanWidth = FreeImage_GetPitch( fitBitMap );
				_pixelSize = FreeImage_GetBPP( fitBitMap ) / 8;			
				_data = new BYTE[_scanWidth * _height];								
				FreeImage_ConvertToRawBits( _data, fitBitMap, _scanWidth, _pixelSize * 8,
					FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE );
			}
			FreeImage_Unload( fitBitMap );			
		}
	}

	if ( _data )
	{
		printf( "Texture '%s' (%d x %d px, %d bpp, %0.1f MB) loaded.\n",
			file_name, _width, _height, _pixelSize * 8, _scanWidth * _height / ( 1024.0f * 1024.0f ) );
	}
	else
	{
		printf( "Texture '%s' not loaded.\n", file_name );		
	}
}

Texture::~Texture()
{	
	if ( _data )
	{
		delete[] _data;
		_data = nullptr;
		
		_width = 0;
		_height = 0;
		_scanWidth = 0;
		_pixelSize = 0;
	}
}

Color3f Texture::getTexel(const float u, const float v, const bool linearize, InterpolationType interpolationType) const
{
	assert( ( u >= 0.0f && u <= 1.0f ) && ( v >= 0.0f && v <= 1.0f ) );

	Color3f texel = { 0 };
	switch (interpolationType) {
		case InterpolationType::BILLINEAR:
			texel = bilinearInterpolation(u, v);
			break;
		case InterpolationType::NEAREST_NEIGHBOUR:
			texel = nearestNeigbourInterpolation(u, v);
	}

	if (linearize)
		return texel.linear();
	else
		return texel;
}

Color3f Texture::bilinearInterpolation(const float u, const float v) const 
{
	const float x = u * _width;
	const float y = v * _height;

	const int x0 = max(0, min(_width - 1, int(x)));
	const int y0 = max(0, min(_height - 1, int(y)));

	const int x1 = min(_width - 1, x0 + 1);
	const int y1 = min(_height - 1, y0 + 1);

	BYTE* p1 = &_data[y0 * _scanWidth + x0 * _pixelSize];
	BYTE* p2 = &_data[y0 * _scanWidth + x1 * _pixelSize];
	BYTE* p3 = &_data[y1 * _scanWidth + x0 * _pixelSize];
	BYTE* p4 = &_data[y1 * _scanWidth + x1 * _pixelSize];

	const float kx = x - x0;
	const float ky = y - y0;

	if (_pixelSize < 12)
	{
		return (Color3f::makeFromBgr<BYTE>(p1) * (1 - kx) * (1 - ky) +
			Color3f::makeFromBgr<BYTE>(p2) * kx * (1 - ky) +
			Color3f::makeFromBgr<BYTE>(p3) * (1 - kx) * ky +
			Color3f::makeFromBgr<BYTE>(p4) * kx * ky) *
			(1.0f / 255.0f);

	}
	else
	{
		return (Color3f::makeFromBgr<float>(p1) * (1 - kx) * (1 - ky) +
			Color3f::makeFromBgr<float>(p2) * kx * (1 - ky) +
			Color3f::makeFromBgr<float>(p3) * (1 - kx) * ky +
			Color3f::makeFromBgr<float>(p4) * kx * ky);
	}
}

Color3f Texture::nearestNeigbourInterpolation(const float u, const float v) const 
{
	const int x = max( 0, min( _width - 1, int( u * _width ) ) );
	const int y = max( 0, min( _height - 1, int( v * _height ) ) );

	const int offset = y * this->_scanWidth + x * _pixelSize;
	const float s = 1.0f / 255.0f;
	const float b = _data[offset] * s;
	const float g = _data[offset + 1] * s;
	const float r = _data[offset + 2] * s;

	return Color3f{ r, g, b };
}



BYTE * Texture::getData() const {
	return _data;
}

int Texture::getWidth() const
{
	return _width;
}

int Texture::getHeight() const
{
	return _height;
}

void Texture::CopyTo( BYTE * data, const int pixel_size )
{
	if ( pixel_size == _pixelSize )
	{
		memcpy( data, data, _scanWidth * _height );
	}
	else
	{
		for ( int y = 0; y < _height; ++y )
		{
			for ( int x = 0; x < _width; ++x )
			{
				for ( int c = 0; c < min( pixel_size, _pixelSize ); c++ )
				{
					data[y * pixel_size * _width + x * pixel_size + c] = data[y * _scanWidth + x * _pixelSize + c];
				}
			}
		}
	}
}


