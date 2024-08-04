#include "gft.h"

gft::sImage32 *EDT(gft::sImage32 *bin, gft::sAdjRel *A, char side){
  //gft::sImage32 *Dx=NULL,*Dy=NULL;
  gft::sImage32 *cost,*cont,*root;
  gft::sPQueue32 *Q=NULL;
  int i,p,q,r,n;
  gft::Pixel u,v,t;
  int tmp=INT_MAX,dx,dy;
  gft::sAdjRel *A4 = gft::AdjRel::Circular(1.0);
  
  cost = gft::Image32::Create(bin->ncols, bin->nrows);
  root = gft::Image32::Create(bin->ncols, bin->nrows);
  cont = gft::Image32::GetObjBorders(bin, A4); 
  //Dx = gft::Image32::Create(cost->ncols, cost->nrows);
  //Dy = gft::Image32::Create(cost->ncols, cost->nrows);
  
  n  = cost->n;
  int nb = SQUARE(MIN(bin->ncols, bin->nrows)/2.0+1); //2*(bin->ncols+bin->nrows)
  Q = gft::PQueue32::Create(nb, n, cost->data);
  
  switch (side) {
  case INTERIOR:
    for(p = 0; p < n; p++){
      root->data[p] = p;
      if (bin->data[p] != 0){
	if (cont->data[p] > 0){
	  cost->data[p] = 0;
	  gft::PQueue32::FastInsertElem(Q, p);
	  //gft::PQueue32::InsertElem(&Q, p);
	} else
	  cost->data[p] = INT_MAX;	  
      }else{
	cost->data[p] = 0;
      }
    }
    break;
  case EXTERIOR:
    for(p = 0; p < n; p++){
      root->data[p] = p;
      if (bin->data[p] == 0){
	cost->data[p] = INT_MAX;	  
      }else{
	if (cont->data[p]>0){
	  cost->data[p]=0;
	  gft::PQueue32::FastInsertElem(Q, p);
	  //gft::PQueue32::InsertElem(&Q, p);
	}else
	  cost->data[p] = 0;
      }
    }
    break;
      case BOTH:
  default:    
    for(p = 0; p < n; p++){
      root->data[p] = p;
      if (cont->data[p] > 0){
	cost->data[p]=0;
	gft::PQueue32::FastInsertElem(Q, p);
	//gft::PQueue32::InsertElem(&Q, p);
      }else{ 
	cost->data[p]=INT_MAX;    
      }
    }
  }
  gft::Image32::Destroy(&cont);
  
  while(!gft::PQueue32::IsEmpty(Q)) {
    //p = gft::PQueue32::RemoveMinFIFO(Q);
    p = gft::PQueue32::FastRemoveMinFIFO(Q);
    u.x = p % cost->ncols;
    u.y = p / cost->ncols;

    r = root->data[p];
    t.x = r % cost->ncols;
    t.y = r / cost->ncols;
    
    for (i=1; i < A->n; i++){
      v.x = u.x + A->dx[i];
      v.y = u.y + A->dy[i];
      
      if(v.x >= 0 && v.x < cost->ncols &&
	 v.y >= 0 && v.y < cost->nrows){
	q = v.x + v.y * cost->ncols;
      
	if (cost->data[p] < cost->data[q]){
	  dx  = v.x - t.x; //Dx->data[p] + abs(A->dx[i]);
	  dy  = v.y - t.y; //Dy->data[p] + abs(A->dy[i]);
	  tmp = SQUARE(dx) + SQUARE(dy);
	  if (tmp < cost->data[q]){
	    if (cost->data[q] == INT_MAX){
	      cost->data[q]  = tmp;
	      gft::PQueue32::FastInsertElem(Q, q);
	      //gft::PQueue32::InsertElem(&Q, q);
	    }
	    else
	      gft::PQueue32::FastUpdateElem(Q, q, tmp);
	      //gft::PQueue32::UpdateElem(&Q, q, tmp);
	    root->data[q] = root->data[p];
	    //Dx->data[q] = dx;
	    //Dy->data[q] = dy;
	  }
	}
      }
    }
  }
  gft::PQueue32::Destroy(&Q);
  gft::AdjRel::Destroy(&A4);
  gft::Image32::Destroy(&root);
  //gft::Image32::Destroy(&Dx);
  //gft::Image32::Destroy(&Dy);
  
  return(cost);
}

