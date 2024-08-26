#ifndef _GFT_IMAGE32_H_
#define _GFT_IMAGE32_H_

#include "gft_common.h"

namespace gft{

  typedef enum {none, linear, cubic} InterpolationType;

  /**
   * \brief Common definitions and functions to manipulate a grayscale image of 32 bits per pixel.
   */
  namespace Image32{

    /**
     * \brief Structure representing a digital image. 
     *
     * It supports both linear and two-dimensional access 
     * (i.e., img->data[p] or img->array[y][x] for a pixel
     * (x,y) at address p=x+y*ncols).
     */
    struct sImage32 {
      int *data;
      int **array;
      int nrows; /* numero de linhas (altura) */
      int ncols; /* numero de colunas (largura) */
      int n;     /* numero de pixels */
      int maxval, minval;
      float dx;
      float dy;
    };

    /**
     * \brief A constructor.
     */
    sImage32 *Create(int ncols,int nrows);
    /**
     * \brief A constructor using img as a template.
     */
    sImage32 *Create(sImage32 *img);
    
    /**
     * \brief A destructor.
     */
    void    Destroy(sImage32 **img);

    /**
     * \brief A copy constructor.
     */
    sImage32 *Clone(sImage32 *img);
    /**
     * \brief A copy constructor.
     */
    sImage32 *Clone(sImage32 *img, Pixel l, Pixel h);

    /**
     * \brief Copies the contents of one image to another destination image.
     */    
    void     Copy(sImage32 *img,  sImage32 *sub, Pixel l);
    /**
     * \brief Copies the contents of one image to another destination image.
     */
    void     Copy(sImage32 *img,  sImage32 *sub, Pixel l, int bkg);    
    /**
     * \brief Copies the contents of one image to another destination image.
     */
    void     Copy(sImage32 *dest, sImage32 *src);

    /**
     * \brief Calculates the sum of two images pixel by pixel. 
     */
    sImage32 *Add( sImage32 *img1, sImage32 *img2);
    /**
     * \brief Adds a constant to all pixels in the image.
     */
    sImage32 *Add( sImage32 *img,  int value);

    /**
     * \brief Calculates the product of two images pixel by pixel.
     *
     * Gives an image in which each pixel is the product of the 
     * corresponding pixels in img1 and img2.
     */
    sImage32 *Mult(sImage32 *img1, sImage32 *img2);

    /**
     * \brief Reads an image from a file on the disc in PGM format.
     */
    sImage32 *Read(char *filename);
    /**
     * \brief Writes an image to a file on the disc in PGM format. 
     */
    void      Write(sImage32 *img, char *filename);

    /**
     * \brief Changes the amount of gray levels in the image to a given bit depth.
     */
    sImage32 *ConvertToNbits(sImage32 *img, int N);
    /**
     * \brief Changes the amount of gray levels in the image to a given bit depth.
     */
    sImage32 *ConvertToNbits(sImage32 *img, int N, bool Imin);

    /**
     * \brief Calculates the image complement by inverting its values (white becomes black, black becomes white).
     *
     * In the complement of a grayscale image, each pixel value is 
     * subtracted from the maximum pixel value. The difference is 
     * used as the pixel value in the output image. In the output image, 
     * dark areas become lighter and light areas become darker.
     */
    sImage32 *Complement(sImage32 *img);

    /**
     * \brief Returns the minimum pixel value present in a given image.
     */
    int     GetMinVal(sImage32 *img);
    /**
     * \brief Returns the minimum pixel value present in a given image.
     *
     * @param img The input image.
     * @param p Pointer to a variable used to store a pixel address having the minimum value.
     */
    int     GetMinVal(sImage32 *img, int *p);
    /**
     * \brief Returns the maximum pixel value present in a given image.
     */
    int     GetMaxVal(sImage32 *img);
    /**
     * \brief Returns the maximum pixel value present in a given image.
     *
     * @param img The input image.
     * @param p Pointer to a variable used to store a pixel address having the maximum value.
     */
    int     GetMaxVal(sImage32 *img, int *p);
    /**
     * \brief Returns the maximum pixel value present in a given image.
     */
    int     GetMaxVal(sImage32 *img, int ignoredvalue);

    /**
     * \brief Returns the number of occurrences (frequency) of a given value in an image. 
     */
    int     GetFreqVal(sImage32 *img, int val);

    /**
     * \brief Fills an image with a given value.
     */
    void    Set(sImage32 *img, int value);
    /**
     * \brief Replaces a given image value with a new value.
     */
    void    Set(sImage32 *img, int old_value, int new_value);    

    /**
     * \brief Tests whether a pixel is valid for a given image.
     */
    bool    IsValidPixel(sImage32 *img, int x, int y);

    /**
     * \brief Calculates the segmentation/binarization by thresholding the image.
     */
    sImage32 *Threshold(sImage32 *img, int L, int H);
    
    //------------------------------------
    /**
     * \brief Adds a frame around an image, with the provided width and fill value.
     */
    sImage32 *AddFrame(sImage32 *img, int sz, int value);
    /**
     * \brief Removes the frame around an image with the given width.
     */
    sImage32 *RemFrame(sImage32 *fimg, int sz);

    //------------------------------------
    /**
     * \brief Resizes the image by the horizontal and vertical factors and interpolation method provided.
     *
     * @param Sx The horizontal resize factor.
     * @param Sy The vertical resize factor.
     */
    sImage32 *Scale(sImage32 *img, float Sx, float Sy,
		    InterpolationType interpolation);

    /**
     * \brief Returns a subimage by clipping the minimum bounding box with non-zero values.
     */
    sImage32 *MBB(sImage32 *img);
    /**
     * \brief Calculates the minimum bounding box with non-zero values.
     */
    void      MBB(sImage32 *img, Pixel *l, Pixel *h);
 
   
  } //end Image32 namespace

  typedef Image32::sImage32 sImage32;

} //end gft namespace



#endif

