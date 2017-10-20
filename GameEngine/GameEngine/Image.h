#pragma once
#include <string>
#include <windows.h>
#include "FreeImage.h"

class Image
{
public:
	Image(std::string _filename);
	~Image();

	std::string getFileName() const;
	void getDimensions(unsigned int &_width, unsigned int &_height);
	unsigned int getHeight() const;
	unsigned int getWidth() const;
	FREE_IMAGE_FORMAT getImgFormat() const;
	BYTE getBitsPerPixel() const;
	int getNumBytes() const;
	BYTE* getImageData() const;
	//bool getIsPacked();

protected:
	std::string filename;
	unsigned int width, height;
	FREE_IMAGE_FORMAT imgFormat;
	BYTE bitsPerPixel;
	unsigned int numBytes; // Not necessarily correct
	//bool isPacked; Since using freeimage, always unpacked

	FIBITMAP *dib;
	BYTE* bits; // Pointer to image data.
};