
#ifndef _GFT_COLOR_H_
#define _GFT_COLOR_H_

#include "gft_common.h"

namespace gft{

  /**
   * \brief Common definitions and functions to manipulate color spaces.
   */  
  namespace Color{

    /**
     * \brief Converts a color to hexadecimal format (a hex triplet).
     *
     * The hex triplet is formed by concatenating three bytes of color channels (24-bit color scheme). 
     * A hex triplet corresponds to a six-digit hexadecimal number.
     */
    int Triplet(int a,int b,int c);

    /**
     * \brief Structure representing a color in RGB space.
     */
    struct sColorRGB{
      int r;
      int g;
      int b;
    };

    /**
     * \brief Structure representing a color in HSV space.
     */
    struct sColorHSV{
      int h;
      int s;
      int v;
    };

    /**
     * \brief Converts the color from the RGB color space to YCbCr color space.
     */
    int RGB2YCbCr(int v);
    /**
     * \brief Converts the color from the YCbCr color space to RGB color space.
     */
    int YCbCr2RGB(int v);

    /**
     * \brief Converts the color from the RGB color space to HSV color space.
     */    
    sColorHSV RGB2HSV(sColorRGB rgb);
    /**
     * \brief Converts the color from the HSV color space to RGB color space.
     */
    sColorRGB HSV2RGB(sColorHSV hsv);

    /**
     * \brief Converts the color from the RGB color space to CIELAB color space.
     */
    void RGB2Lab(const int& R, const int& G, const int& B,
		 double &l, double &a, double &b);
    /**
     * \brief Converts the color from the RGB color space to CIELAB color space.
     */
    void RGB2Lab(const int& R, const int& G, const int& B,
		 float &l, float &a, float &b);

    /**
     * \brief Generates a random color.
     */
    int RandomColor();

    /**
     * \brief Gets the first channel of a given color.
     */
    inline int Channel0(int c);
    /**
     * \brief Gets the second channel of a given color.
     */
    inline int Channel1(int c);
    /**
     * \brief Gets the third channel of a given color.
     */
    inline int Channel2(int c);

    /** 
     * \brief Merges two colors in RGB-space using (1-ratio) of a and ratio of b.
     */
    int MergeRGB(int a,int b,float ratio);
    
    //---------inline definitions------------------

    inline int Channel0(int c){
      return ((c>>16)&0xff);
    }

    inline int Channel1(int c){
      return ((c>>8)&0xff);
    }

    inline int Channel2(int c){
      return (c&0xff);
    }

    /**
     * \brief Inverts the given color, where red is reversed to cyan, green is reversed to magenta and blue is reversed to yellow, and vice versa.
     */
    int Inverse(int c);

    
  } /*end Color namespace*/

  typedef Color::sColorRGB sColorRGB;
  typedef Color::sColorHSV sColorHSV;
  
} /*end gft namespace*/

#endif

