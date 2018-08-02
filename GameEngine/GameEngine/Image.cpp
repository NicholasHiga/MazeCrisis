#include "Image.h"

using std::string;

Image::Image(string _filename)
{
    // image format
    imgFormat = FIF_UNKNOWN;
    // pointer to the image, once loaded
    dib = 0;
    bits = NULL;
    width = 0;
    height = 0;
    filename = _filename;

    TCHAR pwd[MAX_PATH];
    GetCurrentDirectory(MAX_PATH, pwd);
    //MessageBox(NULL, pwd, pwd, 0);

    try
    {
        // check the file signature and deduce its format
        imgFormat = FreeImage_GetFileType(_filename.c_str(), 0);
        // if still unknown, try to guess the file format from the file 
        // extension
        if (imgFormat == FIF_UNKNOWN)
            imgFormat = FreeImage_GetFIFFromFilename(_filename.c_str());
        // if still unkown, return failure
        if (imgFormat == FIF_UNKNOWN)
            throw 0;

        // check that the plugin has reading capabilities and load the file
        if (FreeImage_FIFSupportsReading(imgFormat))
            dib = FreeImage_Load(imgFormat, _filename.c_str());
        // if the image failed to load, return failure
        if (!dib)
            throw 1;

        dib = FreeImage_ConvertTo32Bits(dib);

        // retrieve the image data
        bits = FreeImage_GetBits(dib);
        BYTE a, b, c, d;
        a = *bits;
        b = *(bits + 1);
        c = *(bits + 2);
        d = *(bits + 3);

        // get the image width and height
        width = FreeImage_GetWidth(dib);
        height = FreeImage_GetHeight(dib);
        // get image type
        bitsPerPixel = FreeImage_GetBPP(dib);
        
        // From FreeImage documentation:
        // Calculates(or at least estimates) the total memory usage of a 
        // FreeImage bitmap.This includes the ICC profile size, the size of the
        // embedded thumbnail(if any), the memory required for all the metadata, 
        // as well as any FreeImage internal (housekeeping)memory.
        numBytes = FreeImage_GetMemorySize(dib);

        FreeImage_FlipVertical(dib);

        // if this somehow one of these failed (they shouldn't), return failure
        if ((bits == 0) || (width == 0) || (height == 0))
            throw 2;
    }
    catch (int num)
    {
        if (num == 0)
            throw "File Format Unknown";
        else if (num == 1)
            throw "Image Failed to Load";
        else
            throw "Image Dimensions Impossible";
    }
}

Image::~Image()
{
    // Free FreeImage's copy of the data
    FreeImage_Unload(dib);
}

string 
Image::getFileName() const
{
    return filename;
}

void
Image::getDimensions(unsigned int &_width, unsigned int &_height)
{
    _width = width;
    _height = height;
}

unsigned int
Image::getWidth() const
{
    return width;
}

unsigned int
Image::getHeight() const
{
    return height;
}

FREE_IMAGE_FORMAT
Image::getImgFormat() const
{
    return imgFormat;
}

BYTE
Image::getBitsPerPixel() const
{
    return bitsPerPixel;
}

int
Image::getNumBytes() const
{
    return numBytes;
}

BYTE*
Image::getImageData() const
{
    return bits;
}

/*bool 
Image::getIsPacked()
{
    return isPacked;
}*/