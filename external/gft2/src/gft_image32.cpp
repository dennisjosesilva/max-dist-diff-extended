
#include "gft2/gft_image32.h"

namespace gft{
  namespace Image32{

    sImage32 *Create(sImage32 *img){
      sImage32 *nimg=NULL;
      nimg = Create(img->ncols, img->nrows);
      nimg->maxval = 0;
      nimg->minval = 0;
      nimg->dx = img->dx;
      nimg->dy = img->dy;
      return nimg;
    }
    
    sImage32 *Create(int ncols, int nrows){
      sImage32 *img=NULL;
      int i;
      img = (sImage32 *) calloc(1,sizeof(sImage32));
      if (img == NULL){
	gft::Error((char *)MSG1,(char *)"Image32::Create");
      }
      img->data  = gft::AllocIntArray(nrows*ncols);
      img->ncols = ncols;
      img->nrows = nrows;
      img->n = nrows*ncols;
      img->dx = img->dy = 1.0;
      img->maxval = 0;
      img->minval = 0;
      
      img->array = (int**)malloc(nrows*sizeof(int*));
      if(img->array == NULL){
	gft::Error((char *)MSG1,(char *)"Image32::Create");
      }
      for(i = 0; i < nrows; i++){
	img->array[i] = (img->data + i*ncols);
      }
      return(img);
    }
    

    void Destroy(sImage32 **img){
      sImage32 *aux;
      if(img != NULL){
	aux = *img;
	if (aux != NULL){
	  if(aux->data !=  NULL) gft::FreeIntArray(&aux->data);
	  if(aux->array != NULL) free(aux->array);
	  free(aux);
	  *img = NULL;
	}
      }
    }
    
    
    sImage32 *Clone(sImage32 *img){
      sImage32 *imgc;
      imgc = Create(img->ncols,img->nrows);
      memcpy(imgc->data,img->data,img->ncols*img->nrows*sizeof(int));
      imgc->dx = img->dx;
      imgc->dy = img->dy;
      imgc->maxval = img->maxval;
      imgc->minval = img->minval;      
      return(imgc);
    }


    sImage32 *Clone(sImage32 *img, Pixel l, Pixel h){
      int x,y,p,i;
      sImage32 *sub=NULL;

      if (l.x > h.x || l.y > h.y)
	gft::Error("Invalid coordinates","Clone");
      
      sub = Create(h.x-l.x+1, h.y-l.y+1);
      i=0;
      for (y=l.y; y<=h.y; y++){
	for (x=l.x; x<=h.x; x++){
	  if (IsValidPixel(img, x, y)){
	    p = x + y*img->ncols;
	    sub->data[i] = img->data[p];
	  }
	  i++;
	}
      }
      sub->dx = img->dx;
      sub->dy = img->dy;
      sub->maxval = img->maxval;
      sub->minval = img->minval;      
      return(sub);
    }


    void    Copy(sImage32 *img,
		 sImage32 *sub,
		 Pixel l){
      int i,j,p,q;
      q = 0;
      for (i=0; i<sub->nrows; i++){
        for (j=0; j<sub->ncols; j++){
	  if (IsValidPixel(img,l.x+j,i+l.y)){
	    p = (l.x+j) + (i+l.y)*img->ncols;
	    img->data[p] = sub->data[q];
	  }
	  q++;
        }
      }
    }


    void    Copy(sImage32 *img,
		 sImage32 *sub,
		 Pixel l,
		 int bkg){ //bkg: value to be ignored.
      int i,j,p,q;
      q = 0;
      for (i=0; i<sub->nrows; i++){
        for (j=0; j<sub->ncols; j++){
	  if (IsValidPixel(img,l.x+j,i+l.y)){
	    if(sub->data[q] != bkg){
	      p = (l.x+j) + (i+l.y)*img->ncols;
	      img->data[p] = sub->data[q];
	    }
	  }
	  q++;
        }
      }
    }
    

