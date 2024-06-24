
#include "gft_draw.h"

namespace gft{
  namespace Image32{

    void DrawRectangle(sImage32 *img, 
		       int x1, int y1, 
		       int x2, int y2, int val){
      int i,j;
      for(i=y1; i<=y2; i++){
	for(j=x1; j<=x2; j++){
	  img->array[i][j] = val;
	}
      }
    }


    void DrawLineDDA(sImage32 *img, 
		     int x1, int y1, 
		     int xn, int yn, int val){
      int vx, vy;
      float Dx, Dy;
      int amostras; /* numero de pontos a serem pintados */
      float m; /* coeficiente angular da reta */
      int i;
      float xk, yk;
      int p;
      
      vx = xn - x1;
      vy = yn - y1;
      
      if (vx == 0){
	Dx = 0.0;
	Dy = (float) SIGN(vy);
	amostras = abs(vy)+1;
      }
      else{
	m = ((float)vy )/((float)vx);
	if ( abs(vx) > abs(vy)){
	  Dx = (float) SIGN(vx);
	  Dy = m * Dx;
	  amostras = abs(vx)+1;
	}
	else{
	  Dy = (float) SIGN(vy);
	  Dx = Dy / m;
	  amostras = abs(vy)+1;
	}
      }
      
      xk = (float) x1;
      yk = (float) y1;
      for (i = 0; i < amostras; i++){
	if ( IsValidPixel(img, ROUND(xk), ROUND(yk)) ){
	  p = ROUND(xk)+img->ncols*(ROUND(yk));
	  img->data[p] = val;
	}
	xk += Dx;
	yk += Dy;
      }
    }
    

    void    DrawCircle(sImage32 *img,
		       int x1, int y1,
		       float r,
		       int val){
      int i,j;
      for(i=y1 - ROUND(r+1); i<=y1 + ROUND(r+1); i++){
	for(j=x1 - ROUND(r+1); j<=x1 + ROUND(r+1); j++){
	  if(IsValidPixel(img, j, i)){
	    if((i-y1)*(i-y1)+(j-x1)*(j-x1) <= r*r)
	      img->array[i][j] = val;
	  }
	}
      }
    }


    void    DrawTriangle(sImage32 *img,
			 int xA, int yA,
			 int xB, int yB,
			 int xC, int yC,
			 int val){
      gft::sImage32 *tmp;
      gft::sAdjRel *Adj;
      gft::Pixel PX;
      int xmin,xmax,ymin,ymax;
      int Atx,Aty,Btx,Bty,Ctx,Cty;
      int tval,bkgval;
      int p;
      xmin = xA; xmin = MIN(xmin, xB); xmin = MIN(xmin, xC);
      xmax = xA; xmax = MAX(xmax, xB); xmax = MAX(xmax, xC);
      ymin = yA; ymin = MIN(ymin, yB); ymin = MIN(ymin, yC);
      ymax = yA; ymax = MAX(ymax, yB); ymax = MAX(ymax, yC);
      if(xmax-xmin+1+2 <= 0 || ymax-ymin+1+2 <= 0)
      	return;
      tmp = gft::Image32::Create(xmax-xmin+1+2, ymax-ymin+1+2);
      
      Atx = xA - xmin + 1; Aty = yA - ymin + 1;
      Btx = xB - xmin + 1; Bty = yB - ymin + 1;
      Ctx = xC - xmin + 1; Cty = yC - ymin + 1;
      tval = val - 1;
      bkgval = tval - 1;
      gft::Image32::Set(tmp, tval);
      DrawLine(tmp, Atx, Aty, Btx, Bty, 0.0, val);
      DrawLine(tmp, Btx, Bty, Ctx, Cty, 0.0, val);
      DrawLine(tmp, Ctx, Cty, Atx, Aty, 0.0, val);
      Adj = gft::AdjRel::Neighborhood_4();
      gft::Image32::FloodFill(tmp, Adj, 0, bkgval);
      for(p = 0; p < tmp->n; p++){
	if(tmp->data[p] == tval)
	  tmp->data[p] = val;
      }
      PX.x = xmin - 1;
      PX.y = ymin - 1;
      gft::Image32::Copy(img, tmp, PX, bkgval);
      gft::Image32::Destroy(&tmp);
      gft::AdjRel::Destroy(&Adj);
    }


    void    DrawTriangle(sImage32 *img,
			 int A, int B, int C,
			 int val){
      DrawTriangle(img,
		   A%img->ncols, A/img->ncols,
		   B%img->ncols, B/img->ncols,
		   C%img->ncols, C/img->ncols,
		   val);
    }


