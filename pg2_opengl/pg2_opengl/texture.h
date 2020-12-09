#ifndef TEXTURE_H_
#define TEXTURE_H_

#include "freeimage.h"
#include "structs.h"

class Texture
{
public:
	Texture( const char* fileName );
	~Texture();

	enum InterpolationType { BILLINEAR, NEAREST_NEIGHBOUR };
	
	Color3f getTexel( const float u, const float v, const bool linearize, InterpolationType interpolationType = InterpolationType::BILLINEAR) const;

	int getWidth() const;
	int getHeight() const;

	BYTE* getData() const;
	void CopyTo( BYTE * data, const int pixelSize = 3);

private:	
	int _width{ 0 };
	int _height{ 0 };
	int _scanWidth{ 0 };
	int _pixelSize{ 0 }; 
	BYTE * _data{ nullptr };

	Color3f bilinearInterpolation(const float u, const float v) const;
	Color3f nearestNeigbourInterpolation(const float u, const float v) const;

	Texture( const Texture& ) = delete;
	Texture& operator=( const Texture& ) = delete;
};

#endif
