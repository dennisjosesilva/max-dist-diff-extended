
#ifndef _GFT_IFT_H_
#define _GFT_IFT_H_

#include "gft_common.h"
#include "gft_imagegraph.h"
#include "gft_pqueue32.h"
#include "gft_queue.h"
#include "gft_heap.h"
#include "gft_heap32.h"
#include "gft_heap_lex.h"
#include "gft_heap32fi_lex.h"
#include "gft_heap32fifi_lex.h"
#include "gft_heap32fiif_lex.h"
#include "gft_heap32fif_lex.h"
#include "gft_heappair.h"
#include "gft_stack.h"
#include "gft_graph.h"
#include "gft_set.h"
#include "gft_morphology.h"
#include "gft_layeredgraph.h"
#include "gft_analysis.h"
#include "gft_image32.h"
#include "gft_scene32.h"
#include "gft_adjrel3.h"
#include "gft_adjrel.h"
#include "gft_band.h"

//----------------------------------------
//  Method wrappers:

//The 'label' scene should be pre-initialized as follows:
//  label->data[p]=NIL, unlabeled voxel.
//  label->data[p]=0,   background voxel.
//  label->data[p]=1,   object#1 voxel.
//  label->data[p]=2,   object#2 voxel.
//  ...
//---------------------------------------

namespace gft{
  namespace ift{


    //---------------------------------------
    /* Star Convexity - ICIP 2013 */
    sImage32 *SC_Pred_fsum(sImageGraph *sg,
			   int *S, float power);
    sScene32 *SC_Pred_fsum(sGraph *graph,
			   sScene32 *scn,
			   int *S, float power);    
    sScene32 *SC_Pred_fsum(sScene32 *W,
			   sAdjRel3 *A,
			   int *S, float power);
    
    void SC_IFT(sImageGraph *sg,
		int *S,
		sImage32 *label,
		sImage32 *P_sum);
    //---------------------------------------
    
    //Outer Cut:
    int GetEnergy_Min(sImageGraph *sg,
		      sImage32 *label,
		      int lb);
    int GetEnergy_Max(sImageGraph *sg,
		      sImage32 *label,
		      int lb);
    long long GetEnergy_Sum(sImageGraph *sg,
			    sImage32 *label,
			    int lb);

    int GetEnergy_Min(sGraph *graph,
		      int *label,
		      int lb);

    float GetEnergy_Mean(sGraph *graph,
			 int *label,
			 int lb);
    
    //---------------------------------------
    // ORFC:

    //Outer Cut (the polarity must be embedded in the graph):
    void ORFC(sImageGraph *sg,
	      int *S,
	      sImage32 *label);
 

    //---------------------------------------
    // OIFT:
    
    //Outer Cut:
    void OIFT(sImage32 *W,
	      sAdjRel *A,
	      sImage32 *img,
	      float per,
	      int *S,
	      sImage32 *label);

    //Outer Cut:
    void OIFT(sAdjRel3 *A,
	      sScene32 *scn,
	      float per,
	      int *S,
	      sScene32 *label);
    
    //Outer Cut:
    void OIFT(sScene32 *W,
	      sAdjRel3 *A,
	      sScene32 *scn,
	      float per,
	      int *S,
	      sScene32 *label);

    //Outer Cut:
    void OIFT(sScene32 *Wx,
	      sScene32 *Wy,
	      sScene32 *Wz,
	      sScene32 *scn,
	      float per,
	      int *S,
	      sScene32 *label);
    
    //Outer Cut (the polarity must be embedded in the graph):
    void OIFT(sImageGraph *sg,
	      int *S,
	      sImage32 *label);

    void OIFT_MinMax(sImageGraph *sg,
		     int *S,
		     sImage32 *label,
		     sImage32 *pred,
		     sImage32 *value,
		     int niter);

    void OIFT_MaxMin(sImageGraph *sg,
		     int *S,
		     sImage32 *label,
		     sImage32 *pred,
		     sImage32 *value,
		     int niter);
    
    void OIFT_TZ2Bkg(sImageGraph *sg,
		     int *S,
		     sImage32 *label);
    void OIFT_TZ2Obj(sImageGraph *sg,
		     int *S,
		     sImage32 *label);
    
    void OIFT_TZ(sImageGraph *sg,
		 int *S,
		 sImage32 *label);

