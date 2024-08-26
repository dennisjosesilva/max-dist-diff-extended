
#ifndef _GFT_MARKER_H_
#define _GFT_MARKER_H_

#include "gft2/gft_common.h"
#include "gft2/gft_image32.h"
#include "gft2/gft_set.h"
#include "gft2/gft_adjrel.h"
#include "gft2/gft_analysis.h"
#include "gft2/gft_morphology.h"


namespace gft{
#define EXCEPT_BORDER_MAX_X    0b00000001
#define EXCEPT_BORDER_MIN_X    0b00000010
#define EXCEPT_BORDER_MAX_Y    0b00000100
#define EXCEPT_BORDER_MIN_Y    0b00001000
#define EXCEPT_BORDER_MAX_Z    0b00010000
#define EXCEPT_BORDER_MIN_Z    0b00100000
  
  namespace Image32{
    
    float  MaxRadiusByErosion(sImage32 *bin);
    float  MaxObjRadiusByErosion(sImage32 *bin);
    float  MaxBkgRadiusByErosion(sImage32 *bin);
    
    sImage32 *ObjMaskByErosion(sImage32 *bin, float radius);
    sImage32 *BkgMaskByErosion(sImage32 *bin, float radius);

    int  *GetMarkers(sImage32 *label, sAdjRel *A);

    int *GetImageBorder(sImage32 *img);
    int *GetImageBorder(sImage32 *img,
			uchar exception_flags);
    
   } //end Image32 namespace
} //end gft namespace

 
#endif
