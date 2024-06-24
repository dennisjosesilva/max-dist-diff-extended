
#include "gft_band.h"

namespace gft{
  namespace Band{

    /*
    void AddRect(gft::sImage32 *img,
		 int w, int h){
      int i,j,ii,jj;
      ii = img->nrows/2 - h/2;
      for(i = 0; i < h; i++){
	jj = img->ncols/2 - w/2;
	for(j = 0; j < w; j++){
	  if(gft::Image32::IsValidPixel(img, jj, ii))
	    img->array[ii][jj] += 1;
	  jj++;
	}
	ii++;
      }
    }
    */
    
    /*
    void AddCircle(gft::sImage32 *img,
		   float r){
      int i,j,ii,jj,ic,jc,dj,di;
      ic = img->nrows/2;
      jc = img->ncols/2;
      ii = ic - r;
      for(i = 0; i <= 2*r; i++){
	jj = jc - r;
	for(j = 0; j <= 2*r; j++){
	  dj = jj - jc;
	  di = ii - ic;
	  if(dj*dj + di*di <= r*r)
	    if(gft::Image32::IsValidPixel(img, jj, ii))
	      img->array[ii][jj] += 1;
	  jj++;
	}
	ii++;
      }
    }
    */


    /*
    void AddEllipse(gft::sImage32 *img,
		    float a, float b){
      int i,j,ii,jj,ic,jc,dj,di;
      ic = img->nrows/2;
      jc = img->ncols/2;
      ii = ic - b;
      for(i = 0; i <= 2*b; i++){
	jj = jc - a;
	for(j = 0; j <= 2*a; j++){
	  dj = jj - jc;
	  di = ii - ic;
	  if((dj*dj)/(a*a) + (di*di)/(b*b) <= 1.0)
	    if(gft::Image32::IsValidPixel(img, jj, ii))
	      img->array[ii][jj] += 1;
	  jj++;
	}
	ii++;
      }
    }    
    */
    
    
    gft::sImage32 *CircularTemplate(int size){
      /*
      gft::sImage32 *img,*cimg;
      float r;
      img = gft::Image32::Create(size, size);
      r = 0.0;
      while(r < size){
	AddCircle(img, r);
	r += 1.0;
      }  
      cimg = gft::Image32::Complement(img);
      gft::Image32::Destroy(&img);
      return cimg;
      */
      gft::sImage32 *img;
      int x,y,cx,cy,p;
      float dx,dy,d;
      img = gft::Image32::Create(size, size);
      cx = size/2; cy = size/2;
     
      for(y = 0; y < size; y++){
	for(x = 0; x < size; x++){
	  dx = x - cx;
	  dy = y - cy;
	  d = sqrtf(dx*dx + dy*dy);
	  p = x + y*size;
	  img->data[p] = ROUND(d);
	}
      }
      return img;      
    }

    

    gft::sImage32 *RectangularTemplate(int size, float ratio){
      /*
      gft::sImage32 *img,*cimg;
      float w, h;
      img = gft::Image32::Create(size, size);
      if(ratio >= 1.0){
	w = ratio;
	h = 1.0;
	while(h < size){
	  w += 2.0;
	  h = w/ratio;
	  AddRect(img, ROUND(w), ROUND(h));
	}
      }
      else{
	h = 1.0/ratio;
	w = 1.0;
	while(w < size){
	  h += 2.0;
	  w = h*ratio;
	  AddRect(img, ROUND(w), ROUND(h));
	}
      }
      cimg = gft::Image32::Complement(img);
      gft::Image32::Destroy(&img);      
      return cimg;
      */
      gft::sImage32 *img;
      int x,y,cx,cy,p;
      float dx,dy,s;
      img = gft::Image32::Create(size, size);
      cx = size/2; cy = size/2;      
      if(ratio >= 1.0){
	for(y = 0; y < size; y++){
	  for(x = 0; x < size; x++){
	    dx = fabsf(x - cx);
	    dy = fabsf(y - cy);
	    if(dx/dy >= ratio)
	      s = dx;
	    else
	      s = dy*ratio;
	    p = x + y*size;
	    img->data[p] = ROUND(s);
	  }
	}
      }
      else{ //ratio < 1.0
	for(y = 0; y < size; y++){
	  for(x = 0; x < size; x++){
	    dx = fabsf(x - cx);
	    dy = fabsf(y - cy);
	    if(dx/dy >= ratio)
	      s = dx/ratio;
	    else
	      s = dy;
	    p = x + y*size;
	    img->data[p] = ROUND(s);
	  }
	}
      }
      return img;
    }

    

