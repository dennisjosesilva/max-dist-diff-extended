
#ifndef _GFT_EVALUATION_H_
#define _GFT_EVALUATION_H_

#include "gft_common.h"
#include "gft_image32.h"
#include "gft_image32f.h"
#include "gft_scene32.h"
#include "gft_scene8.h"
#include "gft_adjrel.h"
#include "gft_analysis.h"

namespace gft{
  namespace Image32{

    double   DiceSimilarity(sImage32 *mask1,
			    sImage32 *mask2);
    double   DiceSimilarity(sImage32 *trimap,
			    sImage32 *mask,
			    int ignoredval);
    double    JaccardSimilarity(sImage32 *mask1,
			       sImage32 *mask2);
    double    JaccardSimilarity(sImage32 *trimap,
			       sImage32 *mask,
			       int ignoredval);
    double    FuzzyJaccardSimilarity(sImage32 *prob1,
				    sImage32 *prob2,
				    int Pmax1,
				    int Pmax2);
    double    ErrorRateWithinBoundingBox(sImage32 *trimap,
					sImage32 *mask,
					int ignoredval,
					int xmin,
					int ymin,
					int xmax,
					int ymax);
    
    //mask1: Ground Truth
    //mask2: Segmentation Result
    double   GeneralBalancedJaccard(sImage32 *mask1,
				   sImage32 *mask2);
    double   GeneralBalancedJaccard(sImage32 *trimap,
				   sImage32 *mask,
				   int ignoredval);
    
    //mask1: Ground Truth
    //mask2: Segmentation Result
    int     AssessTP(sImage32 *mask1, sImage32 *mask2);
    int     AssessFN(sImage32 *mask1, sImage32 *mask2);
    int     AssessFP(sImage32 *mask1, sImage32 *mask2);
    int     AssessTN(sImage32 *mask1, sImage32 *mask2);
    
    sImage32 *GetObjError(sImage32 *gtruth,
			  sImage32 *mask);
    sImage32 *GetBkgError(sImage32 *gtruth,
			  sImage32 *mask);

    //mask1: Ground Truth
    //mask2: Segmentation Result
    double    BoundaryError(sImage32 *mask1,
			   sImage32 *mask2);
    double    BoundaryFP(sImage32 *mask1,
			sImage32 *mask2);
    double    BoundaryFN(sImage32 *mask1,
			sImage32 *mask2);

    float Sharpness(sImage32 *prob, int Pmax);
    float Sharpness(sImage32 *prob, sImage32 *mask, int Pmax);    
    
  } //end Image32 namespace


  namespace Image32f{

    float Sharpness(sImage32f *prob);

  } //end Image32f namespace

  
  namespace Scene32{

    double    DiceSimilarity(sScene32 *mask1,
			     sScene32 *mask2);
    
  } //end Scene32 namespace


  namespace Scene8{

    double    DiceSimilarity(sScene8 *mask1,
			     sScene8 *mask2);

    sScene8 *GetObjError(sScene8 *gtruth,
			 sScene8 *mask);
    sScene8 *GetBkgError(sScene8 *gtruth,
			 sScene8 *mask);
    
  } //end Scene8 namespace

  
} //end gft namespace


#endif

