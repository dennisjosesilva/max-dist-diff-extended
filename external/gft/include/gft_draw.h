#ifndef _GFT_DRAW_H_
#define _GFT_DRAW_H_

#include "gft_common.h"
#include "gft_image32.h"
#include "gft_cimage.h"
#include "gft_adjrel.h"
#include "gft_queue.h"

namespace gft{

  namespace Image32{

    void    DrawRectangle(sImage32 *img, 
			  int x1, int y1, 
			  int x2, int y2, int val);
    void    DrawLineDDA(sImage32 *img, 
			int x1, int y1, 
			int xn, int yn, int val);
    void    DrawCircle(sImage32 *img,
		       int x1, int y1,
		       float r,
		       int val);
    void    DrawTriangle(sImage32 *img,
			 int A, int B, int C,
			 int val);
    void    DrawTriangle(sImage32 *img,
			 int xA, int yA,
			 int xB, int yB,
			 int xC, int yC,
			 int val);
    
    void    DrawLine(sImage32 *img, 
		     int A, int B, 
		     float r, int val);
    void    DrawLine(sImage32 *img, 
		     int xA, int yA,
		     int xB, int yB,
		     float r, int val);
    void    DrawArrow(sImage32 *img, 
		      int A, int B,
		      float r, float w, float h,
		      int val);
    void    DrawArrow(sImage32 *img, 
		      int xA, int yA,
		      int xB, int yB,
		      float r, float w, float h,
		      int val);

    void    DrawGielisEquation(sImage32 *img,
			       int x,
			       int y,
			       float A,
			       float B,
			       float n1,
			       float n2,
			       float n3,
			       int m,
			       int val);
    
    void    FloodFill(sImage32 *img, sAdjRel *A,
		      int P, int val);


  } //end Image32 namespace



  namespace CImage{

    void    DrawRectangle(sCImage *cimg, 
			  int x1, int y1, 
			  int x2, int y2, int color);
    void    DrawLineDDA(sCImage *cimg,
			int x1, int y1, 
			int xn, int yn, int color);
    void    DrawCircle(sCImage *cimg,
		       int x1, int y1,
		       float r,
		       int color);
    void    DrawTriangle(sCImage *cimg,
			 int A, int B, int C,
			 int color);
    void    DrawTriangle(sCImage *cimg,
			 int xA, int yA,
			 int xB, int yB,
			 int xC, int yC,
			 int color);
    
    void    DrawLine(sCImage *cimg,
		     int A, int B, 
		     float r, int color);    
    void    DrawLine(sCImage *cimg, 
		     int xA, int yA,
		     int xB, int yB,
		     float r, int color);

    void    DrawArrow(sCImage *cimg,
		      int A, int B,
		      float r, float w, float h,
		      int color);
    void    DrawArrow(sCImage *cimg,
		      int xA, int yA,
		      int xB, int yB,
		      float r, float w, float h,
		      int color);

    
    void    FloodFill(sCImage *cimg, sAdjRel *A,
		      int P, int color);
    
  } //end CImage namespace    
  

} //end gft namespace


#endif

