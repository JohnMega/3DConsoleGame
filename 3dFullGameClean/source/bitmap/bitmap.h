#pragma once

#include <string>
#include <vector>

// ----------------------------------------------------------------------------
/**
 * Represents a single Pixel in the image. A Pixel has red, green, and blue
 * components that are mixed to form a color. Each of these values can range
 * from 0 to 255
**/
class BmpPixel
{
public:
    // Stores the individual color components.
    int red, green, blue;

    // Initializes a Pixel with a default black color.
    BmpPixel() : red(0), green(0), blue(0) { }

    // Initializes a color Pixel with the specified RGB values.
    BmpPixel(int r, int g, int b) : red(r), green(g), blue(b) { }
};

// ----------------------------------------------------------------------------
//To abbreviate a pixel matrix built as a vector of vectors
typedef std::vector < std::vector <BmpPixel> > PixelMatrix;

// ----------------------------------------------------------------------------
/**
 * Represents a bitmap where a grid of pixels (in row-major order)
 * describes the color of each pixel within the image. Limited to Windows BMP
 * formatted images with no compression and 24 bit color depth.
**/
class Bitmap
{
private:
    PixelMatrix pixels;

public:
    float height, wight;

    /**
     * Opens a file as its name is provided and reads pixel-by-pixel the colors
     * into a matrix of RGB pixels. Any errors will cout but will result in an
     * empty matrix (with no rows and no columns).
     *
     * @param name of the filename to be opened and read as a matrix of pixels
    **/
    void open(std::string);

    /**
     * Saves the current image, represented by the matrix of pixels, as a
     * Windows BMP file with the name provided by the parameter. File extension
     * is not forced but should be .bmp. Any errors will cout and will NOT
     * attempt to save the file.
     *
     * @param name of the filename to be written as a bmp image
    **/
    void save(std::string);

    /**
     * Validates whether or not the current matrix of pixels represents a
     * proper image with non-zero-size rows and consistent non-zero-size
     * columns for each row. In addition, each pixel in the matrix is validated
     * to have red, green, and blue components with values between 0 and 255
     *
     * @return boolean value of whether or not the matrix is a valid image
    **/
    bool isImage();

    /**
     * Provides a vector of vector of pixels representing the bitmap
     *
     * @return the bitmap image, represented by a matrix of RGB pixels
    **/
    PixelMatrix toPixelMatrix();

    /**
     * Overwrites the current bitmap with that represented by a matrix of
     * pixels. Does not validate that the new matrix of pixels is a proper
     * image.
     *
     * @param a matrix of pixels to represent a bitmap
    **/
    void fromPixelMatrix(const PixelMatrix&);

};