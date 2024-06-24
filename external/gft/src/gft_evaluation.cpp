
#include "gft_evaluation.h"

namespace gft{
  namespace Image32{
    
    double DiceSimilarity(sImage32 *mask1, 
			  sImage32 *mask2){
      double nelems_intersec,nelems_union;
      int  p,n;
      
      /* compute similarity between shapes */
      n = mask1->n;
      nelems_intersec = nelems_union = 0.0;
      for(p=0; p<n; p++){
	if(mask1->data[p]>0){
	  nelems_union++;
	  if(mask2->data[p]>0)
	    nelems_intersec++;
	}
	else{
	  if(mask2->data[p]>0)
	    nelems_union++;
	}
      }
      return((2.0*nelems_intersec)/(nelems_union+nelems_intersec));
    }



    double DiceSimilarity(sImage32 *trimap, 
			  sImage32 *mask,
			  int ignoredval){
      double nelems_intersec,nelems_union;
      int  p,n;
      
      /* compute similarity between shapes */
      n = trimap->n;
      nelems_intersec = nelems_union = 0.0;
      for(p=0; p<n; p++){
	if(trimap->data[p] == ignoredval)
	  continue;

	if(trimap->data[p]>0){
	  nelems_union++;
	  if(mask->data[p]>0)
	    nelems_intersec++;
	}
	else{
	  if(mask->data[p]>0)
	    nelems_union++;
	}
      }
      return((2.0*nelems_intersec)/(nelems_union+nelems_intersec));
    }
    
    

    double    JaccardSimilarity(sImage32 *mask1,
			       sImage32 *mask2){
      double nelems_intersec,nelems_union;
      int  p,n;
      
      /* compute similarity between shapes */
      n = mask1->ncols * mask1->nrows;
      nelems_intersec = nelems_union = 0.0;
      for(p=0; p<n; p++){
	if(mask1->data[p]>0){
	  nelems_union++;
	  if(mask2->data[p]>0)
	    nelems_intersec++;
	}
	else{
	  if(mask2->data[p]>0)
	    nelems_union++;
	}
      }
      return(nelems_intersec/nelems_union);
    }



    double    JaccardSimilarity(sImage32 *trimap,
			       sImage32 *mask,
			       int ignoredval){
      double nelems_intersec,nelems_union;
      int  p,n;
      /* compute similarity between shapes */
      n = trimap->n;
      nelems_intersec = nelems_union = 0.0;
      for(p=0; p<n; p++){
	if(trimap->data[p] == ignoredval)
	  continue;
	
	if(trimap->data[p]>0){
	  nelems_union++;
	  if(mask->data[p]>0)
	    nelems_intersec++;
	}
	else{
	  if(mask->data[p]>0)
	    nelems_union++;
	}
      }
      return(nelems_intersec/nelems_union);
    }
    
    
    double    FuzzyJaccardSimilarity(sImage32 *prob1,
				    sImage32 *prob2,
				    int Pmax1,
				    int Pmax2){
      double nelems_intersec,nelems_union,pr1,pr2;
      int  p,n;
      
      /* compute similarity between shapes */
      n = prob1->n;
      nelems_intersec = nelems_union = 0.0;
      for(p=0; p<n; p++){
	pr1 = (double)prob1->data[p]/(double)Pmax1;
	pr2 = (double)prob2->data[p]/(double)Pmax2;
	nelems_union += MAX(pr1, pr2);
	nelems_intersec += MIN(pr1, pr2);
      }
      return(nelems_intersec/nelems_union);
    }



    double    ErrorRateWithinBoundingBox(sImage32 *trimap,
					sImage32 *mask,
					int ignoredval,
					int xmin,
					int ymin,
					int xmax,
					int ymax){
      double nerror,size;
      int  p,x,y;
      size = (xmax-xmin+1.0)*(ymax-ymin+1.0);
      nerror = 0.0;
      for(y = ymin; y <= ymax; y++){
	for(x = xmin; x <= xmax; x++){
	  p = x + y*mask->ncols;
	  if(trimap->data[p] == ignoredval)
	    continue;
	  
	  if(trimap->data[p]>0){
	    if(mask->data[p] == 0)
	      nerror++;
	  }
	  else{
	    if(mask->data[p]>0)
	      nerror++;
	  }
	}
      }
      return(100.0*(nerror/size));
    }
    

    //mask1: Ground Truth
    //mask2: Segmentation Result
    double   GeneralBalancedJaccard(sImage32 *mask1,
				   sImage32 *mask2){
      double gbc;
      int p,fn=0,fp=0,o=0;
      for(p=0; p < mask1->n; p++){
	if(mask1->data[p]>0){
	  o++;
	  if(mask2->data[p]==0)
	    fn++;
	}
	else if(mask1->data[p]==0 && mask2->data[p]>0)
	  fp++;
      }
      if(fp > o) gbc = 0.0;
      else  	 gbc = (double)(((double)(o-fp)*(double)(o-fn))/((double)o*((double)o)));
      return gbc;
    }



