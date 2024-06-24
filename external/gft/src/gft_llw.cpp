
#include "gft_llw.h"

namespace gft{
  namespace LLW{

    int RightSide(int p,
		  gft::sImage32 *img,
		  int dx, int dy){
      int p_x,p_y,q_x,q_y,q;
      p_x = p % img->ncols;
      p_y = p / img->ncols;
      if(dx == 1 && dy == 0){
	dx = 0;
	dy = 1;
      }
      else if(dx == -1 && dy == 0){
	dx = 0;
	dy = -1;
      }
      else if(dx == 0 && dy == 1){
	dx = -1;
	dy = 0;
      }
      else if(dx == 0 && dy == -1){
	dx = 1;
	dy = 0;
      }
      else if(dx == 1 && dy == 1){
	dx = 0;
	dy = 1;
      }
      else if(dx == -1 && dy == -1){
	dy = -1;
	dx = 0;
      }
      else if(dx == -1 && dy == 1){
	dx = -1;
	dy = 0;
      }
      else if(dx == 1 && dy == -1){
	dx = 1;
	dy = 0;
      }
      
      q_x = p_x + dx;
      q_y = p_y + dy;
      if(gft::Image32::IsValidPixel(img, q_x, q_y))
	q = q_x + q_y*img->ncols;
      else
	q = NIL;
      return q;
    }


    int LeftSide(int p,
		 gft::sImage32 *img,
		 int dx, int dy){
      int p_x,p_y,q_x,q_y,q;
      p_x = p % img->ncols;
      p_y = p / img->ncols;
      if(dx == 1 && dy == 0){
	dx = 0;
	dy = -1;
      }
      else if(dx == -1 && dy == 0){
	dx = 0;
	dy = 1;
      }
      else if(dx == 0 && dy == 1){
	dx = 1;
	dy = 0;
      }
      else if(dx == 0 && dy == -1){
	dx = -1;
	dy = 0;
      }
      else if(dx == 1 && dy == 1){
	dx = 1;
	dy = 0;
      }
      else if(dx == -1 && dy == -1){
	dy = 0;
	dx = -1;
      }
      else if(dx == -1 && dy == 1){
	dx = 0;
	dy = 1;
      }
      else if(dx == 1 && dy == -1){
	dx = 0;
	dy = -1;
      }  
      
      q_x = p_x + dx;
      q_y = p_y + dy;
      if(gft::Image32::IsValidPixel(img, q_x, q_y))
	q = q_x + q_y*img->ncols;
      else
	q = NIL;
      return q;
    }
    


    /*
      gft::sImage32 *ByAccAbsDiff(gft::sImage32 *img,
      float r){
      gft::sImage32 *W;
      gft::sAdjRel *A = gft::AdjRel::Circular(r);
      int p,q,i;
      int weight;
      int u_x,u_y,v_x,v_y;
      
      W = gft::Image32::Create(img->ncols, img->nrows);
      for(p = 0; p < img->n; p++){
      u_x = p%img->ncols;
      u_y = p/img->ncols;
      W->data[p] = 0;
      for(i=1; i<A->n; i++){
      v_x = u_x + A->dx[i];
      v_y = u_y + A->dy[i];
      if(gft::Image32::IsValidPixel(img,v_x,v_y)){
      q = v_x + img->ncols*v_y;
	
      weight = abs(img->data[p]-img->data[q]);
      W->data[p] += weight;
      }
      }
      }
      gft::AdjRel::Destroy(&A);
      return W;
      }
    */