    bool isOIFT(sImageGraph *sg,
		int *S,
		sImage32 *Slabel,
		sImage32 *label,
		sImage32 *pred,
		sImage32 *ord,
		bool complete_check);
    
    bool isOIFT_Segmentation(sImageGraph *sg,
			     int *S,
			     sImage32 *Slabel,
			     sImage32 *label);

    bool isOIFT_Forest(sImageGraph *sg,
		       int *S,
		       sImage32 *Slabel,
		       sImage32 *pred,
		       sImage32 *ord);

    bool isForest(sImageGraph *sg,
		  sImage32 *pred);

    //Remover:
    void OIFT_guided(sImageGraph *sg,
		     int *S,
		     sImage32 *label,
		     sImage32 *pred,
		     sImage32 *ord);
    
    //Outer Cut (the polarity must be embedded in the graph):
    void OIFT(sGraph *graph,
	      sGraph *transpose,
	      int *S,
	      int *label);

    //Inner Cut (the polarity must be embedded in the graph):
    void OIFT_in(sImageGraph *sg,
		 int *S,
		 sImage32 *label);

    void OIFT_Heap(sImageGraph *sg,
		   int *S,
		   sImage32 *label);
    void OIFT_Heap(sGraph *graph,
		   sGraph *transpose,
		   int *S,
		   int *label);
        
    //Outer Cut (the polarity must be embedded in the graph):
    //Energy based implementations.

    void EOIFT(sImageGraph *sg,
	       int *S,
	       sImage32 *label,
	       int e_max);

    void EOIFT(sGraph *graph,
	       sGraph *transpose,
	       int *S,
	       int *label,
	       int e_max);

    void EOIFT_Heap(sImageGraph *sg,
		    int *S,
		    sImage32 *label,
		    float e_max);
    void EOIFT_Heap_2(sImageGraph *sg,
		      int *S,
		      sImage32 *label,
		      float e_max);

    void EOIFT_Heap(sGraph *graph,
		    sGraph *transpose,
		    int *S,
		    int *label,
		    float e_max);
    
    void EOIFT_Heap_2(sGraph *graph,
		      sGraph *transpose,
		      int *S,
		      int *label,
		      float e_max);

    //---------------------------------------

    void IFT_fmax(sGraph *graph,
		  int *S,
		  int *label,
		  int *cost);
     
    void IFT_fmax_Heap(sGraph *graph,
		       int *S,
		       int *label,
		       float *cost);

    void IFT_feuc(sImageGraph *g,
		  int *S,
		  sImage32 *label,
		  sImage32 *cost,
		  sImage32 *pred);
    
    void IFT_fw(sImageGraph *g,
		int *S,
		sImage32 *label,
		sImage32 *cost,
		sImage32 *pred);

    void IFT_fw(sGraph *graph,
		int *S,
		int *label,
		int *cost);

    void IFT_fw(sGraph *graph,
		int *S,
		int *label,
		int *cost,
		int *pred);
        
    void IFT_fw_Heap(sGraph *graph,
		     int *S,
		     int *label,
		     float *cost,
		     int *pred);
    
    //---------------------------------------
    sImage32 *Cost_fmin(sImageGraph *sg,
			int *S, int lb,
			sImage32 *label);

    //---------------------------------------
    // OIFT with Connectivity Constraints:

    /* To be used by COIFT */
    sImage32 *Cost_fmax(sImageGraph *sg,
			int *S, int lb,
			sImage32 *label);
    
    void COIFT(sImageGraph *sg,
	       int *S,
	       sImage32 *label);
  
    //---------------------------------------
    //Boundary Band (BB) - ICIP 2014

    sImage32 *BB_Geodesic_Cost(sImage32 *pred,
			       sAdjRel *A);
    sScene32 *BB_Geodesic_Cost(sScene32 *pred,
			       sAdjRel3 *A);
    
    sImage32 *BB_CropTemplate(sImage32 *cost_template,
			      int *S,
			      sImage32 *label,
			      int lb);
    
    void BB_OIFT(sImageGraph *sg,
		 int *S,
		 sImage32 *L,
		 sImage32 *C,
		 sImage32 *P,
		 float delta);


    void RBB_OIFT(sImageGraph *sg,
		  int *S,
		  sImage32 *L,
		  sImage32 *C,
		  sImage32 *P,
		  float delta);

    void B_OIFT(sImageGraph *sg,
		int *S,
		sImage32 *L,
		sImage32 *C,
		float delta);