    void     Copy(sImage32 *dest, sImage32 *src){
      if(dest->n == src->n){
	memcpy(dest->data, src->data, sizeof(int)*dest->n);
	dest->dx = src->dx;
	dest->dy = src->dy;
      }
      dest->dx = src->dx;
      dest->dy = src->dy;
      dest->maxval = src->maxval;
      dest->minval = src->minval;      
    }

    
    sImage32 *Add( sImage32 *img1,  sImage32 *img2){
      sImage32 *sum;
      int p,n;
      sum = Clone(img1);
      n = img1->n;
      for(p = 0; p < n; p++)
	sum->data[p] += img2->data[p];
      return(sum);
    }

    
    sImage32 *Add( sImage32 *img,   int value){
      sImage32 *sum;
      int p,n;
      sum = Clone(img);
      n = img->n;
      for(p = 0; p < n; p++)
	sum->data[p] += value;
      return(sum);
    }


    sImage32 *Mult(sImage32 *img1, sImage32 *img2){
      sImage32 *mult;
      int p,n;
      mult = Clone(img1);
      n = img1->n;
      for(p = 0; p < n; p++)
	mult->data[p] *= img2->data[p];
      return(mult);
    }
   
    
    sImage32 *Read(char *filename){
      FILE *fp=NULL;
      unsigned char *value=NULL;
      unsigned short *value_16=NULL;
      char type[10];
      int  i,ncols,nrows,n,Imax;
      sImage32 *img=NULL;
      char z[256];
      
      fp = fopen(filename,"rb");
      if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",filename);
        exit(-1);
      }
      fscanf(fp,"%s\n",type);
      if ((strcmp(type,"P5")==0)){
	gft::NCFgets(z,255,fp);
        sscanf(z,"%d %d\n",&ncols,&nrows);
        n = ncols*nrows;
	gft::NCFgets(z,255,fp);
        sscanf(z,"%d\n",&Imax);
	//printf("Imax: %d\n",Imax);
	if(Imax <= 255){
	  //fgetc(fp); //Da problema com imagem gerada pelo opencv em python
	  fseek(fp, -n, SEEK_END); //funciona no caso acima

	  value = (unsigned char *)calloc(n,sizeof(unsigned char));
	  if (value != NULL){
	    fread(value,sizeof(unsigned char),n,fp);
	  }
	  else{
	    gft::Error((char *)MSG1,(char *)"Image32::Read");
	  }
	  fclose(fp);
	  img = Create(ncols,nrows);
	  for (i=0; i < n; i++)
	    img->data[i] = (int)value[i];
	  free(value);
	}
	else if(Imax <= 65535){
	  fseek(fp, -n*sizeof(unsigned short), SEEK_END); //funciona no caso acima

	  value_16 = (unsigned short *)calloc(n,sizeof(unsigned short));
	  if (value_16 != NULL){
	    fread(value_16,sizeof(unsigned short),n,fp);
	  }
	  else{
	    gft::Error((char *)MSG1,(char *)"Image32::Read");
	  }
	  fclose(fp);
	  img = Create(ncols,nrows);
	  for (i=0; i < n; i++)
	    img->data[i] = (int)value_16[i];
	  free(value_16);
	}
	else
	  return NULL;
      }
      else{
        if ((strcmp(type,"P2")==0)){
	  gft::NCFgets(z,255,fp);
	  sscanf(z,"%d %d\n",&ncols,&nrows);
	  n = ncols*nrows;
	  gft::NCFgets(z,255,fp);
	  sscanf(z,"%d\n",&Imax);
	  img = Create(ncols,nrows);
	  img->maxval = Imax;
	  for (i=0; i < n; i++)
	    fscanf(fp,"%d",&img->data[i]);
	  fclose(fp);
        }
        else{
	  fprintf(stderr,"Input image must be P2 or P5\n");
	  exit(-1);
        }
      }
      return(img);
    }
    
    
    void Write(sImage32 *img,char *filename){
      FILE *fp;
      int i, n, Imax, maxval;
      
      fp = fopen(filename,"wb");
      if (fp == NULL){
        fprintf(stderr,"Cannot open %s\n",filename);
        exit(-1);
      }
      n = img->ncols*img->nrows;
      maxval = img->maxval;
      Imax = MAX(maxval, GetMaxVal(img));
      if (Imax==INT_MAX){
	gft::Warning((char *)"Image with infinity values",(char *)"Image::Write");
	Imax = INT_MIN;
	for (i=0; i < n; i++)
	  if ((img->data[i] > Imax)&&(img->data[i]!=INT_MAX))
	    Imax = img->data[i];
        fprintf(fp,"P2\n");
        fprintf(fp,"%d %d\n",img->ncols,img->nrows);
        fprintf(fp,"%d\n",Imax+1);
      }
      else{
        fprintf(fp,"P2\n");
        fprintf(fp,"%d %d\n",img->ncols,img->nrows);
        if (Imax==0) Imax++;
        fprintf(fp,"%d\n",Imax);
      }
      
      for (i=0; i < n; i++){
	if (img->data[i]==INT_MAX)
	  fprintf(fp,"%d ",Imax+1);
        else
	  fprintf(fp,"%d ",img->data[i]);
	if (((i+1)%17) == 0)
	  fprintf(fp,"\n");
      }
      fclose(fp);
    }
    
    
    sImage32 *ConvertToNbits(sImage32 *img, int N){
      sImage32 *imgN;
      int min,max,i,n,Imax;
      
      imgN = Create(img->ncols,img->nrows);
      n    = img->ncols*img->nrows;
      Imax = (int)(pow(2,N)-1);
      
      min = INT_MAX;
      max = INT_MIN;
      for (i=0; i < n; i++){
        if ((img->data[i] != INT_MIN)&&(img->data[i] != INT_MAX)){
	  if (img->data[i] > max)
	    max = img->data[i];
	  if (img->data[i] < min)
	    min = img->data[i];
        }
      }

      if(max < img->maxval && img->maxval != INT_MAX)
	max = img->maxval;
      
      if (min != max)
        for (i=0; i < n; i++){
	  if ((img->data[i] != INT_MIN)&&(img->data[i] != INT_MAX)){
	    imgN->data[i] = (int)(((float)(Imax)*(float)(img->data[i] - min))/
				  (float)(max-min));
	  }
	  else{
	    if (img->data[i]==INT_MIN)
	      imgN->data[i] = 0;
	    else
	      imgN->data[i] = Imax;
	  }
        }
      return(imgN);
    }