    int *LWPath(gft::sImage32 *img,
		gft::sImage32 *W,
		gft::sAdjRel *A,
		gft::sSet *O,
		gft::sSet *D,
		int Gmax,
		double *c){
      gft::sImage32 *pred, *dest;
      gft::sSet *tmp;
      gft::sHeap64f *Q;
      double *cost = NULL;
      int *path;
      int p,q,npath,i,r,l;
      pred = gft::Image32::Create(img);
      dest = gft::Image32::Create(img);
      gft::Image32::Set(pred, NIL);
      cost = gft::AllocDoubleArray(img->ncols*img->nrows);
      Q = gft::Heap64f::Create(img->ncols*img->nrows, cost);
      
      for(p = 0; p < img->n; p++){
	if(img->data[p] < 0)
	  cost[p] = DBL_MIN;
	else
	  cost[p] = DBL_MAX;
      }
      
      tmp = O;
      while(tmp != NULL){
	p = tmp->elem;
	cost[p] = 0.0;
	gft::Heap64f::Insert_MinPolicy(Q, p);
	tmp = tmp->next;
      }
      
      tmp = D;
      while(tmp != NULL){
	p = tmp->elem;
	dest->data[p] = 1;
	tmp = tmp->next;
      }
      //--------------------------
      double *Dist;
      double edge,temp;
      int u_x,u_y,v_x,v_y;
      //gft::Heap64f::SetRemovalPolicy(Q, MINVALUE);
      
      Dist = gft::AllocDoubleArray(A->n);
      for(i = 1; i < A->n; i++)
	Dist[i] = sqrt(A->dx[i]*A->dx[i] + A->dy[i]*A->dy[i]);
      
      while(!gft::Heap64f::IsEmpty(Q)){
	gft::Heap64f::Remove_MinPolicy(Q, &p);
	if(dest->data[p] > 0) break;
	
	u_x = p%img->ncols; 
	u_y = p/img->ncols; 
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(v_x >= 0 && v_x < img->ncols &&
	     v_y >= 0 && v_y < img->nrows){
	    q = v_x + img->ncols*v_y;
	    if(Q->color[q] != BLACK && img->data[q] >= 0){
	      r = RightSide(p, img, A->dx[i], A->dy[i]);
	      l = LeftSide( p, img, A->dx[i], A->dy[i]);
	      edge = (W->data[p] + W->data[q])*0.5;
	      if(W->data[p] <= Gmax && W->data[q] <= Gmax){
		if(r != NIL && l != NIL){
		  if(img->data[r] > img->data[l])
		    edge *= 1.5;
		  else if(img->data[r] < img->data[l])
		    edge *= 0.5;
		}
	      }
	      edge = pow(edge, 5.0);
	      
	      temp = cost[p] + edge + Dist[i];
	      
	      if(temp < cost[q]){
		gft::Heap64f::Update_MinPolicy(Q, q, temp);
		pred->data[q] = p;
	      }
	    }
	  }
	}
      }
      gft::FreeDoubleArray(&Dist);    
      
      //--------------------------
      *c = cost[p];
      
      npath = 0;
      q = p;
      while(q != NIL){
	npath++;
	q = pred->data[q];
      }
      path = gft::AllocIntArray(npath+1);
      
      path[0] = npath;
      q = p;
      i = 0;
      while(q != NIL){
	i++;
	path[i] = q;
	q = pred->data[q];
      }
      
      gft::FreeDoubleArray(&cost);
      gft::Image32::Destroy(&pred);
      gft::Image32::Destroy(&dest);
      gft::Heap64f::Destroy(&Q);
      
      return path;
    }



    void DrawPaths(gft::sCImage *cimg,
		   gft::sImage32 *roi_img,
		   int p, int size,
		   int *path1,
		   int *path2,
		   gft::sSet *B){
      gft::sSet *tmp;
      int i,q,q_x,q_y,p_x,p_y,t,t_x,t_y;
      p_x = p % cimg->C[0]->ncols;
      p_y = p / cimg->C[0]->ncols;
      
      tmp = B;
      while(tmp != NULL){
	q = tmp->elem;
	q_x = q % size;
	q_y = q / size;
	t_x = p_x + q_x - size/2;
	t_y = p_y + q_y - size/2;
	t = t_x + t_y*cimg->C[0]->ncols;
	cimg->C[0]->data[t] = 0;
	cimg->C[1]->data[t] = 0;
	cimg->C[2]->data[t] = 255;
	tmp = tmp->next;
      }
      for(i = 1; i <= path1[0]; i++){
	q = path1[i];
	q_x = q % size;
	q_y = q / size;
	t_x = p_x + q_x - size/2;
	t_y = p_y + q_y - size/2;
	t = t_x + t_y*cimg->C[0]->ncols;
	cimg->C[0]->data[t] = 255;
	cimg->C[1]->data[t] = 0;
	cimg->C[2]->data[t] = 0;
      }
      for(i = 1; i <= path2[0]; i++){
	q = path2[i];
	q_x = q % size;
	q_y = q / size;
	t_x = p_x + q_x - size/2;
	t_y = p_y + q_y - size/2;
	t = t_x + t_y*cimg->C[0]->ncols;
	cimg->C[0]->data[t] = 0;
	cimg->C[1]->data[t] = 255;
	cimg->C[2]->data[t] = 0;
      }
      /*
	for(q = 0; q < roi_img->n; q++){
	if(roi_img->data[q] >= 0){
	q_x = q % size;
	q_y = q / size;
	t_x = p_x + q_x - size/2;
	t_y = p_y + q_y - size/2;
	t = t_x + t_y*cimg->C[0]->ncols;
	cimg->C[0]->data[t] = roi_img->data[q];
	cimg->C[1]->data[t] = roi_img->data[q];
	cimg->C[2]->data[t] = roi_img->data[q];
	}
	}
      */
    }
    