    void B_OIFT(sGraph *graph,
		sGraph *transpose,
		int *S,
		int *L,
		int *C,
		float delta);
    
    //---------------------------------------

    int *GetSeedsByLabel(int *S,
			 sImage32 *label,
			 int lb);
    int *GetAllInternalSeedsByLabel(int *S,
				    sImage32 *label,
				    int lb,
				    int *hr,
				    int nlayers);
    sImageGraph *GetPolarityGraph(sImageGraph *graph,
				  sCImage *cimg,
				  sImage32 *img,
				  int pol);
    
    void HL_OIFT(sImageGraph *graph,
		 sCImage *cimg,
		 sImage32 *img,
		 float radius,
		 char *hierarchy,
		 int *S,
		 sImage32 *label);
    
    void HL_OIFT(sLayeredGraph *lg,
		 int Wmax, int *S, int *L, int *hr);

    void HL_OIFT_2(sLayeredGraph *lg,
		   int Wmax, int *S, int *L, int *hr);

    //---------------------------------------

    //Compute watershed by fpeak from markers
    void IFT_fpeak(sImage32 *grad,
		   sAdjRel *A,
		   sImage32 *label);
    
    //Compute watershed by fwv from markers
    void IFT_fwv(sImage32 *grad,
		 sAdjRel *A,
		 sImage32 *label);

    //--------------------------------------

    void RelaxMobj(sImageGraph *sg,
		   int *S,
		   sImage32 *label,
		   int ntimes);

    //beta = 1;
    float *ORelax_dual(sImageGraph *sg,
		       int *S,
		       sImage32 *label, int ntimes);

    //beta = 1;
    float *Relax_dual(sImageGraph *sg,
		      int *S,
		      sImage32 *label,
		      int ntimes);
    
    void Relax(sImageGraph *sg,
	       int *S,
	       sImage32 *label,
	       int ntimes);

    void ORelax_1(sImageGraph *sg,
		  int *S,
		  sImage32 *label,
		  int ntimes);

    void ORelax(sImageGraph *sg,
		int *S,
		sImage32 *label,
		int ntimes);

    void ORelax_i(sImageGraph *sg,
		  int *S,
		  sImage32 *label,
		  int ntimes);

    void ORelax_s(sImageGraph *sg,
		  int *S,
		  sImage32 *label,
		  int ntimes);    
    //--------------------------------------
    void ORelax_1(sAdjRel3 *A,
		  sScene32 *scn,
		  float per,
		  int *S,
		  sScene32 *label,
		  int ntimes);

    void ORelax_1(sScene32 *W,
		  sAdjRel3 *A,
		  sScene32 *scn,
		  float per,
		  int *S,
		  sScene32 *label,
		  int ntimes);

    void ORelax_1(sScene32 *Wx,
		  sScene32 *Wy,
		  sScene32 *Wz,
		  sScene32 *scn,
		  float per,
		  int *S,
		  sScene32 *label,
		  int ntimes);
    
    //--------------------------------------
    void GGC_maxmin(sImageGraph *graph,
		    sCImage *cimg,
		    sImage32 *img,
		    int method,
		    float power_geodesic,
		    float delta,
		    float theta_hedgehog,
		    float R,
		    int postproc,
		    int niterations,
		    int conn,
		    int pol,
		    int shapepriors,
		    int costtemplate,
		    int *S,
		    sImage32 *label);

    //--------------------------------------

    void DOIFT_removeSubTree(int q_in,
			     sHeap *Q,
			     sImage32 *pred,
			     sImage32 *root,
			     sAdjRel *A);

    void DOIFT_removeSubTree(int q_in,
			     sHeap *Q,
			     int *pred,
			     int *root,
			     sGraph *graph);
    
    void DOIFT_PaintSubTree(int r,
			    sImageGraph *sg,
			    sImage32 *pred,
			    sImage32 *img,
			    int val);
    
    void DOIFT_TreeRemoval(int *R,
			   sImageGraph *sg,
			   sHeap32fiif_lex *Q,
			   sImage32 *pred,
			   sImage32 *root,
			   sImage32 *maxorder,
			   sImage32 *order);

    void DOIFT_TreeRemoval(int *R,
			   sImageGraph *sg,
			   sHeap *Q,
			   sImage32 *pred,
			   sImage32 *root);

