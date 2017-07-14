#pragma once
#include <string>
#include <windows.h>
#include "FreeImage.h"

class Image
{
public:
	Image(std::string _filename);
	~Image();

	std::string getFileName();
	void getDimensions(unsigned int &_width, unsigned int &_height);
	unsigned int getHeight();
	unsigned int getWidth();
	FREE_IMAGE_FORMAT getImgFormat();
	BYTE getBitsPerPixel();
	int getNumBytes();
	BYTE* getImageData();
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