    void    DrawLine(sImage32 *img, 
		     int xA, int yA,
		     int xB, int yB,
		     float r, int val){
      int x1, y1, xn, yn, vx, vy;
      float Dx, Dy;
      int amostras; /* numero de pontos a serem pintados */
      float m; /* coeficiente angular da reta */
      int i;
      float xk, yk;
      x1 = xA; y1 = yA;
      xn = xB; yn = yB;
      vx = xn - x1; 
      vy = yn - y1;
      
      if(vx == 0){  
	Dx = 0.0;
	Dy = (float) SIGN(vy);
	amostras = abs(vy)+1;
      }
      else{  
	m = ((float)vy )/((float)vx);
	if( abs(vx) > abs(vy)){
	  Dx = (float) SIGN(vx);
	  Dy = m * Dx;
	  amostras = abs(vx)+1; 
	}
	else{  
	  Dy = (float) SIGN(vy);
	  Dx = Dy / m;
	  amostras = abs(vy)+1;
	}
      }
   
      xk = (float) x1;
      yk = (float) y1;
      for(i = 0; i < amostras; i++){ 
	if( gft::Image32::IsValidPixel(img,ROUND(xk),ROUND(yk)) )
	  DrawCircle(img, ROUND(xk), ROUND(yk), r, val);
	xk += Dx;
	yk += Dy;
      }
    }
    

    void    DrawLine(sImage32 *img, 
		     int A, int B, 
		     float r, int val){
      int x1, y1, xn, yn;
      x1 = A%img->ncols; y1 = A/img->ncols;
      xn = B%img->ncols; yn = B/img->ncols;
      DrawLine(img, x1, y1, xn, yn, r, val);
    }


    void    DrawArrow(sImage32 *img, 
		      int xA, int yA,
		      int xB, int yB,
		      float r, float w, float h,
		      int val){
      float dlm,dlx,dly,dtx,dty;
      int xC,yC,xD,yD;
      
      if(xA == xB && yA == yB)
	return;
      
      // A vector along the arc:
      dlm = hypot(xB - xA, yB - yA);
      dlx = (xB - xA)/dlm;
      dly = (yB - yA)/dlm;
      // A vector orthogonal to the arc:
      dtx = -dly;
      dty =  dlx;
      // Arrowhead corners:
      xC = xB - ROUND(h*dlx + w*dtx);
      yC = yB - ROUND(h*dly + w*dty);
      xD = xB - ROUND(h*dlx - w*dtx);
      yD = yB - ROUND(h*dly - w*dty);
      // Draw arc from p to q:
      DrawLine(img,
	       xA, yA,
	       ROUND(xA + dlx*(dlm-h)), ROUND(yA + dly*(dlm-h)),
	       r, val);
      // Paint arrowhead:
      DrawTriangle(img,
		   xB, yB,
		   xC, yC,
		   xD, yD, val);      
    }

    
    void    DrawArrow(sImage32 *img, 
		      int A, int B,
		      float r, float w, float h,
		      int val){
      int xB,yB,xA,yA;
      xB = B%img->ncols;
      yB = B/img->ncols;
      xA = A%img->ncols;
      yA = A/img->ncols;
      DrawArrow(img, 
		xA, yA,
		xB, yB,
		r, w, h,
		val);      
    }


    void    DrawGielisEquation(sImage32 *img,
			       int x,
			       int y,
			       float A,
			       float B,
			       float n1,
			       float n2,
			       float n3,
			       int m,
			       int val){
      gft::sAdjRel *Adj;
      float sum, r, dx, dy, phi;
      int i,px,py,p; //qx,qy,C;
      for(p = 0; p < img->n; p++){
	px = p%img->ncols;
	py = p/img->ncols;
	dx = px - x;
	dy = py - y;
	phi = atan2f(dy, dx);
	if(phi < 0.0) phi += 2*PI;

	sum  = powf(fabsf(cosf((m/4.0)*phi)/A), n2);
	sum += powf(fabsf(sinf((m/4.0)*phi)/B), n3);
	r = powf(sum, -1.0/n1);

	if(dx*dx + dy*dy <= r*r)
	  img->data[p] = val;
	/*
	dx = r*cosf(i);
	dy = r*sinf(i);
	px = ROUND(x + dx);
	py = ROUND(y + dy);
	if(i > 0){
	  DrawLine(img,
		   qx, qy,
		   px, py,
		   0.0, val);
	}
	qx = px;
	qy = py;
	*/
      }
      /*
      C = x + y*img->ncols;
      Adj = gft::AdjRel::Neighborhood_4();
      gft::Image32::FloodFill(tmp, Adj, C, val);
      */
    }

    