    sImage32 *ConvertToNbits(sImage32 *img, int N, bool Imin){
      sImage32 *imgN;
      int min,max,i,n,Imax;

      if(Imin)
	return ConvertToNbits(img, N);
      
      imgN = Create(img->ncols,img->nrows);
      n    = img->ncols*img->nrows;
      Imax = (int)(pow(2,N)-1);
      
      min = INT_MAX;
      max = INT_MIN;
      for (i=0; i < n; i++){
        if ((img->data[i] != INT_MIN)&&(img->data[i] != INT_MAX)){
	  if (img->data[i] > max)
	    max = img->data[i];
	  if (img->data[i] < min)
	    min = img->data[i];
        }
      }

      if(max < img->maxval && img->maxval != INT_MAX)
	max = img->maxval;
      
      for (i=0; i < n; i++){
	if ((img->data[i] != INT_MIN)&&(img->data[i] != INT_MAX)){
	  imgN->data[i] = (int)(((float)(Imax)*(float)(img->data[i]))/
				(float)(max));
	}
	else{
	  if (img->data[i]==INT_MIN)
	    imgN->data[i] = 0;
	  else
	    imgN->data[i] = Imax;
	}
      }
      return(imgN);
    }

    
    sImage32 *Complement(sImage32 *img){
      sImage32 *comp;
      int p,Imax;
      Imax = GetMaxVal(img);
      comp = Create(img);
      for(p = 0; p < img->n; p++)
	comp->data[p] = Imax - img->data[p];
      return comp;
    } 

    
    int GetMinVal(sImage32 *img){
      int i,min,n;
      n = img->ncols*img->nrows;
      min = img->data[0];
      for (i=1; i < n; i++)
        if (img->data[i] < min)
	  min = img->data[i];
      img->minval = min;
      return(min);
    }


    int GetMinVal(sImage32 *img, int *p){
      int i,pmin;
      pmin = 0;
      for(i=1; i < img->n; i++)
        if(img->data[i] < img->data[pmin])
	  pmin = i;
      *p = pmin;
      img->minval = img->data[pmin];
      return(img->data[pmin]);
    }

    
    int GetMaxVal(sImage32 *img){
      int i,max,n;
      n = img->ncols*img->nrows;
      max = img->data[0];
      for (i=1; i < n; i++)
        if (img->data[i] > max)
	  max = img->data[i];
      img->maxval = MAX(max, img->maxval);
      return(max);
    }