    void DOIFT_TreeRemoval(int *R,
			   sGraph *graph,
			   sGraph *transpose,			   
			   sHeap *Q,
			   int *pred,
			   int *root);
    
    //Outer Cut (the polarity must be embedded in the graph):
    //GIFT with first OIFT path-cost function (i.e., fmax).
    void DOIFT(sImageGraph *sg,
	       int *S,
	       int *R,
	       sImage32 *label,
	       sHeap *Q,
	       sImage32 *pred,
	       sImage32 *root);

    void DOIFT(sGraph *graph,
	       sGraph *transpose,
	       int *S,
	       int *R,
	       int *label,
	       sHeap *Q,
	       int *pred,
	       int *root);
    
    //-------------------------------------------
    //Efficient version for OIFT segmentation:
    
    /*
    void DOIFT_TreeRemoval_2(int *R,
			     sImageGraph *sg,
			     sHeap32fi_lex *Q,
			     sImage32 *pred,
			     //sImage32 *root,
			     sImage32 *maxorder,
			     int *i_inv,
			     int *T);
    */

    void DOIFT_TreeRemoval_5(int *R,
			     sImageGraph *sg,
			     sHeap32fi_lex *Q,
			     sImage32 *pred,
			     sImage32 *maxorder,
			     int *T);

    void DOIFT_TreeRemoval_5(int *R,
			     sGraph *graph,
			     sHeap32fi_lex *Q,
			     int *pred,
			     int *maxorder,
			     int *T);


    void DOIFT_TreeRemoval__5(int *R,
			      sImageGraph *sg,
			      sHeap32fif_lex *Q,
			      sImage32 *pred,
			      sImage32 *maxorder,
			      int *T);
    
    //Outer Cut (the polarity must be embedded in the graph):
    void DOIFT_2(sImageGraph *sg,
		 int *S,
		 int *R,
		 sImage32 *label,
		 sHeap32fi_lex *Q,
		 sImage32 *pred,
		 //sImage32 *root,
		 sImage32 *maxorder,
		 int *iter);

    void DOIFT__2(sImageGraph *sg,
		  int *S,
		  int *R,
		  sImage32 *label,
		  sHeap32fi_lex *Q,
		  sImage32 *pred,
		  sImage32 *maxorder,
		  int *iter);

    //Outer Cut (the polarity must be embedded in the graph):
    void DOIFT__2(sGraph *graph,
		  sGraph *transpose,
		  int *S,
		  int *R,
		  int *label,
		  sHeap32fi_lex *Q,
		  int *pred,
		  int *maxorder,
		  int *iter);

    void DOIFT__2p(sImageGraph *sg,
		   int *S,
		   int *R,
		   sImage32 *label,
		   sHeap32fi_lex *Q,
		   sImage32 *pred,
		   sImage32 *maxorder,
		   int *iter);

    //Outer Cut (the polarity must be embedded in the graph):
    void DOIFT__2p(sGraph *graph,
		   sGraph *transpose,
		   int *S,
		   int *R,
		   int *label,
		   sHeap32fi_lex *Q,
		   int *pred,
		   int *maxorder,
		   int *iter);

    void DOIFT__2p_hierarchy(sGraph *graph,
			     sGraph *transpose,
			     int *S,
			     int *R,
			     int *label,
			     sHeap32fi_lex *Q,
			     int *pred,
			     int *maxorder,
			     sHeap32 *E,
			     int *H,
			     int *obj_area);

    int *OIFT_area_hierarchy(sGraph *graph,
			     sGraph *transpose,
			     int *S0,
			     int *S1);
			     //gft::sImage32 *SP);
    
    void DOIFT__3(sImageGraph *sg,
		  int *S,
		  int *R,
		  sImage32 *label,
		  sHeap32fif_lex *Q,
		  sImage32 *pred,
		  sImage32 *maxorder,
		  int *iter);
    
    //----------------------------------------
    //More general and theoretical approach.
    //Solves graph_doift_12.txt    

    //Outer Cut (the polarity must be embedded in the graph):
    void DOIFT_(sImageGraph *sg,
		int *S,
		int *R,
		sImage32 *label,
		sHeap32fiif_lex *Q,
		sImage32 *pred,
		sImage32 *root,
		sImage32 *maxorder,
		sImage32 *order,	       
		int *iter);
    
  } //end ift namespace
} //end gft namespace

#endif