    gft::sImage32 *EllipticalTemplate(int size, float ratio){
      /*
      gft::sImage32 *img,*cimg;
      float a,b;
      img = gft::Image32::Create(size, size);
      if(ratio >= 1.0){
	a = ratio;
	b = 1.0;
	while(b < size){
	  a += 1.0;
	  b = a/ratio;
	  AddEllipse(img, a, b);
	}
      }
      else{
	b = 1.0/ratio;
	a = 1.0;
	while(a < size){
	  b += 1.0;
	  a = b*ratio;
	  AddEllipse(img, a, b);
	}
      }
      cimg = gft::Image32::Complement(img);
      gft::Image32::Destroy(&img);
      return cimg;
      */
      gft::sImage32 *img;
      int x,y,cx,cy,p;
      float dx,dy,d,r,phi,s,fa,fb;
      img = gft::Image32::Create(size, size);
      cx = size/2; cy = size/2;
      if(ratio >= 1.0){
	for(y = 0; y < size; y++){
	  for(x = 0; x < size; x++){
	    dx = x - cx;
	    dy = y - cy;
	    phi = atan2f(dy, dx);
	    if(phi < 0.0) phi += 2*PI;
	    fa = cosf(phi)/(1.0*ratio);
	    fb = sinf(phi)/1.0;
	    r = powf(fa*fa+fb*fb, -1.0/2.0);
	    d = sqrtf(dx*dx + dy*dy);
	    s = d/r;
	    p = x + y*size;
	    img->data[p] = ROUND(s*ratio);
	  }
	}
      }
      else{ //ratio < 1.0
	for(y = 0; y < size; y++){
	  for(x = 0; x < size; x++){
	    dx = x - cx;
	    dy = y - cy;
	    phi = atan2f(dy, dx);
	    if(phi < 0.0) phi += 2*PI;
	    fa = cosf(phi)/1.0;
	    fb = sinf(phi)/(1.0/ratio);
	    r = powf(fa*fa+fb*fb, -1.0/2.0);
	    d = sqrtf(dx*dx + dy*dy);
	    s = d/r;
	    p = x + y*size;
	    img->data[p] = ROUND(s/ratio);
	  }
	}
      }
      return img;
    }
    

    float GielisEquation(float phi,
			 float A,
			 float B,
			 float n1,
			 float n2,
			 float n3,
			 int m){
      float sum, r;
      sum  = powf(fabsf(cosf((m/4.0)*phi)/A), n2);
      sum += powf(fabsf(sinf((m/4.0)*phi)/B), n3);
      r = powf(sum, -1.0/n1);
      return r;
    }

    
    gft::sImage32 *GielisEquationTemplate(int size,
					  float A,
					  float B,
					  float n1,
					  float n2,
					  float n3,
					  int m){
      gft::sImage32 *img;
      int x,y,cx,cy,p,stepx;
      float dx,dy,d,dmin,r,phi,rmax,s,smin,rx_1,rx_2,ry_1,ry_2;
      img = gft::Image32::Create(size, size);
      cx = size/2; cy = size/2;
      /*
      dx = 0 - cx;
      dy = 0;
      phi = atan2f(dy, dx);
      if(phi < 0.0) phi += 2*PI;
      rx_1 = GielisEquation(phi,A,B,n1,n2,n3,m);

      dx = size-1 - cx;
      dy = 0;
      phi = atan2f(dy, dx);
      if(phi < 0.0) phi += 2*PI;
      rx_2 = GielisEquation(phi,A,B,n1,n2,n3,m);

      dx = 0;
      dy = 0 - cy;
      phi = atan2f(dy, dx);
      if(phi < 0.0) phi += 2*PI;
      ry_1 = GielisEquation(phi,A,B,n1,n2,n3,m);

      dx = 0;
      dy = size-1 - cy;
      phi = atan2f(dy, dx);
      if(phi < 0.0) phi += 2*PI;
      ry_2 = GielisEquation(phi,A,B,n1,n2,n3,m);      

      cx = ROUND( (0*rx_2 + (size-1)*rx_1)/(rx_1 + rx_2) );
      cy = ROUND( (0*ry_2 + (size-1)*ry_1)/(ry_1 + ry_2) );
      */
      smin = FLT_MAX;
      dmin = FLT_MAX;
      for(y = 0; y < size; y++){
	if(y == 0 || y == size-1)
	  stepx = 1;
	else
	  stepx = size-1;
	for(x = 0; x < size; x += stepx){
	  dx = x - cx;
	  dy = y - cy;
	  phi = atan2f(dy, dx);
	  if(phi < 0.0) phi += 2*PI;
	  r = GielisEquation(phi,A,B,n1,n2,n3,m);
	  d = sqrtf(dx*dx + dy*dy);
	  s = d/r;
	  if(s < smin){
	    smin = s;
	    dmin = d;
	  }
	  else if(s == smin && d < dmin)
	    dmin = d;
	}
      }

      for(y = 0; y < size; y++){
	for(x = 0; x < size; x++){
	  dx = x - cx;
	  dy = y - cy;
	  phi = atan2f(dy, dx);
	  if(phi < 0.0) phi += 2*PI;
	  r = GielisEquation(phi,A,B,n1,n2,n3,m);
	  d = sqrtf(dx*dx + dy*dy);
	  s = d/r;
	  p = x + y*size;
	  img->data[p] = ROUND((s/smin)*dmin);
	}
      }
      return img;
    }

    
    
  } /*end Band namespace*/
} /*end gft namespace*/