    /*
      double ComputeMeanW(gft::sImage32 *W,
      int *path1, int *path2,
      double power,
      int d){
      double sum = 0.0;
      int i, j, p;
      j = 0;
      for(i = 2; i <= path1[0]; i++){
      j++;
      if(j > d) break;
      p = path1[i];
      sum += pow(W->data[p], power);
      }
      j = 0;
      for(i = path2[0]; i >= 1; i--){
      j++;
      if(j > d + 1) break;
      p = path2[i];
      sum += pow(W->data[p], power);
      }
      return pow(sum/(2.0*d + 1.0), 1.0/power);  //(path1[0] + path2[0] - 1.0);
      }
    */


    double ComputeCurvature(int ncols,
			    int *path1, int *path2,
			    double *curv_x,
			    double *curv_y){
      int n,i;
      int a,b,c;
      int a_x,a_y,b_x,b_y,c_x,c_y;
      double d_x, d_y, d, curv;
      double dab_x, dab_y, dab;
      double dbc_x, dbc_y, dbc;
      double sum_curv_x, sum_curv_y;
      //path1[1]; == path2[path2[0]];
      n = MIN(path1[0], path2[0]);
      b = path1[1];
      b_x = b % ncols;
      b_y = b / ncols;
      curv = 0.0;
      sum_curv_x = 0.0;
      sum_curv_y = 0.0;
      for(i = n/2+1; i <= n; i++){ //i = 2;
	a = path1[i];
	a_x = a % ncols;
	a_y = a / ncols;
	c = path2[path2[0]-i+1];
	c_x = c % ncols;
	c_y = c / ncols;
	d_x = a_x - 2.*b_x + c_x;
	d_y = a_y - 2.*b_y + c_y;
	d = sqrtf(d_x*d_x + d_y*d_y);
	dab_x = a_x - b_x;
	dab_y = a_y - b_y;
	dab = sqrtf(dab_x*dab_x + dab_y*dab_y);
	dbc_x = c_x - b_x;
	dbc_y = c_y - b_y;
	dbc = sqrtf(dbc_x*dbc_x + dbc_y*dbc_y);
	curv += d/(dab + dbc);
	sum_curv_x += d_x/(dab + dbc);
	sum_curv_y += d_y/(dab + dbc);
      }
      curv /= (n-(n/2+1)+1);
      if(curv_x != NULL)
	*curv_x = sum_curv_x/(n-(n/2+1)+1);
      if(curv_y != NULL)
	*curv_y = sum_curv_y/(n-(n/2+1)+1);
      return curv;
    }
    
    
    double *Curvature(gft::sImage32 *img,
		      gft::sImage32 *W,
		      gft::sImage32 *mask,
		      float r,
		      bool draw,
		      double **V_curv_x,
		      double **V_curv_y){
      gft::sAdjRel *A = gft::AdjRel::Circular(1.5);
      gft::sCImage *cimg = NULL;
      gft::sImage32 *roi_img, *roi_W, *roi_W2, *W_mask;
      int p_x,p_y,q_x,q_y,t_x,t_y;
      int p,size,dx,dy,q,t,i,j,Gmax,draw_point = 0;
      double cost1, cost2, curv;
      int *path1, *path2;
      gft::sSet *B=NULL, *C=NULL;
      double *dw = NULL;
      double curv_x, curv_y;
      /*
      int Px[] = {118, 96, 167, 139, 187,  51,  56,  98, 106, 42, 187, 120, 85, 161, 142, 198,  67};
      int Py[] = { 84, 78, 107, 136,  87, 105, 155, 144, 120, 79, 147, 156, 99, 138,  92, 110, 133};
      int nP = 17;
      */
      /*
      int  Px[] = {55, 130, 111, 85, 52, 15, 34, 34,  88, 126, 159, 154,  76, 104, 160,  35, 130};
      int  Py[] = {74,  81,  50, 29, 19, 64, 90, 48, 147, 127,  97,  74, 117, 119,  45, 136,  22};
      char Ty[] = {'o','o', 'o','V','V','V','V', '|','|', 'V', '|', 'L', 'o', 'o', 'V', 'V', 'L'};
      int nP = 18;
      */
      /* 
      int  Px[] = {454, 391, 355, 537, 227, 230, 258, 347, 455, 459, 160, 507};
      int  Py[] = {624, 579, 599, 415, 273, 329, 434, 133,  71, 115, 520,  92};
      char Ty[] = {'V', '|', '|', ' ', 'o', ' ', 'o', ' ', '|', '|', '|', '|'};
      int nP = 12;
      */
      /*
      int  Px[] = {127, 168, 140, 230};
      int  Py[] = {362, 330, 437, 438};
      char Ty[] = {'|', '|', '|', '|'};
      int nP = 4;
      */
      
      dw = gft::AllocDoubleArray(img->ncols*img->nrows);
      if(V_curv_x != NULL)
	*V_curv_x = gft::AllocDoubleArray(img->ncols*img->nrows);
      if(V_curv_y != NULL)
	*V_curv_y = gft::AllocDoubleArray(img->ncols*img->nrows);

      size = 2*ROUND(r+1)+1;
      //printf("size: %d\n", size);

      if(draw)
	cimg = gft::CImage::Clone(img);
      roi_img = gft::Image32::Create(size, size);
      roi_W   = gft::Image32::Create(size, size);
      roi_W2  = gft::Image32::Create(size, size);
      gft::Set::Insert(&C, ROUND(r+1) + ROUND(r+1)*roi_img->ncols);

      Gmax = gft::Image32::GetMaxVal(W);
      //printf("Gmax: %d\n", Gmax);

      if(mask != NULL){
	W_mask = gft::Image32::Clone(W);
	for(p = 0; p < img->n; p++){
	  if(mask->data[p] == 0)
	    W_mask->data[p] += 2*Gmax;
	}
	W = W_mask;
      }
      
      for(p = 0; p < img->n; p++){
	if(mask != NULL && mask->data[p] == 0)
	  continue;

	p_x = p % img->ncols;
	p_y = p / img->ncols;

	/*
	draw_point = 0;
	for(j = 0; j < nP; j++){
	  if(Px[j] == p_x && Py[j] == p_y){
	    draw_point = 1;
	    printf("%c: ",Ty[j]);
	  }
	}
	*/
	
	if(p_x % 30 == 0 && p_y % 30 == 0)
	  draw_point = 1;
	else
	  draw_point = 0;
	
	//----- Copying subimages -------------
	t = 0;
	for(dy = -ROUND(r+1); dy <= ROUND(r+1); dy++){
	  for(dx = -ROUND(r+1); dx <= ROUND(r+1); dx++){
	    q_x = p_x + dx;
	    q_y = p_y + dy;
	    if(gft::Image32::IsValidPixel(img, q_x, q_y)){
	      q = q_x + q_y*img->ncols;
	      if(dx*dx+dy*dy <= r*r){
		roi_img->data[t] = img->data[q];
		roi_W->data[t] = W->data[q];
	      }
	      else{
		roi_img->data[t] = -1;
		roi_W->data[t] = -1;
	      }
	    }
	    else{
	      roi_img->data[t] = -2;
	      roi_W->data[t] = -2;
	    }
	    t++;
	  }
	}
	memcpy(roi_W2->data, roi_W->data, size*size*sizeof(int));
	//-------initializing seeds----------------------
	
	for(t = 0; t < roi_img->n; t++){
	  t_x = t % roi_img->ncols;
	  t_y = t / roi_img->ncols;
	  if(roi_img->data[t] < 0) continue;
	  for(i=1; i<A->n; i++){
	    q_x = t_x + A->dx[i];
	    q_y = t_y + A->dy[i];
	    if(gft::Image32::IsValidPixel(roi_img,q_x,q_y)){
	      q = q_x + q_y*roi_img->ncols;
	      if(roi_img->data[q] == -1){
		gft::Set::Insert(&B, t);
		break;
	      }
	    }
	  }
	}
	
	//-------------------------------------
	path1 = LWPath(roi_img, roi_W, A, B, C, Gmax, &cost1);
	//for(i = 2; i <= path1[0]; i++)
	//  roi_W2->data[path1[i]] = 5*Gmax;
	for(i = path1[0]; i > 2; i--)
	  if(abs(path1[i]%roi_img->ncols - roi_img->ncols/2) > 1.0 ||
	     abs(path1[i]/roi_img->ncols - roi_img->nrows/2) > 1.0)
	    gft::Image32::DrawAdjRel(roi_W2, A, path1[i], 2*Gmax);
	path2 = LWPath(roi_img, roi_W2, A, C, B, Gmax, &cost2);
	
	curv = ComputeCurvature(roi_img->ncols, path1, path2, &curv_x, &curv_y);
	
	//dw[p] = (cost1 + cost2)/(path1[0]+path2[0]-2);
	//dw[p] = pow(dw[p], 1.0/2.0);
	//dw[p] = dw[p]*(1. + curv*curv);
	
	//dw[p] = (double)W->data[p];
	//dw[p] = dw[p] + Gmax*curv*curv; //dw[p] = dw[p]*(1. + curv*curv);
	dw[p] = curv;

	if(V_curv_x != NULL)
	  (*V_curv_x)[p] = curv_x;
	if(V_curv_y != NULL)
	  (*V_curv_y)[p] = curv_y;
	
	if(draw_point && draw){
	  printf("curv: %f\n", curv);
	  DrawPaths(cimg, roi_img, p, size, path1, path2, B);
	}
	  
	gft::FreeIntArray(&path1);
	gft::FreeIntArray(&path2);
	
	//-------------------------------------    
	gft::Set::Destroy(&B);
	B = NULL;
      }
      
      if(draw){
	gft::CImage::Write(cimg, (char *)"paths.ppm");
	gft::CImage::Destroy(&cimg);
      }
      if(mask != NULL)
	gft::Image32::Destroy(&W_mask);
      gft::Set::Destroy(&C);
      gft::Image32::Destroy(&roi_img);
      gft::Image32::Destroy(&roi_W2);
      gft::Image32::Destroy(&roi_W);
      gft::AdjRel::Destroy(&A);
      return dw;
    }