    int GetMaxVal(sImage32 *img, int *p){
      int i,pmax;
      pmax = 0;
      for(i=1; i < img->n; i++)
        if(img->data[i] > img->data[pmax])
	  pmax = i;
      *p = pmax;
      img->maxval = MAX(img->data[pmax], img->maxval);
      return(img->data[pmax]);
    }    

    
    int GetMaxVal(sImage32 *img, int ignoredvalue){
      int p,n,Imax=INT_MIN;
      n = img->ncols*img->nrows;
      for(p=0; p<n; p++){
	if(img->data[p] != ignoredvalue && img->data[p]>Imax)
	  Imax = img->data[p];
      }
      img->maxval = MAX(Imax, img->maxval);
      return Imax;
    }

    
    int GetFreqVal(sImage32 *img, int val){
      int p, freq = 0;
      for(p=0; p<img->n; p++){
	if(img->data[p] == val)
	  freq++;
      }
      return freq;
    }
    
    
    void Set(sImage32 *img, int value){
      int i;
      if(value == 0)
	bzero(img->data, img->n*sizeof(int));
      else
	for (i=0; i < img->n; i++)
	  img->data[i] = value;
    }


    void Set(sImage32 *img, int old_value, int new_value){
      int i;
      for(i=0; i < img->n; i++){
	if(img->data[i] == old_value)
	  img->data[i] = new_value;
      }
    }
    
    
    bool IsValidPixel(sImage32 *img, int x, int y){
      return ((x >= 0)&&(x < img->ncols)&&
	      (y >= 0)&&(y < img->nrows));
    }


    bool IsValidPixel(sImage32 *img, Pixel u){
      return ((u.x >= 0)&&(u.x < img->ncols)&&
	      (u.y >= 0)&&(u.y < img->nrows));
    }

    
    sImage32 *Threshold(sImage32 *img, int L, int H){
      sImage32 *bin = Clone(img);
      int p;
      for(p = 0; p<img->ncols*img->nrows; p++){
	if(img->data[p] >= L && img->data[p] <= H){
	  bin->data[p] = 1;
	}
	else
	  bin->data[p] = 0;
      }
      return bin;
    }
    
    
    //------------------------------------
    
    sImage32 *AddFrame(sImage32 *img, int sz, int value){
      sImage32 *fimg;
      int y,*dst,*src,nbytes,offset;
      
      fimg = Create(img->ncols+(2*sz), img->nrows+(2*sz));
      Set(fimg, value);
      nbytes = sizeof(int)*img->ncols;
      offset = sz + fimg->ncols*sz;
      for (y=0,src=img->data,dst=fimg->data+offset; y < img->nrows;y++,src+=img->ncols,dst+=fimg->ncols){
	memcpy(dst, src, nbytes);
      }
      return(fimg);
    }


