
#ifndef _GFT_MORPHOLOGY3_H_
#define _GFT_MORPHOLOGY3_H_

#include "gft2/gft_scene8.h"
#include "gft2/gft_adjrel3.h"
#include "gft2/gft_set.h"
#include "gft2/gft_pqueue32.h"
#include "gft2/gft_pqueue8.h"
#include "gft2/gft_morphology.h"
#include "gft2/gft_marker3.h"


namespace gft{
  namespace Scene8{

    sScene8 *Dilate(sScene8 *scn, sAdjRel3 *A);
    sScene8 *Erode(sScene8 *scn, sAdjRel3 *A);

    sScene8 *DilateBin(sScene8 *bin, sSet **seed, float radius);
    sScene8 *ErodeBin(sScene8 *bin, sSet **seed, float radius);

    sScene8 *Open(sScene8 *scn, sAdjRel3 *A);
    sScene8 *Close(sScene8 *scn, sAdjRel3 *A);
    
    sScene8 *CloseBin(sScene8 *bin, float radius);
    sScene8 *OpenBin(sScene8 *bin, float radius);

    sScene8 *CloseHoles(sScene8 *scn);
    sScene8 *CloseHolesFromSeeds(sScene8 *scn,
				 sAdjRel3 *A,
				 int *S);
    
  } //end Scene8 namespace


  
  namespace Scene32{
    sScene32 *Dilate(sScene32 *scn, sAdjRel3 *A);
    sScene32 *Erode(sScene32 *scn, sAdjRel3 *A);

    sScene32 *CloseHoles(sScene32 *scn);
    sScene32 *CloseHolesSliceBySlice_z(sScene32 *scn);
    
    sScene32 *CloseHolesFromSeeds(sScene32 *scn,
				  sAdjRel3 *A,
				  int *S);

  } //end Scene32 namespace

  
} //end gft namespace

#endif