    gft::sImage32 *WeightImage(gft::sImage32 *img,
			       float r){
      gft::sImage32 *LWW, *W, *tmp;
      double dwmax, curv;
      double *dw;
      int p,Gmax;
      //W = ByAccAbsDiff(img, 1.5);
      W = gft::Image32::SobelFilter(img);
      tmp = gft::Image32::Complement(W);
      gft::Image32::Destroy(&W);
      W = tmp;

      Gmax = gft::Image32::GetMaxVal(W);
      dw = Curvature(img, W, NULL, r, false, NULL, NULL);

      for(p = 0; p < img->n; p++){      
      	curv = dw[p];
	dw[p] = (double)W->data[p] + Gmax*curv*curv;
      }

      dwmax = 0.0;
      for(p = 0; p < img->n; p++){
	if(dw[p] > dwmax)
	  dwmax = dw[p];
      }
      //printf("dwmax: %f\n", dwmax);
      
      LWW = gft::Image32::Create(img);
      for(p = 0; p < img->n; p++){
	LWW->data[p] = 2000 - ROUND((dw[p]/dwmax)*2000);
      }
      
      //gft::Image32::Write(LWW, (char *)"lwarcw.pgm");
      
      gft::FreeDoubleArray(&dw);
      gft::Image32::Destroy(&W);
      return LWW;
    }


    //----------------------------------

    double *Curvature(gft::sImage32 *img,
		      gft::sImage32 *spixels,
		      float r,
		      double **V_curv_x,
		      double **V_curv_y){
      gft::sImage32 *LWW, *W, *tmp, *bin;
      gft::sAdjRel *A;
      double dwmax, curv;
      double *dw;
      int p,Gmax;
      W = gft::Image32::SobelFilter(img);
      tmp = gft::Image32::Complement(W);
      gft::Image32::Destroy(&W);
      W = tmp;

      A = gft::AdjRel::Neighborhood_4();
      bin = gft::Image32::GetBorders(spixels, A);
      dw = Curvature(img, W, bin, r, false, V_curv_x, V_curv_y);
      gft::Image32::Destroy(&bin);
      gft::Image32::Destroy(&W);
      gft::AdjRel::Destroy(&A);
      return dw;
    }

    
    
  } /*end LLW namespace*/
} /*end gft namespace*/