    sImage32 *RemFrame(sImage32 *fimg, int sz){
      sImage32 *img;
      int y,*dst,*src,nbytes,offset;
      
      img = Create(fimg->ncols-(2*sz), fimg->nrows-(2*sz));
      nbytes = sizeof(int)*img->ncols;
      offset = sz + fimg->ncols*sz;
      for (y=0,src=fimg->data+offset,dst=img->data; y < img->nrows;y++,src+=fimg->ncols,dst+=img->ncols){
	memcpy(dst, src, nbytes);
      }
      return(img);
    }

    
    sImage32 *Scale_linear(sImage32 *img, float Sx, float Sy){
      float S[2][2],x,y,d1,d2,d3,d4,Ix1,Ix2,If;
      sImage32 *scl;
      Pixel u,v,prev,next;
      
      if (Sx == 0.0) Sx = 1.0;
      if (Sy == 0.0) Sy = 1.0;
      
      S[0][0] = 1.0/Sx;
      S[0][1] = 0;
      S[1][0] = 0;
      S[1][1] = 1.0/Sy;
      
      scl = Create((int)(img->ncols*fabs(Sx) + 0.5),
		   (int)(img->nrows*fabs(Sy) + 0.5));
      
      for (v.y=0; v.y < scl->nrows; v.y++)
        for (v.x=0; v.x < scl->ncols; v.x++){
	  x = ((v.x-scl->ncols/2.)*S[0][0]) + img->ncols/2.;
	  y = ((v.y-scl->nrows/2.)*S[1][1]) + img->nrows/2.;
	  u.x = (int)(x+0.5);
	  u.y = (int)(y+0.5);
	  if (IsValidPixel(img,u.x,u.y)){
	    if (x < u.x){
	      next.x = u.x;
	      prev.x = u.x - 1;
	    }
	    else{
	      next.x = u.x + 1;
	      prev.x = u.x;
	    }
	    d1 = next.x - x;
	    d2 = x - prev.x;
	    if (y < u.y){
	      next.y = u.y;
	      prev.y = u.y - 1;
	    }
	    else{
	      next.y = u.y + 1;
	      prev.y = u.y;
	    }
	    d3 = next.y - y;
	    d4 = y - prev.y;
	    
	    if (IsValidPixel(img,prev.x,prev.y)&&
		IsValidPixel(img,next.x,prev.y))
	      Ix1 = (d1*img->array[prev.y][prev.x] +
		     d2*img->array[prev.y][next.x]);
	    else
	      Ix1 = img->array[u.y][u.x];
	    
	    if (IsValidPixel(img,prev.x,next.y)&&
		IsValidPixel(img,next.x,next.y))
	      Ix2 = (d1*img->array[next.y][prev.x] +
		     d2*img->array[next.y][next.x]);
	    else
	      Ix2 = img->array[u.y][u.x];
	    
	    If = d3*Ix1 + d4*Ix2;
	    
	    scl->array[v.y][v.x] = (int)If;
	  }
        }
      
      return(scl);
    }

    
    sImage32 *Scale_none(sImage32 *img, float Sx, float Sy){
      float S[2][2],x,y;
      sImage32 *scl;
      Pixel u,v;
      
      if (Sx == 0.0) Sx = 1.0;
      if (Sy == 0.0) Sy = 1.0;
      
      S[0][0] = 1.0/Sx;
      S[0][1] = 0;
      S[1][0] = 0;
      S[1][1] = 1.0/Sy;
      
      scl = Create((int)(img->ncols*Sx),
		   (int)(img->nrows*Sy));
      
      for (v.y=0; v.y < scl->nrows; v.y++)
        for (v.x=0; v.x < scl->ncols; v.x++){
	  x = ((v.x-scl->ncols/2.)*S[0][0]) + img->ncols/2.;
	  y = ((v.y-scl->nrows/2.)*S[1][1]) + img->nrows/2.;
	  u.x = (int)(x);
	  u.y = (int)(y);
	  if (IsValidPixel(img,u.x,u.y)){
	    scl->array[v.y][v.x] = img->array[u.y][u.x];
	  }
        }
      return(scl);
    }
    
    
    sImage32 *Scale(sImage32 *img, float Sx, float Sy,
		    InterpolationType interpolation){
      if(interpolation == none)
	return Scale_none(img, Sx, Sy);
      else if(interpolation == linear)
	return Scale_linear(img, Sx, Sy);
      else
	return NULL;
    }



    sImage32 *MBB(sImage32 *img){
      Pixel lower,higher;
      sImage32 *mbb=NULL;
      MBB(img, &lower, &higher);
      mbb = Clone(img, lower, higher);
      return(mbb);
    }

   
    void     MBB(sImage32 *img, Pixel *l, Pixel *h){
      Pixel p;
      l->x  = img->ncols-1;
      l->y  = img->nrows-1;
      h->x = 0;
      h->y = 0;
      for(p.y=0; p.y < img->nrows; p.y++)
	for(p.x=0; p.x < img->ncols; p.x++)    
	  if(img->data[p.x + p.y*img->ncols] > 0){
	    if(p.x < l->x)
	      l->x = p.x;
	    if(p.y < l->y)
	      l->y = p.y;
	    if(p.x > h->x)
	      h->x = p.x;
	    if(p.y > h->y)
	      h->y = p.y;
	  }
    }
    
    
  } /*end Image32 namespace*/
} /*end gft namespace*/

