#ifndef _GFT_ANALYSIS3_H_
#define _GFT_ANALYSIS3_H_

#include "gft2/gft_common.h"
#include "gft2/gft_scene32.h"
#include "gft2/gft_scene8.h"
#include "gft2/gft_adjrel3.h"
#include "gft2/gft_pqueue32.h"
#include "gft2/gft_stack.h"
#include "gft2/gft_queue.h"

namespace gft{
  namespace Scene8{

    int ComputeBinCoG(sScene8 *bin);
    
    sScene8 *GetObjBorders(sScene8 *scn, sAdjRel3 *A);
    sScene8 *GetObjBorders(sScene8 *scn, sAdjRel3 *A, bool frame);
    
    sScene32 *Mask2EDT(sScene8 *bin, sAdjRel3 *A,
		       char side, int limit, char sign);
    
    sScene32 *LabelBinComp(sScene8 *bin, sAdjRel3 *A);
    void SelectLargestComp(sScene8 *bin, sAdjRel3 *A);
    void SelectLargestComp(sScene8 *bin);
    
    int GetBinArea(sScene8 *bin);
    
  } //end Scene8 namespace


} //end gft namespace

#endif

