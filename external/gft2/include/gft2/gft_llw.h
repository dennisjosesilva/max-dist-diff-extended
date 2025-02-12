
#ifndef _GFT_LWARCW_H_
#define _GFT_LWARCW_H_


#include "gft2/gft_common.h"
#include "gft2/gft_image32.h"
#include "gft2/gft_filtering.h"
#include "gft2/gft_set.h"
#include "gft2/gft_heap64f.h"
#include "gft2/gft_analysis.h"


namespace gft{

  namespace LLW{
  
    gft::sImage32 *WeightImage(gft::sImage32 *img,
			       float r);
    
    double *Curvature(gft::sImage32 *img,
		      gft::sImage32 *W,
		      gft::sImage32 *mask,
		      float r,
		      bool draw,
		      double **V_curv_x,
		      double **V_curv_y);
    //----------------------------------------

    double *Curvature(gft::sImage32 *img,
		      gft::sImage32 *spixels,
		      float r,
		      double **V_curv_x,
		      double **V_curv_y);
    
  } //end LLW namespace
 
} //end gft namespace

#endif

