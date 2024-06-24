#ifndef _GFT_CIMAGE_H_
#define _GFT_CIMAGE_H_

#include "gft_image32.h"
#include "gft_color.h"

namespace gft{

  /**
   * \brief Common definitions and functions to manipulate a color image, providing three samples (color channels) for each pixel.
   */
  namespace CImage{

    /**
     * \brief Structure representing a color image. 
     *
     * It supports both linear and two-dimensional access 
     * (i.e., cimg->C[c]->data[p] or cimg->C[c]->array[y][x] for a pixel
     * (x,y) at address p=x+y*ncols of channel c).
     */    
    struct sCImage {
      sImage32 *C[3];
    };

    /**
     * \brief A constructor.
     */
    sCImage *Create(int ncols, int nrows);
    /**
     * \brief A constructor using cimg as a template.
     */
    sCImage *Create(sCImage *cimg);
    /**
     * \brief A constructor using img as a template.
     */
    sCImage *Create(sImage32 *img);

    /**
     * \brief A destructor.
     */
    void    Destroy(sCImage **cimg);

    /**
     * \brief A copy constructor.
     */
    sCImage *Clone(sCImage *cimg);
    /**
     * \brief A copy constructor.
     */
    sCImage *Clone(sCImage *cimg, Pixel l, Pixel h);
    /**
     * \brief A copy constructor.
     */
    sCImage *Clone(sImage32 *img);
    /**
     * \brief A copy constructor.
     */
    sCImage *Clone(sImage32 *img, int Imin, int Imax);

    /**
     * \brief Copies the contents of one image to another destination image.
     */
    void    Copy(sCImage *cimg, sCImage *sub, Pixel l);
    /**
     * \brief Copies the contents of one image to another destination image.
     */
    void    Copy(sCImage *cimg, sCImage *sub, Pixel l, int bkgcolor);   

    /**
     * \brief selects a random color for each brightness value in the input grayscale image for outputting a color image.
     */
    sCImage *RandomColorize(sImage32 *img);
    /**
     * \brief selects a random color for each brightness value in the input grayscale image for outputting a color image.
     */
    sCImage *ColorizeLabel(sImage32 *label);
    
    /**
     * \brief Reads a color image of a file on the disc in PPM format.
     */
    sCImage *Read(char *filename);
    /**
     * \brief Writes a color image to a file on the disc in PPM format. 
     */
    void    Write(sCImage *cimg, char *filename);

    /**
     * \brief Fills a color image with a given color.
     */    
    void    Set(sCImage *cimg, int r, int g, int b);
    /**
     * \brief Fills a color image with a given color.
     */
    void    Set(sCImage *cimg, int color);

    /**
     * \brief Tests whether a pixel is valid for a given image.
     */    
    bool    IsValidPixel(sCImage *cimg, int x, int y);

    /**
     * \brief Changes an image from RGB to CIELAB color space.
     */
    sCImage *RGB2Lab(sCImage *cimg);

    /**
     * \brief Changes an image from RGB to grayscale.
     */
    sImage32 *Lightness(sCImage *cimg);

    /**
     * \brief Changes an image from RGB to grayscale.
     *
     * The luminosity method works best overall and is the 
     * default method used if you ask GIMP to change an image 
     * from RGB to grayscale 
     */
    sImage32 *Luminosity(sCImage *cimg);

    /**
     * \brief Calculates the minimum bounding box without the background color.
     */
    void MBB(sCImage *cimg, int bkgcolor, Pixel *l, Pixel *h);

    /**
     * \brief Adds a frame around a color image, with the provided width and fill color.
     */
    sCImage *AddFrame(sCImage *cimg, int sz, int r, int g, int b);
    /**
     * \brief Removes the frame around a color image with the given width.
     */
    sCImage *RemFrame(sCImage *cimg, int sz);

    /**
     * \brief Resizes the color image by the horizontal and vertical factors and interpolation method provided.
     *
     * @param Sx The horizontal resize factor.
     * @param Sy The vertical resize factor.
     */
    sCImage *Scale(sCImage *cimg, float Sx, float Sy,
		   gft::InterpolationType interpolation);
    
  } //end CImage namespace

  typedef CImage::sCImage sCImage;

} //end gft namespace

#endif

