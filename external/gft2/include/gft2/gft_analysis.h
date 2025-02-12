#ifndef _GFT_ANALYSIS_H_
#define _GFT_ANALYSIS_H_

#include "gft2/gft_common.h"
#include "gft2/gft_image32.h"
#include "gft2/gft_adjrel.h"
#include "gft2/gft_pqueue32.h"
#include "gft2/gft_stack.h"
#include "gft2/gft_queue.h"

namespace gft{
  namespace Image32{

    /**
     * \brief Calculates the Center of Gravity (CoG) for each label. 
     *
     * Returns an array of integers C, where C[x] is the CoG of the label x.
     * Each value p = C[x] is a pixel index, where p = x + y*ncols.
     */
    int *ComputeCoG(sImage32 *label);

    int ComputeBinCoG(sImage32 *bin);
    
    sImage32 *RegMin(sImage32 *img, sAdjRel *A);
    sImage32 *LabelBinComp(sImage32 *bin, sAdjRel *A);

    void SelectLargestComp(sImage32 *bin);
    
    //sImage32 *DistTrans(sImage32 *bin, sAdjRel *A, char side); 
    //sImage32 *SignedDistTrans(sImage32 *bin, sAdjRel *A, char side);

    sImage32 *GetObjBorder(sImage32 *bin);
    sImage32 *GetObjBorders(sImage32 *img, sAdjRel *A);
    sImage32 *GetObjBorders(sImage32 *img, sAdjRel *A, bool frame);
    sImage32 *GetBorders(sImage32 *img, sAdjRel *A);
    
    sImage32 *LabelContour(sImage32 *bin);
    sImage32 *LabelContour(sImage32 *bin, sImage32 *contourid);
    
    sImage32 *Mask2EDT(sImage32 *bin, sAdjRel *A,
		       char side, int limit, char sign);
    
    void Mask2EDT(sImage32 *bin, sAdjRel *A,
		  char side, int limit, char sign,
		  sImage32 *cost, sImage32 *root);

    sImage32 *Multiscaleskeletons(sImage32 *bin);


    float PerimeterLength(sImage32 *bin);

    int GetBinArea(sImage32 *bin);
   
    
  } //end Image32 namespace
} //end gft namespace

#endif