    //trimap: Ground Truth
    //mask: Segmentation Result
    double   GeneralBalancedJaccard(sImage32 *trimap,
				   sImage32 *mask,
				   int ignoredval){
      double gbc;
      int p,fn=0,fp=0,o=0;
      for(p=0; p < trimap->n; p++){
	if(trimap->data[p] == ignoredval)
	  continue;

	if(trimap->data[p]>0){
	  o++;
	  if(mask->data[p]==0)
	    fn++;
	}
	else if(trimap->data[p]==0 && mask->data[p]>0)
	  fp++;
      }
      if(fp > o) gbc = 0.0;
      else  	 gbc = (double)(((double)(o-fp)*(double)(o-fn))/((double)o*((double)o)));
      return gbc;
    }
    

    
    //mask1: Ground Truth
    //mask2: Segmentation Result
    int     AssessTP(sImage32 *mask1, sImage32 *mask2){
      int p,n,tp=0;
      
      n = mask1->ncols * mask1->nrows;
      for(p=0; p<n; p++){
	if(mask1->data[p]>0 && mask2->data[p]>0)
	  tp++;
      }
      return tp;
    }
    
    
    //mask1: Ground Truth
    //mask2: Segmentation Result
    int     AssessFN(sImage32 *mask1, sImage32 *mask2){
      int p,n,fn=0;
      
      n = mask1->ncols * mask1->nrows;
      for(p=0; p<n; p++){
	if(mask1->data[p]>0 && mask2->data[p]==0)
	  fn++;
      }
      return fn;
    }
    
    //mask1: Ground Truth
    //mask2: Segmentation Result
    int     AssessFP(sImage32 *mask1, sImage32 *mask2){
      int p,n,fp=0;
      
      n = mask1->ncols * mask1->nrows;
      for(p=0; p<n; p++){
	if(mask1->data[p]==0 && mask2->data[p]>0)
	  fp++;
      }
      return fp;
    }


    //mask1: Ground Truth
    //mask2: Segmentation Result
    int     AssessTN(sImage32 *mask1, sImage32 *mask2){
      int p,n,tn=0;
      
      n = mask1->ncols * mask1->nrows;
      for(p=0; p<n; p++){
	if(mask1->data[p]==0 && mask2->data[p]==0)
	  tn++;
      }
      return tn;
    }



    sImage32 *GetObjError(sImage32 *gtruth,
			  sImage32 *mask){
      sImage32 *err = NULL;
      int p,n;
      
      n = mask->ncols*mask->nrows;
      err = Create(mask->ncols, mask->nrows);
      
      for(p=0; p<n; p++){
	if(gtruth->data[p] > 0){
	  if(mask->data[p] == 0)
	    err->data[p] = 1;
	}
      }
      return err;
    }


    sImage32 *GetBkgError(sImage32 *gtruth,
			  sImage32 *mask){
      sImage32 *err = NULL;
      int p,n;
      
      n = mask->ncols*mask->nrows;
      err = Create(mask->ncols, mask->nrows);
      
      for(p=0; p<n; p++){
	if(gtruth->data[p] == 0){
	  if(mask->data[p] > 0)
	    err->data[p] = 1;
	}
      }
      return err;
    }
    

    //mask1: Ground Truth
    //mask2: Segmentation Result
    double    BoundaryError(sImage32 *mask1,
			   sImage32 *mask2){
      sImage32 *border,*dist2 = NULL;
      sAdjRel *A = gft::AdjRel::Circular(1.5);
      double mean,d = 0.0;
      int n,p,nb = 0;
      
      //dist2  = DistTrans(mask1, A, BOTH);
      border = GetObjBorder(mask2);

      n = mask1->ncols*mask1->nrows;
      for(p=0; p<n; p++){
	if(border->data[p]>0){
	  nb++;
	  d += sqrt((double)dist2->data[p]);
	}
      }
      Destroy(&border);
      Destroy(&dist2);
      gft::AdjRel::Destroy(&A);
      
      mean = (d/(double)nb);
      return mean;
    }