    void FloodFill(sImage32 *img, sAdjRel *A,
		   int P, int val){
      gft::sQueue *Q;
      int old, p, q, i;
      int px,py,qx,qy;
      if(P < 0 || P >= img->n)
	return;
      old = img->data[P];
      if(old == val)
	return;
      Q = gft::Queue::Create(img->n);
      gft::Queue::Push(Q, P);
      img->data[P] = val;
      while(!gft::Queue::IsEmpty(Q)){
	p = gft::Queue::Pop(Q);
	px = p%img->ncols;
	py = p/img->ncols;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(img, qx, qy)){
	    q = qx + qy*img->ncols;
	    if(img->data[q] == old){
	      gft::Queue::Push(Q, q);
	      img->data[q] = val;
	    }
	  }
	}
      }
      gft::Queue::Destroy(&Q);
    }
   

  } /*end Image32 namespace*/




  namespace CImage{

    
    void    DrawRectangle(sCImage *cimg, 
			  int x1, int y1, 
			  int x2, int y2, int color){
      DrawRectangle(cimg->C[0],
		    x1, y1, x2, y2, gft::Color::Channel0(color));
      DrawRectangle(cimg->C[1],
		    x1, y1, x2, y2, gft::Color::Channel1(color));
      DrawRectangle(cimg->C[2],
		    x1, y1, x2, y2, gft::Color::Channel2(color));
    }

    void    DrawLineDDA(sCImage *cimg,
			int x1, int y1, 
			int xn, int yn, int color){
      DrawLineDDA(cimg->C[0],
		  x1, y1, xn, yn, gft::Color::Channel0(color));
      DrawLineDDA(cimg->C[1],
		  x1, y1, xn, yn, gft::Color::Channel1(color));
      DrawLineDDA(cimg->C[2],
		  x1, y1, xn, yn, gft::Color::Channel2(color));
    }

    void    DrawCircle(sCImage *cimg,
		       int x1, int y1,
		       float r,
		       int color){
      DrawCircle(cimg->C[0],
		 x1, y1, r,
		 gft::Color::Channel0(color));
      DrawCircle(cimg->C[1],
		 x1, y1, r,
		 gft::Color::Channel1(color));
      DrawCircle(cimg->C[2],
		 x1, y1, r,
		 gft::Color::Channel2(color));
    }




    void    DrawTriangle(sCImage *cimg,
			 int xA, int yA,
			 int xB, int yB,
			 int xC, int yC,
			 int color){
      gft::sImage32 *tmp;
      gft::sAdjRel *Adj;
      gft::Pixel PX;
      int xmin,xmax,ymin,ymax;
      int Atx,Aty,Btx,Bty,Ctx,Cty,tval,bkgval;
      int p, ncols;
      ncols = (cimg->C[0])->ncols;
      xmin = xA; xmin = MIN(xmin, xB); xmin = MIN(xmin, xC);
      xmax = xA; xmax = MAX(xmax, xB); xmax = MAX(xmax, xC);
      ymin = yA; ymin = MIN(ymin, yB); ymin = MIN(ymin, yC);
      ymax = yA; ymax = MAX(ymax, yB); ymax = MAX(ymax, yC);
      if(xmax-xmin+1+2 <= 0 || ymax-ymin+1+2 <= 0)
	return;
      tmp = gft::Image32::Create(xmax-xmin+1+2, ymax-ymin+1+2);
      
      Atx = xA - xmin + 1; Aty = yA - ymin + 1;
      Btx = xB - xmin + 1; Bty = yB - ymin + 1;
      Ctx = xC - xmin + 1; Cty = yC - ymin + 1;
      tval = gft::Color::Channel0(color);
      tval = MIN(tval, gft::Color::Channel1(color));
      tval = MIN(tval, gft::Color::Channel2(color)) - 1;
      bkgval = tval - 1;
      gft::Image32::Set(tmp, tval);
      DrawLine(tmp, Atx, Aty, Btx, Bty, 0.0, gft::Color::Channel0(color));
      DrawLine(tmp, Btx, Bty, Ctx, Cty, 0.0, gft::Color::Channel0(color));
      DrawLine(tmp, Ctx, Cty, Atx, Aty, 0.0, gft::Color::Channel0(color));
      Adj = gft::AdjRel::Neighborhood_4();
      gft::Image32::FloodFill(tmp, Adj, 0, bkgval);
      for(p = 0; p < tmp->n; p++){
	if(tmp->data[p] == tval)
	  tmp->data[p] = gft::Color::Channel0(color);
      }
      PX.x = xmin - 1;
      PX.y = ymin - 1;
      gft::Image32::Copy(cimg->C[0], tmp, PX, bkgval);
      gft::Image32::Set(tmp,
			gft::Color::Channel0(color),
			gft::Color::Channel1(color));
      gft::Image32::Copy(cimg->C[1], tmp, PX, bkgval);
      gft::Image32::Set(tmp,
			gft::Color::Channel1(color),
			gft::Color::Channel2(color));
      gft::Image32::Copy(cimg->C[2], tmp, PX, bkgval);      
      gft::Image32::Destroy(&tmp);
      gft::AdjRel::Destroy(&Adj);
    }

    

    void    DrawTriangle(sCImage *cimg,
			 int A, int B, int C,
			 int color){
      int ncols;
      ncols = (cimg->C[0])->ncols;
      DrawTriangle(cimg,
		   A%ncols, A/ncols,
		   B%ncols, B/ncols,
		   C%ncols, C/ncols,
		   color);
   }

    

    void    DrawLine(sCImage *cimg,
		     int A, int B, 
		     float r, int color){
      DrawLine(cimg->C[0],
	       A, B, 
	       r, gft::Color::Channel0(color));
      DrawLine(cimg->C[1],
	       A, B, 
	       r, gft::Color::Channel1(color));
      DrawLine(cimg->C[2],
	       A, B, 
	       r, gft::Color::Channel2(color));
    }



    void    DrawLine(sCImage *cimg, 
		     int xA, int yA,
		     int xB, int yB,
		     float r, int color){
      DrawLine(cimg->C[0],
	       xA, yA, xB, yB, 
	       r, gft::Color::Channel0(color));
      DrawLine(cimg->C[1],
	       xA, yA, xB, yB, 
	       r, gft::Color::Channel1(color));
      DrawLine(cimg->C[2],
	       xA, yA, xB, yB,
	       r, gft::Color::Channel2(color));
    }
   
    

    void    DrawArrow(sCImage *cimg,
		      int A, int B,
		      float r, float w, float h,
		      int color){
      DrawArrow(cimg->C[0],
		A, B,
		r, w, h,
		gft::Color::Channel0(color));
      DrawArrow(cimg->C[1],
		A, B,
		r, w, h,
		gft::Color::Channel1(color));
      DrawArrow(cimg->C[2],
		A, B,
		r, w, h,
		gft::Color::Channel2(color));
    }



    void    DrawArrow(sCImage *cimg,
		      int xA, int yA,
		      int xB, int yB,
		      float r, float w, float h,
		      int color){
      DrawArrow(cimg->C[0],
		xA, yA, xB, yB,
		r, w, h,
		gft::Color::Channel0(color));
      DrawArrow(cimg->C[1],
		xA, yA, xB, yB,
		r, w, h,
		gft::Color::Channel1(color));
      DrawArrow(cimg->C[2],
		xA, yA, xB, yB,
		r, w, h,
		gft::Color::Channel2(color));
    }
    

    
    void    FloodFill(sCImage *cimg, sAdjRel *A,
		      int P, int color){
      gft::sQueue *Q;
      int old, p, q, i, color_q;
      int px,py,qx,qy;
      if(P < 0 || P >= (cimg->C[0])->n)
	return;
      old = gft::Color::Triplet( (cimg->C[0])->data[P],
				 (cimg->C[1])->data[P],
				 (cimg->C[2])->data[P] );
      if(old == color)
	return;
      Q = gft::Queue::Create((cimg->C[0])->n);
      gft::Queue::Push(Q, P);
      (cimg->C[0])->data[P] = gft::Color::Channel0(color);
      (cimg->C[1])->data[P] = gft::Color::Channel1(color);
      (cimg->C[2])->data[P] = gft::Color::Channel2(color);      
      while(!gft::Queue::IsEmpty(Q)){
	p = gft::Queue::Pop(Q);
	px = p%(cimg->C[0])->ncols;
	py = p/(cimg->C[0])->ncols;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::CImage::IsValidPixel(cimg, qx, qy)){
	    q = qx + qy*(cimg->C[0])->ncols;
	    color_q = gft::Color::Triplet( (cimg->C[0])->data[q],
					   (cimg->C[1])->data[q],
					   (cimg->C[2])->data[q] );
	    if(color_q == old){
	      gft::Queue::Push(Q, q);
	      (cimg->C[0])->data[q] = gft::Color::Channel0(color);
	      (cimg->C[1])->data[q] = gft::Color::Channel1(color);
	      (cimg->C[2])->data[q] = gft::Color::Channel2(color);
	    }
	  }
	}
      }
      gft::Queue::Destroy(&Q);
    }
    


  } /*end CImage namespace*/
} /*end gft namespace*/