    //mask1: Ground Truth
    //mask2: Segmentation Result
    double    BoundaryFP(sImage32 *mask1,
			sImage32 *mask2){
      sImage32 *border,*dist2 = NULL;
      sAdjRel *A = gft::AdjRel::Circular(1.5);
      double mean,d = 0.0;
      int n,p,nb = 0;

      //dist2  = SignedDistTrans(mask1, A, BOTH);
      border = GetObjBorder(mask2);

      n = mask1->ncols*mask1->nrows;
      for(p=0; p<n; p++){
	if(border->data[p]>0){
	  nb++;
	  if(dist2->data[p]<0)
	    d += sqrtf((double)(-dist2->data[p]));
	}
      }
      
      Destroy(&border);
      Destroy(&dist2);
      gft::AdjRel::Destroy(&A);
      
      mean = (d/(double)nb);
      return mean;
    }


    //mask1: Ground Truth
    //mask2: Segmentation Result
    double    BoundaryFN(sImage32 *mask1,
			sImage32 *mask2){
      sImage32 *border,*dist2 = NULL;
      sAdjRel *A = gft::AdjRel::Circular(1.5);
      double mean,d = 0.0;
      int n,p,nb = 0;
      
      //dist2  = SignedDistTrans(mask1, A, BOTH);
      border = GetObjBorder(mask2);
      
      n = mask1->ncols*mask1->nrows;
      for(p=0; p<n; p++){
	if(border->data[p]>0){
	  nb++;
	  if(dist2->data[p]>0)
	    d += sqrtf((double)dist2->data[p]);
	}
      }
      
      Destroy(&border);
      Destroy(&dist2);
      gft::AdjRel::Destroy(&A);
      
      mean = (d/(double)nb);
      return mean;
    }



    float Sharpness(sImage32 *prob, int Pmax){
      float pr,sum;
      int p;
      sum = 0.0;
      for(p = 0; p < prob->n; p++){
	pr = (float)prob->data[p]/(float)Pmax;
	sum += 2*fabsf(pr-0.5);
      }
      return sum/prob->n;
    }


    float Sharpness(sImage32 *prob, sImage32 *mask, int Pmax){
      float pr,sum;
      int p,n;
      n = 0;
      sum = 0.0;
      for(p = 0; p < prob->n; p++){
	if(mask->data[p] > 0){
	  pr = (float)prob->data[p]/(float)Pmax;
	  sum += 2*fabsf(pr-0.5);
	  n++;
	}
      }
      return sum/n;
    }
    
    
  } //end Image32 namespace



  namespace Image32f{

    float Sharpness(sImage32f *prob){
      float pr,sum;
      int p;
      sum = 0.0;
      for(p = 0; p < prob->n; p++){
	pr = prob->data[p];
	sum += 2*fabsf(pr-0.5);
      }
      return sum/prob->n;
    }

  } //end Image32f namespace

  

  namespace Scene32{
  
    double DiceSimilarity(sScene32 *mask1, 
			 sScene32 *mask2){
      double nelems_intersec,nelems_union;
      int  p,n;
      
      /* compute similarity between shapes */
      n = mask1->n;
      nelems_intersec = nelems_union = 0.0;
      for(p = 0; p < n; p++){
	if(mask1->data[p]>0){
	  nelems_union++;
	  if(mask2->data[p]>0)
	    nelems_intersec++;
	}
	else{
	  if(mask2->data[p]>0)
	    nelems_union++;
	}
      }
      return((2.0*nelems_intersec)/(nelems_union+nelems_intersec));
    }
    
    
  } //end Scene32 namespace




  namespace Scene8{
  
    double DiceSimilarity(sScene8 *mask1, 
			  sScene8 *mask2){
      double nelems_intersec,nelems_union;
      int  p,n;
      
      /* compute similarity between shapes */
      n = mask1->n;
      nelems_intersec = nelems_union = 0.0;
      for(p = 0; p < n; p++){
	if(mask1->data[p]>0){
	  nelems_union++;
	  if(mask2->data[p]>0)
	    nelems_intersec++;
	}
	else{
	  if(mask2->data[p]>0)
	    nelems_union++;
	}
      }
      return((2.0*nelems_intersec)/(nelems_union+nelems_intersec));
    }



    sScene8 *GetObjError(sScene8 *gtruth,
			 sScene8 *mask){
      sScene8 *err = NULL;
      int p;
      err = Create(mask);
      for(p=0; p<mask->n; p++){
	if(gtruth->data[p] > 0){
	  if(mask->data[p] == 0)
	    err->data[p] = 1;
	}
      }
      return err;
    }

    
    sScene8 *GetBkgError(sScene8 *gtruth,
			 sScene8 *mask){
      sScene8 *err = NULL;
      int p;
      err = Create(mask);
      for(p=0; p<mask->n; p++){
	if(gtruth->data[p] == 0){
	  if(mask->data[p] > 0)
	    err->data[p] = 1;
	}
      }
      return err;
    }
    
    
  } //end Scene8 namespace


  
} //end gft namespace


