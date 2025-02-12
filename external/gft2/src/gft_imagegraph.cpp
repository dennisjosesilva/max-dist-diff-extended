
#include "gft2/gft_imagegraph.h"


#include "gft2/gft_graph.h"
#include "gft2/gft_ctree.h"


namespace gft{
  namespace ImageGraph{


    sImageGraph *ByEuclideanDistance(sImage32 *img, float r){
      sImageGraph *sg=NULL;
      sAdjRel *A=AdjRel::Circular(r);
      int p,q,i,n;
      int weight;
      int u_x,u_y,v_x,v_y;
      
      n  = img->ncols*img->nrows;
      sg = Create(img->ncols, 
		  img->nrows, A);
      sg->type = DISSIMILARITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u_x = p%img->ncols; //PixelX(img,p);
	u_y = p/img->ncols; //PixelY(img,p);
	
	(sg->n_link[p])[0] = NIL;
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(Image32::IsValidPixel(img,v_x,v_y)){
	    q = v_x + img->ncols*v_y;
	    
	    weight = abs(img->data[p]-img->data[q]) + 1;
	    (sg->n_link[p])[i] = weight;
	    if(weight > sg->Wmax)
	      sg->Wmax = weight; 
	  }
	  else
	    (sg->n_link[p])[i] = INT_MAX;
	}
      }
      return sg;
    }


    sImageGraph *ByEuclideanDistance(sCImage *cimg, float r){
      sImageGraph *sg=NULL;
      sAdjRel *A=AdjRel::Circular(r);
      int p,q,i,n;
      int weight;
      int u_x,u_y,v_x,v_y;
      int dr,dg,db;
      int ncols, nrows;

      ncols = cimg->C[0]->ncols;
      nrows = cimg->C[0]->nrows;
      n  = ncols*nrows;
      sg = Create(ncols, nrows, A);
      sg->type = DISSIMILARITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u_x = p%ncols; //PixelX(img,p);
	u_y = p/ncols; //PixelY(img,p);
	
	(sg->n_link[p])[0] = NIL;
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(Image32::IsValidPixel(cimg->C[0], v_x, v_y)){
	    q = v_x + ncols*v_y;
	    dr = cimg->C[0]->data[p] - cimg->C[0]->data[q];
	    dg = cimg->C[1]->data[p] - cimg->C[1]->data[q];
	    db = cimg->C[2]->data[p] - cimg->C[2]->data[q];
	    weight = ROUND(sqrtf(dr*dr + dg*dg + db*db)) + 1;
	    (sg->n_link[p])[i] = weight;
	    if(weight > sg->Wmax)
	      sg->Wmax = weight; 
	  }
	  else
	    (sg->n_link[p])[i] = INT_MAX;
	}
      }
      return sg;
    }



    sImageGraph *ByEuclideanDistance(sCImage32f *cimg, float r){
      sImageGraph *sg=NULL;
      sAdjRel *A=AdjRel::Circular(r);
      int p,q,i,n;
      int weight;
      int u_x,u_y,v_x,v_y;
      float dr,dg,db;
      int ncols, nrows;

      ncols = cimg->C[0]->ncols;
      nrows = cimg->C[0]->nrows;
      n  = ncols*nrows;
      sg = Create(ncols, nrows, A);
      sg->type = DISSIMILARITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u_x = p%ncols; //PixelX(img,p);
	u_y = p/ncols; //PixelY(img,p);
	
	(sg->n_link[p])[0] = NIL;
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(Image32f::IsValidPixel(cimg->C[0], v_x, v_y)){
	    q = v_x + ncols*v_y;
	    dr = cimg->C[0]->data[p] - cimg->C[0]->data[q];
	    dg = cimg->C[1]->data[p] - cimg->C[1]->data[q];
	    db = cimg->C[2]->data[p] - cimg->C[2]->data[q];
	    weight = ROUND(sqrtf(dr*dr + dg*dg + db*db)) + 1;
	    (sg->n_link[p])[i] = weight;
	    if(weight > sg->Wmax)
	      sg->Wmax = weight; 
	  }
	  else
	    (sg->n_link[p])[i] = INT_MAX;
	}
      }
      return sg;
    }
    
    

    sImageGraph *ByAccAbsDiff(sImage32 *img, float r, float R){
      sImageGraph *sg=NULL;
      sImage32 *W;
      sAdjRel *A=AdjRel::Circular(R);
      int p,q,i;
      int weight;
      int u_x,u_y,v_x,v_y;
      
      W = Image32::Create(img->ncols, img->nrows);
      for(p = 0; p < img->n; p++){
	u_x = p%img->ncols; //PixelX(img,p);
	u_y = p/img->ncols; //PixelY(img,p);
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(Image32::IsValidPixel(img,v_x,v_y)){
	    q = v_x + img->ncols*v_y;
	    
	    weight = abs(img->data[p]-img->data[q]);
	    W->data[p] += weight;
	  }
	}
      }
      sg = ByWeightImage(W, r);
      Image32::Destroy(&W);
      AdjRel::Destroy(&A);
      return sg;
    }
    

    sImageGraph *ByAccAbsDiff(sCImage *cimg, float r, float R){
      sImageGraph *sg=NULL;
      sImage32 *W;
      sAdjRel *A=AdjRel::Circular(R);
      int p,q,i;
      float weight,sum;
      int dr,dg,db;
      int u_x,u_y,v_x,v_y;
      
      W = Image32::Create(cimg->C[0]->ncols, cimg->C[0]->nrows);
      for(p = 0; p < cimg->C[0]->n; p++){
	u_x = p%cimg->C[0]->ncols; //PixelX(img,p);
	u_y = p/cimg->C[0]->ncols; //PixelY(img,p);

	sum = 0.0;
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(Image32::IsValidPixel(cimg->C[0],v_x,v_y)){
	    q = v_x + cimg->C[0]->ncols*v_y;
	    
	    dr = abs(cimg->C[0]->data[p] - cimg->C[0]->data[q]);
	    dg = abs(cimg->C[1]->data[p] - cimg->C[1]->data[q]);
	    db = abs(cimg->C[2]->data[p] - cimg->C[2]->data[q]);
	    weight = sqrtf(dr*dr + dg*dg + db*db);
	    sum += weight;
	  }
	}
	W->data[p] = ROUND(sum);
      }
      sg = ByWeightImage(W, r);
      Image32::Destroy(&W);
      AdjRel::Destroy(&A);
      return sg;
    }
    
    
    sImageGraph *ByWeightImage(sImage32 *W, float r){
      sImageGraph *sg=NULL;
      sAdjRel *A=AdjRel::Circular(r);
      int p,q,i,n;
      int weight;
      int u_x,u_y,v_x,v_y;
      
      n  = W->ncols*W->nrows;
      sg = Create(W->ncols, 
		  W->nrows, A);
      sg->type = DISSIMILARITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u_x = p%W->ncols; //PixelX(W,p);
	u_y = p/W->ncols; //PixelY(W,p);
	
	(sg->n_link[p])[0] = INT_MAX;
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(Image32::IsValidPixel(W,v_x,v_y)){
	    q = v_x + W->ncols*v_y;
	    
	    weight = W->data[p] + W->data[q] + 1;
	    (sg->n_link[p])[i] = weight;
	    
	    if(weight > sg->Wmax) sg->Wmax = weight;
	  }
	  else
	    (sg->n_link[p])[i] = INT_MAX;
	}
      }
      return sg;
    }


    sImageGraph   *ByHomogeneityAffinity(sImage32 *img, float r){
      sImageGraph *sg=NULL;
      sg = ByEuclideanDistance(img, r);
      ChangeType(sg, CAPACITY);
      return sg;
    }
    

    /*
    sImageGraph   *ByHomogeneityAffinity(sImage32 *img, float r){
      sImageGraph *sg=NULL;
      sAdjRel *A = gft::AdjRel::Circular(r);
      int p,q,i,n;
      int weight;
      Pixel u,v;
      
      n        = img->ncols*img->nrows;
      sg       = Create(img->ncols, img->nrows, A);
      sg->type = DISSIMILARITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u.x = p%img->ncols;
	u.y = p/img->ncols;
	for(i=1; i<A->n; i++){
	  v.x = u.x + A->dx[i];
	  v.y = u.y + A->dy[i];
	  if(gft::Image32::IsValidPixel(img,v.x,v.y)){
	    q = v.x + img->ncols * v.y;
	    
	    weight = abs(img->data[p]-img->data[q]) + 1;
	    (sg->n_link[p])[i] = weight;
	    
	    if(weight > sg->Wmax) sg->Wmax = weight;
	  }
	  else
	    (sg->n_link[p])[i] = INT_MAX;
	}
      }
      ChangeType(sg, CAPACITY);
      return sg;
    }
    */


    sImageGraph *ByHomogeneityAffinity(sCImage *cimg, float r){
      sImageGraph *sg=NULL;
      sg = ByEuclideanDistance(cimg, r);
      ChangeType(sg, CAPACITY);
      return sg;
    }
    

    /*
    sImageGraph *ByHomogeneityAffinity(sCImage *cimg, float r){
      sImageGraph *sg=NULL;
      sAdjRel *A = gft::AdjRel::Circular(r);
      int p,q,i,n,ncols,nrows;
      int weight,dr,dg,db;
      Pixel u,v;
      
      ncols = cimg->C[0]->ncols;
      nrows = cimg->C[0]->nrows;
      n  = ncols*nrows;
      sg = Create(ncols, nrows, A);
      sg->type = DISSIMILARITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u.x = p%ncols; //PixelX(img,p);
	u.y = p/ncols; //PixelY(img,p);
	for(i=1; i<A->n; i++){
	  v.x = u.x + A->dx[i];
	  v.y = u.y + A->dy[i];
	  if(gft::Image32::IsValidPixel(cimg->C[0],v.x,v.y)){
	    q = v.x + ncols*v.y;
	    dr = abs(cimg->C[0]->data[p] - cimg->C[0]->data[q]);
	    dg = abs(cimg->C[1]->data[p] - cimg->C[1]->data[q]);
	    db = abs(cimg->C[2]->data[p] - cimg->C[2]->data[q]);
	    weight = ROUND(sqrtf(dr*dr + dg*dg + db*db)) + 1;
	    (sg->n_link[p])[i] = weight;
	    
	    if(weight > sg->Wmax) sg->Wmax = weight;
	  }
	  else
	    (sg->n_link[p])[i] = INT_MAX;
	}
      }
      ChangeType(sg, CAPACITY);
      return sg;
    }
    */



    sImageGraph *WeightedMean(sImageGraph *G1, 
			      sImageGraph *G2,
			      float w2){
      sImageGraph *sg;  
      int i,p,q,n,ncols,nrows;
      int weight,v1,v2,worst;
      Pixel u,v;
      sAdjRel *A;
      
      if(G1->A->n != G2->A->n ||
	 G1->type != G2->type){
	gft::Error((char*)"Incompatible graphs",
		   (char*)"ImageGraph::WeightedMean");
      }
      A = gft::AdjRel::Clone(G1->A);
      ncols     = G1->ncols; 
      nrows     = G1->nrows;
      n         = ncols*nrows;
      sg        = Create(ncols, nrows, A);
      sg->type  = G1->type;
      sg->Wmax  = 0;
      if(G1->type == DISSIMILARITY)
	worst = INT_MAX;
      else
	worst = 0;
      
      for(p=0; p<n; p++){
	u.x = p%ncols;
	u.y = p/ncols;
	weight = 0;
	for(i=1; i<A->n; i++){
	  v.x = u.x + A->dx[i];
	  v.y = u.y + A->dy[i];
	  if(v.x>=0 && v.y>=0 && v.x<ncols && v.y<nrows){
	    q     = v.x + v.y*ncols;
	    
	    v1 = (G1->n_link[p])[i];
	    v2 = (G2->n_link[p])[i];

	    if(v1 == INT_MAX || v2 == INT_MAX){
	      (sg->n_link[p])[i] = INT_MAX;
	      continue;
	    }
	    else if(v1 == 0 || v2 == 0){
	      (sg->n_link[p])[i] = 0;
	      continue;
	    }
	    
	    weight = ROUND(w2*v2 +(1.0-w2)*v1);
	    
	    (sg->n_link[p])[i] = weight;
	    if(weight > sg->Wmax) sg->Wmax = weight;
	  }
	  else
	    (sg->n_link[p])[i] = worst;
	}
      }
      
      return sg;
    }
    

    sImageGraph *ByLevel(sImage32 *obj, int T, float r, int Wmax){
      sImageGraph *sg=NULL;
      sAdjRel *A = gft::AdjRel::Circular(r);
      int p,q,i,n,max,min;
      int weight;
      Pixel u,v;
      
      n  = obj->ncols*obj->nrows;
      sg = Create(obj->ncols, obj->nrows, A);
      sg->type = CAPACITY;
      sg->Wmax = 0;
      
      for(p=0; p<n; p++){
	u.x = p%obj->ncols; 
	u.y = p/obj->ncols; 
	for(i=1; i<A->n; i++){
	  v.x = u.x + A->dx[i];
	  v.y = u.y + A->dy[i];
	  if(gft::Image32::IsValidPixel(obj,v.x,v.y)){
	    q = v.x + obj->ncols * v.y;
	    
	    max = MAX(obj->data[p], obj->data[q]);
	    min = MIN(obj->data[p], obj->data[q]);
	    if(max>=T && min<T) weight = 1;
	    else   	        weight = Wmax; 
	    
	    (sg->n_link[p])[i] = weight;
	  }
	  else
	    (sg->n_link[p])[i] = 0;
	}
      }
      sg->Wmax = Wmax;
      return sg;
    }
    


    sImageGraph *ByExclusion(sImage32 *Pobj,
			     sImage32 *Pbkg,
			     float r,
			     int Pmax){
      sImageGraph *sg=NULL;
      sAdjRel *A = gft::AdjRel::Circular(r);
      int p,q,i,n,omin,bmin;
      int weight;
      Pixel u,v;
      
      n  = Pobj->n;
      sg = Create(Pobj->ncols, Pobj->nrows, A);
      sg->type = CAPACITY;
      sg->Wmax = 0;
      
      for(p = 0; p < n; p++){
	u.x = p%Pobj->ncols; 
	u.y = p/Pobj->ncols; 
	for(i = 1; i < A->n; i++){
	  v.x = u.x + A->dx[i];
	  v.y = u.y + A->dy[i];
	  if(gft::Image32::IsValidPixel(Pobj,v.x,v.y)){
	    q = v.x + Pobj->ncols * v.y;
	    
	    omin = MIN(Pobj->data[p], Pobj->data[q]);
	    bmin = MIN(Pbkg->data[p], Pbkg->data[q]);
	    weight = MAX(omin, bmin) + 1;
	    
	    (sg->n_link[p])[i] = weight;
	    //if(weight > sg->Wmax) sg->Wmax = weight;
	  }
	  else
	    (sg->n_link[p])[i] = 0;
	}
      }
      sg->Wmax = Pmax + 1;
      return sg;
    }



    sImageGraph *ByExclusion(sImage32 *Pobj,
			     float r,
			     int Pmax){
      sImageGraph *sg=NULL;
      sAdjRel *A = gft::AdjRel::Circular(r);
      int p,q,i,n,omin,bmin;
      int weight;
      Pixel u,v;
      
      n  = Pobj->n;
      sg = Create(Pobj->ncols, Pobj->nrows, A);
      sg->type = CAPACITY;
      sg->Wmax = 0;
      
      for(p = 0; p < n; p++){
	u.x = p%Pobj->ncols;
	u.y = p/Pobj->ncols;
	for(i = 1; i < A->n; i++){
	  v.x = u.x + A->dx[i];
	  v.y = u.y + A->dy[i];
	  if(gft::Image32::IsValidPixel(Pobj,v.x,v.y)){
	    q = v.x + Pobj->ncols * v.y;
	    
	    omin = MIN(Pobj->data[p], Pobj->data[q]);
	    bmin = Pmax - MAX(Pobj->data[p], Pobj->data[q]);
	    weight = MAX(omin, bmin) + 1;
	    
	    (sg->n_link[p])[i] = weight;
	    //if(weight > sg->Wmax) sg->Wmax = weight;
	  }
	  else
	    (sg->n_link[p])[i] = 0;
	}
      }
      sg->Wmax = Pmax + 1;
      return sg;
    }
    

    void LinearStretch(sImageGraph *sg, 
		       int f1, int f2, 
		       int g1, int g2){
      sAdjRel *A;
      int p,n,i,v;
      float a=1.0;
      
      n = sg->ncols*sg->nrows;
      if (f1 != f2) 
	a = (float)(g2-g1)/(float)(f2-f1);
      else
	gft::Error((char *)"Invalid input value",
		   (char *)"ImageGraph::LinearStretch");
      
      A = sg->A;
      for(p = 0; p < n; p++){
	for(i = 1; i < A->n; i++){
	  if((sg->n_link[p])[i] == INT_MAX ||
	     (sg->n_link[p])[i] == 0)
	    continue;
	  v = (sg->n_link[p])[i];
	  
	  if(v < f1)
	    (sg->n_link[p])[i] = g1;
	  else if(v > f2)
	    (sg->n_link[p])[i] = g2;
	  else
	    (sg->n_link[p])[i] = MIN(ROUND(a*(v-f1)+g1),g2);
	}
      }
      sg->Wmax = g2;
    }


    sImageGraph *SaliencyMap(char mergehistory[],
			     int ncols, int nrows){
      sAdjRel *A;
      sImageGraph *sg;
      sImage32 *label;
      sGraph *g;
      sCTree *ct;
      sCTreeNode *ct_p, *ct_q;
      FILE *fp;
      int i,j, n, p, q, lp, lq, Lmax;
      int px, py, qx, qy;
      int energy, min_energy = INT_MAX;
      fp = fopen(mergehistory, "r");
      if(fp == NULL){
	printf("Warning: file not found\n");
	return NULL;
      }
      A = gft::AdjRel::Circular(1.0);
      sg = Create(ncols, nrows, A);

      for(p = 0; p < ncols*nrows; p++)
	for(i = 1; i < A->n; i++)
	  (sg->n_link[p])[i] = INT_MAX;

      fscanf(fp, "%d", &n);
      //printf("n: %d\n", n);
      
      for(i = 0; i < n; i++){
	fscanf(fp, "%d %d %d", &p, &q, &energy);
	//printf("p: %d, q: %d, energy: %d\n",p,q,energy);
	
	j = GetArcIndex(sg, p, q);
	(sg->n_link[p])[j] = energy;
	j = GetArcIndex(sg, q, p);
	(sg->n_link[q])[j] = energy;
	if(energy < min_energy)
	  min_energy = energy;
      }

      label = LabelConnectedComponent(sg, min_energy, min_energy);
      Lmax = Image32::GetMaxVal(label);
      //printf("Lmax: %d\n",Lmax);
      //gft::Image32::Write(label, (char *)"LABEL.pgm");
      
      g = Graph::Create(Lmax+1, 10, NULL);

      rewind(fp);
      fscanf(fp, "%d", &n);
      for(i = 0; i < n; i++){
	fscanf(fp, "%d %d %d", &p, &q, &energy);
	//printf("p: %d, q: %d, energy: %d\n",p,q,energy);

	lp = label->data[p];
	lq = label->data[q];
	if(lp != lq)
	  Graph::AddEdge(g, lp, lq, energy);
      }
      fclose(fp);

      ct = CTree::EdgeBasedMinTree(g, min_energy);
     
      for(p = 0; p < ncols*nrows; p++){
	px = p%ncols;
	py = p/ncols;
	lp = label->data[p];
	for(i = 1; i < A->n; i++){
	  if((sg->n_link[p])[i] != INT_MAX)
	    continue;
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(qx >= 0 && qx < ncols &&
	     qy >= 0 && qy < nrows){
	    q = qx + qy*ncols;
	    lq = label->data[q];
	    if(lp == lq)
	      (sg->n_link[p])[i] = min_energy;
	    else{
	      j = Graph::GetArcIndex(g, lp, lq);
	      if(j != NIL)
		(sg->n_link[p])[i] = g->nodes[lp].Warcs[j];
	      else{ //min-tree:
		//common ancestor of lp and lq:
		ct_p = &(ct->nodes[lp]);
		ct_q = &(ct->nodes[lq]);
		while(ct_p != ct_q){
		  if(ct_p->level < ct_q->level)
		    ct_p = ct_p->parent;
		  else
		    ct_q = ct_q->parent;
		}
		(sg->n_link[p])[i] = ct_p->level;
	      }

	    }
	  }
	}
      }
      Image32::Destroy(&label);
      CTree::Destroy(&ct);
      Graph::Destroy(&g);
      
      return sg;
    }
    

    int GetArcIndex(sImageGraph *g,
		    int p, int q){
      sAdjRel *A;
      int ncols,dx,dy,i;
      
      A = g->A;
      ncols = g->ncols;
      dx = q%ncols - p%ncols;
      dy = q/ncols - p/ncols;
      
      for(i=1; i<A->n; i++){
	if(dx == A->dx[i] && dy == A->dy[i])
	  return i;
      }
      return -1;
    }
    

    sImageGraph   *Create(int ncols, int nrows, sAdjRel *A){
      sImageGraph *sg;
      int p,n,i;
      n = ncols*nrows;
      sg = (sImageGraph *)calloc(1, sizeof(sImageGraph));
      if(sg == NULL)
	gft::Error((char *)MSG1,(char *)"ImageGraph::Create");
      sg->A = A;
      sg->type = DISSIMILARITY;
      sg->ncols = ncols;
      sg->nrows = nrows;
      sg->n_link = (int **)calloc(n,sizeof(int *));
      if(sg->n_link == NULL)
	gft::Error((char *)MSG1,(char *)"ImageGraph::Create");
      for(p=0; p<n; p++){
	sg->n_link[p] = gft::AllocIntArray(A->n);
	for(i=0; i<A->n; i++)
	  (sg->n_link[p])[i] = INT_MAX;
      }
      sg->Wmax = 0;
      
      return sg;
    }


    void    Destroy(sImageGraph **sg){
      sImageGraph *aux;  
      int p,n;
      
      aux = *sg;
      if(aux != NULL){
	n = aux->ncols*aux->nrows;
	AdjRel::Destroy(&(aux->A));
	for(p=0; p<n; p++){
	  free(aux->n_link[p]);
	}
	free(aux->n_link);
	free(*sg);
	*sg = NULL;
      }
    }
    

    sImageGraph   *Clone(sImageGraph *sg){
      sImageGraph *clone = NULL;
      int p,n,i;
      
      n = sg->ncols*sg->nrows;
      
      clone = Create(sg->ncols, 
		     sg->nrows, 
		     AdjRel::Clone(sg->A));
      for(p=0; p<n; p++){
	for(i=1; i<(sg->A)->n; i++){
	  (clone->n_link[p])[i] = (sg->n_link[p])[i];
	}
      }
      clone->Wmax = sg->Wmax;
      clone->type = sg->type;

      return clone;
    }


    sImageGraph *ReadFromTxt(char *filename){
      sImageGraph *sg;
      sAdjRel *A = gft::AdjRel::Circular(1.0);
      int ncols,nrows,i,j,p,q,k,val,type;
      char c;
      FILE *fp;
      
      fp = fopen(filename,"r");
      if(fp == NULL)
	gft::Error((char *)"Cannot open file",
		   (char *)"ImageGraph::ReadFromTxt");
      fscanf(fp,"%d %d %d\n",&type,&ncols,&nrows);
      sg = Create(ncols, nrows, A);
      sg->type = type;
      sg->Wmax = 0;
      
      for(i=0; i<nrows*2-1; i++){
	if(i%2==0) //Horizontal edges.
	  for(j=0; j<ncols-1; j++){
	    p = j + ncols*(i/2);
	    q = p + 1;
	    fscanf(fp,"%d",&val);
	    if(val > sg->Wmax)
	      sg->Wmax = val;
	    k = GetArcIndex(sg, q, p);
	    (sg->n_link[q])[k] = val;
	    fscanf(fp," %c",&c);
	    if(c == '/'){
	      fscanf(fp,"%d",&val);
	      if(val > sg->Wmax)
		sg->Wmax = val;
	    }
	    else
	      ungetc((int)c, fp);
	    k = GetArcIndex(sg, p, q);
	    (sg->n_link[p])[k] = val;
	  }
	else       //Vertical edges.
	  for(j=0; j<ncols; j++){
	    p = j + ncols*(i-1)/2;
	    q = j + ncols*(i+1)/2;
	    fscanf(fp,"%d",&val);
	    if(val > sg->Wmax)
	      sg->Wmax = val;
	    k = GetArcIndex(sg, p, q);
	    (sg->n_link[p])[k] = val;
	    fscanf(fp," %c",&c);
	    if(c == '/'){
	      fscanf(fp,"%d",&val);
	      if(val > sg->Wmax)
		sg->Wmax = val;
	    }
	    else
	      ungetc((int)c, fp);
	    k = GetArcIndex(sg, q, p);
	    (sg->n_link[q])[k] = val;	    
	  }
      }
      
      fclose(fp);
      
      return sg;
    }


    void         Write2Txt(sImageGraph *sg, 
			   char *filename){
      int ncols,nrows,i,j,p,q,k,val,symmetric;
      FILE *fp;
      
      if(sg->A->n!=5)
	gft::Error((char *)"Current implementation only supports adjacency-4",
		   (char *)"ImageGraph::Write2Txt");
      
      fp = fopen(filename,"w");
      if(fp == NULL)
	gft::Error((char *)"Cannot open file",
		   (char *)"ImageGraph::Write2Txt");
      
      symmetric = IsUndirectedGraph(sg);

      ncols = sg->ncols;
      nrows = sg->nrows;
      fprintf(fp,"%d %d %d\n",sg->type,ncols,nrows);
      
      for(i=0; i<nrows*2-1; i++){
	if(i%2==0) //Horizontal edges.
	  for(j=0; j<ncols-1; j++){
	    p = j + ncols*(i/2);
	    q = p + 1;
	    if(!symmetric){
	      k = GetArcIndex(sg, q, p);
	      val = (sg->n_link[q])[k];
	      fprintf(fp,"%d/",val);
	    }
	    k = GetArcIndex(sg, p, q);
	    val = (sg->n_link[p])[k];
	    fprintf(fp,"%d ",val);
	  }
	else       //Vertical edges.
	  for(j=0; j<ncols; j++){
	    p = j + ncols*(i-1)/2;
	    q = j + ncols*(i+1)/2;

	    k = GetArcIndex(sg, p, q);
	    val = (sg->n_link[p])[k];
	    fprintf(fp,"%d",val);
	    if(!symmetric){
	      k = GetArcIndex(sg, q, p);
	      val = (sg->n_link[q])[k];
	      fprintf(fp,"/%d ",val);
	    }
	    else
	      fprintf(fp," ",val);
	  }
	fprintf(fp,"\n");
      }
      
      fclose(fp);
    }


    sImage32 *ArcWeightImage(sImageGraph *sg){
      int i,p,n,weight;
      sImage32 *arcw;
      sAdjRel *A;
      
      A    = sg->A;
      arcw = Image32::Create(sg->ncols,sg->nrows);
      n    = arcw->ncols*arcw->nrows;
      
      if(sg->type == CAPACITY){
	for(p=0; p<n; p++){
	  arcw->data[p] = sg->Wmax;
	  for(i=1; i<A->n; i++){
	    weight = (sg->n_link[p])[i];
	    arcw->data[p] = MIN(weight, arcw->data[p]);
	  }
	}
      }
      else{ //DISSIMILARITY
	for(p=0; p<n; p++){
	  arcw->data[p] = 0;
	  for(i=1; i<A->n; i++){
	    weight = (sg->n_link[p])[i];
	    if(weight == INT_MAX) continue;
	    arcw->data[p] = MAX(weight, arcw->data[p]);
	  }
	}
      }
      return(arcw);
    }


    sImage32 *KhalimskyGrid(sImageGraph *sg){
      sImage32 *grid;
      sAdjRel *A;
      int p_x, p_y, p;
      int q_x, q_y;
      int i, weight;
      A    = sg->A;
      if(A->n != 5) return NULL;
      grid = Image32::Create(sg->ncols*2+1, sg->nrows*2+1);
      for(p_y = 0; p_y < sg->nrows; p_y++){
	for(p_x = 0; p_x < sg->ncols; p_x++){
	  p = p_x + p_y*sg->ncols;
	  grid->array[p_y*2+1][p_x*2+1] = 0;
	  for(i=1; i<A->n; i++){
	    q_x = p_x + A->dx[i];
	    q_y = p_y + A->dy[i];
	    if(q_x >= 0 && q_x < sg->ncols &&
	       q_y >= 0 && q_y < sg->nrows)
	      weight = (sg->n_link[p])[i];
	    else
	      weight = 0;

	    if(p_y == q_y)
	      grid->array[p_y*2+1][MIN(p_x, q_x)*2+2] = weight;
	    else if(p_x == q_x)
	      grid->array[MIN(p_y, q_y)*2+2][p_x*2+1] = weight;
	  }
	}
      }

      for(p_y = 0; p_y <= sg->nrows; p_y++){
	for(p_x = 0; p_x <= sg->ncols; p_x++){
	  weight = 0;
	  for(i=1; i<A->n; i++){
	    q_x = p_x*2 + A->dx[i];
	    q_y = p_y*2 + A->dy[i];
	    if(q_x >= 0 && q_x < grid->ncols &&
	       q_y >= 0 && q_y < grid->nrows)
	      weight = MAX(weight, grid->array[q_y][q_x]);
	  }
	  grid->array[p_y*2][p_x*2] = weight;
	}
      }
      
      return grid;
    }


    sImage32 *LabelConnectedComponent(sImageGraph *sg,
				      int wmin, int wmax){
      sImage32 *label;
      sQueue *Q;
      int p, q, r, i, lb = 0;
      int rx,ry,qx,qy;
      Q = Queue::Create(sg->ncols*sg->nrows);
      label = Image32::Create(sg->ncols, sg->nrows);
      Image32::Set(label, NIL);
      for(p = 0; p < sg->ncols*sg->nrows; p++){
	if(label->data[p] != NIL)
	  continue;
	
	label->data[p] = lb;
	Queue::Push(Q, p);

	while(!Queue::IsEmpty(Q)){
	  r = Queue::Pop(Q);
	  rx = r % sg->ncols;
	  ry = r / sg->ncols;

	  for(i = 1; i < sg->A->n; i++){
	    qx = rx + sg->A->dx[i];
	    qy = ry + sg->A->dy[i];
	    if(qx >= 0 && qx < sg->ncols &&
	       qy >= 0 && qy < sg->nrows){
	      q = qx + qy*sg->ncols;
	      if((sg->n_link[r])[i] >= wmin && (sg->n_link[r])[i] <= wmax &&
		 label->data[q] == NIL){
		label->data[q] = lb;
		Queue::Push(Q, q);
	      }
	    }
	  }
	}
	lb++;
      }
      Queue::Destroy(&Q);
      return label;
    }
    

    void Orient2Digraph(sImageGraph *sg, 
			sImage32 *img, float per){
      sAdjRel *A;
      int p,q,n,i,val,max = 0;
      int u_x,u_y,v_x,v_y;
      float alpha;
      alpha = per/100.0;
      n = sg->ncols*sg->nrows;
      A = sg->A;
      
      for(p=0; p<n; p++){
	u_x = p%img->ncols;
	u_y = p/img->ncols;
	
	for(i=1; i<A->n; i++){
	  if((sg->n_link[p])[i] == INT_MAX ||
	     (sg->n_link[p])[i] == 0)
	    continue;
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(!IsValidPixel(img,v_x,v_y)) continue;
	  q = v_x + img->ncols*v_y;
	  
	  val = (sg->n_link[p])[i];
	  if(img->data[p] > img->data[q]){
	    val = MAX(ROUND(val*(1.0 + alpha)), 1);
	  }
	  else if(img->data[p] < img->data[q]){
	    val = MAX(ROUND(val*(1.0 - alpha)), 1);
	  }
	  (sg->n_link[p])[i] = val;
	  if(val > max) max = val;
	}
      }
      sg->Wmax = max;
    }
    

    void Transpose(sImageGraph *sg){
      sAdjRel *A;
      int p,q,n,i,j,val_pq,val_qp;
      int u_x,u_y,v_x,v_y;
      
      n = sg->ncols*sg->nrows;
      A = sg->A;
      
      for(p=0; p<n; p++){
	u_x = p%sg->ncols;
	u_y = p/sg->ncols;
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  
	  if(v_x < 0 || v_x >= sg->ncols ||
	     v_y < 0 || v_y >= sg->nrows)
	    continue;
	  
	  q = v_x + sg->ncols*v_y;
	  if(p > q){
	    j = GetArcIndex(sg, q, p);
	    
	    val_pq = (sg->n_link[p])[i];
	    val_qp = (sg->n_link[q])[j];
	    
	    (sg->n_link[p])[i] = val_qp;
	    (sg->n_link[q])[j] = val_pq;
	  }
	}
      }
    }
    


    void Orient2DigraphInner(sImageGraph *sg, sImage32 *P_sum){
      sAdjRel *A;
      int p,q,n,i;//val,max = 0;
      int u_x,u_y,v_x,v_y;
      int new_value;

      if(sg->type == DISSIMILARITY)
	new_value = 0;
      else
	new_value = INT_MAX;
      
      n = sg->ncols*sg->nrows;
      A = sg->A;
      
      for(p=0; p<n; p++){
	u_x = p%sg->ncols;
	u_y = p/sg->ncols;
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(!IsValidPixel(P_sum,v_x,v_y)) continue;
	  q = v_x + sg->ncols*v_y;
	  
	  if(P_sum->data[q] == p)
	    sg->n_link[p][i] = new_value;  //w(P_sum(q),q)=0
	  
	  //val = (sg->n_link[p])[i]; 
	  //if(val > max && val != INT_MAX) max = val;
	}
      }
      //sg->Wmax = max;
    }

    
    void Orient2DigraphOuter(sImageGraph *sg, sImage32 *P_sum){
      sAdjRel *A;
      int p,q,n,i;//val,max = 0;
      int u_x,u_y,v_x,v_y;
      int new_value;

      if(sg->type == DISSIMILARITY)
	new_value = 0;
      else
	new_value = INT_MAX;
      
      n = sg->ncols*sg->nrows;
      A = sg->A;

      for(p=0; p<n; p++){
	u_x = p%sg->ncols;
	u_y = p/sg->ncols;
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(!Image32::IsValidPixel(P_sum,v_x,v_y)) continue;
	  q = v_x + sg->ncols*v_y;
	  
	  if (P_sum->data[p] == q) {
	    //j = GetArcIndex(sg, q, p);
	    sg->n_link[p][i] = new_value;  // w(q,P_sum(q)) = 0
	  }
	  //val = (sg->n_link[p])[i]; 
	  //if(val > max && val != INT_MAX) max = val;
	}
      }
      //sg->Wmax = max;
    }
    
    /*
    void  SuppressZeroWeightedArcs(sImageGraph *g){
      int i,p,n,wzero = 0;
      sAdjRel *A;
      
      A = g->A;
      n = g->ncols*g->nrows;
      for(p=0; p<n && wzero==0; p++){
	for(i=1; i<A->n; i++){
	  if((g->n_link[p])[i] == 0){
	    wzero = 1;
	    break;
	  }
	}
      }
      
      if(wzero==1){
	for(p=0; p<n; p++){
	  for(i=1; i<A->n; i++){
	    if((g->n_link[p])[i] == INT_MAX)
	      continue;
	    (g->n_link[p])[i] += 1; 
	  }
	}
	g->Wmax++;
      }
    }
    */    


    void ChangeType(sImageGraph *sg, 
		    int type){
      int i,p,n;
      sAdjRel *A;
      
      if(sg->type==type)
	return;
      
      A = sg->A;
      n = sg->ncols*sg->nrows;
      for(p=0; p<n; p++){
	for(i=1; i<A->n; i++){
	  if((sg->n_link[p])[i] == INT_MAX)
	    (sg->n_link[p])[i] = 0;
	  else if((sg->n_link[p])[i] == 0)
	    (sg->n_link[p])[i] = INT_MAX;
	  else
	    (sg->n_link[p])[i] = sg->Wmax - (sg->n_link[p])[i] + 1;
	}
      }
      if(sg->type==DISSIMILARITY)
	sg->type = CAPACITY;
      else
	sg->type = DISSIMILARITY;
    }


    void ChangeWeight(sImageGraph *sg,
		      int old_val, int new_val){
      int i,p,n;
      sAdjRel *A;
      
      A = sg->A;
      n = sg->ncols*sg->nrows;
      for(p=0; p<n; p++){
	for(i=1; i<A->n; i++){
	  if((sg->n_link[p])[i] == old_val){
	    (sg->n_link[p])[i] = new_val;
	    sg->Wmax = MAX(sg->Wmax, new_val);
	  }
	}
      }
    }
    

    // Increasing transformation.
    void Pow(sImageGraph *sg, 
	     int power, int max){
      sAdjRel *A;
      int p,n,i;
      long double v,m;
      
      if(sg->Wmax<=1) return;
      if(power<=0)    return;
      
      n = sg->ncols*sg->nrows;
      A = sg->A;
      
      for(p=0; p<n; p++){
	for(i=1; i<A->n; i++){
	  if((sg->n_link[p])[i] == INT_MAX ||
	     (sg->n_link[p])[i] == 0) continue;
	  v = (double)((sg->n_link[p])[i]);
	  m = powl((v/sg->Wmax),power);
	  (sg->n_link[p])[i] = MAX(ROUND(m*max), 1);
	}
      }
      sg->Wmax = max;
    }
    

    /*
    sImageGraph   *Convert2Symmetric(sImageGraph *g,
                                     int method){
      sImageGraph *sg;
      sAdjRel *A;
      int i,k,p,q,n,valpq,valqp,val=0;
      int ncols,nrows;
      int u_x,u_y,v_x,v_y;
      
      A     = g->A;
      ncols = g->ncols;
      nrows = g->nrows;
      n = ncols*nrows;
      
      sg = Create(ncols, nrows,
		  AdjRel::Clone(A));
      sg->type = g->type;
      sg->Wmax = 0;
      for(p=0; p<n; p++){
	u_x = p%ncols;
	u_y = p/ncols;
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(v_x>=0 && v_x<ncols && 
	     v_y>=0 && v_y<nrows){
	    q = v_x + v_y*ncols;
	    k = GetArcIndex(g, q, p);
	    
	    valpq = (g->n_link[p])[i];
	    valqp = (g->n_link[q])[k];
	    switch(method){
	    case 0:
	      val = (valpq + valqp)/2;
	      break;
	    case 1:
	      val = MAX(valpq, valqp);
	      break;
	    case 2:
	      val = MIN(valpq, valqp);
	      break;
	    }
	    (sg->n_link[p])[i] = val;
	    (sg->n_link[q])[k] = val;
	    if(val > sg->Wmax)
	      sg->Wmax = val;
	  }
	}
      }
      
      return sg;
    }
    */


    void Convert2HedgehogDigraph(sImageGraph *sg,
				 sImage32 *cost,
				 float theta){
      sImage32 *gradx=NULL,*grady=NULL;
      sKernel *Kx,*Ky;
      sAdjRel *A;
      int ncols,nrows;
      int u_x,u_y,v_x,v_y;
      int i,p,q,n,i_min;
      int newval;
      float mag,pq_x,pq_y,cosine;
      float inner_product1, inner_product2, min_inner_product;
      float Dd_px,Dd_py,Dd_qx,Dd_qy;
      float *Dpq;
      bool found;
      A     = sg->A;
      ncols = sg->ncols;
      nrows = sg->nrows;
      n = ncols*nrows;

      cosine = cosf(theta*(PI/180.0) + PI/2.0);
      //--------------------
      Dpq = (float *)malloc(A->n*sizeof(float));
      for(i = 1; i < A->n; i++){
	Dpq[i] = sqrtf(SQUARE(A->dx[i]) + SQUARE(A->dy[i]));
      }
      //--------------------
      
      if(sg->type == DISSIMILARITY)
	newval = 0;
      else
	newval = INT_MAX;
      
      Ky = Kernel::Make("3,3,-1.0,-2.0,-1.0,0.0,0.0,0.0,1.0,2.0,1.0");
      Kx = Kernel::Make("3,3,-1.0,0.0,1.0,-2.0,0.0,2.0,-1.0,0.0,1.0");
      gradx = LinearFilter(cost, Kx);
      grady = LinearFilter(cost, Ky);

      for(p = 0; p < n; p++){
	u_x = p%ncols;
	u_y = p/ncols;

	Dd_px = gradx->data[p];
	Dd_py = grady->data[p];
	mag = sqrtf(SQUARE(Dd_px) + SQUARE(Dd_py));
	if(mag != 0.0){
	  Dd_px /= mag;
	  Dd_py /= mag;
	}

	found = false;
	min_inner_product = 2.0;
	for(i = 1; i < A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(v_x >= 0 && v_x < ncols && 
	     v_y >= 0 && v_y < nrows){
	    q = v_x + v_y*ncols;

	    Dd_qx = gradx->data[q];
	    Dd_qy = grady->data[q];
	    mag = sqrtf(SQUARE(Dd_qx) + SQUARE(Dd_qy));
	    if(mag != 0.0){
	      Dd_qx /= mag;
	      Dd_qy /= mag;
	    }
	    
	    pq_x = A->dx[i]/Dpq[i];
	    pq_y = A->dy[i]/Dpq[i];
	    inner_product1 = Dd_px * pq_x + Dd_py * pq_y;
	    inner_product2 = Dd_qx * pq_x + Dd_qy * pq_y;

	    if(inner_product1 < min_inner_product){
	      min_inner_product = inner_product1;
	      i_min = i;
	    }
	    
	    if(inner_product1 <= cosine or inner_product2 <= cosine){
	      (sg->n_link[p])[i] = newval;
	      found = true;
	    }
	  }
	}

	if(!found)
	  (sg->n_link[p])[i_min] = newval;
      }
      
      free(Dpq);
      Destroy(&gradx);
      Destroy(&grady);
      Kernel::Destroy(&Kx);
      Kernel::Destroy(&Ky);      
    }    
    

    /*    
    void ComputeMinMaxArcs(sImageGraph *sg,
			   int *min, int *max){
      sAdjRel *A;
      int n,p,i,w;
      n = sg->ncols*sg->nrows;
      A = sg->A;
      *max = INT_MIN;
      *min = INT_MAX;
      for(p=0; p<n; p++){
	for(i=1; i<A->n; i++){

	  //if((sg->n_link[p])[i] == INT_MAX ||
	  //   (sg->n_link[p])[i] == 0) continue;

	  w = (sg->n_link[p])[i];
	  if(w > *max)
	    *max = w;
	  if(w < *min)
	    *min = w;
	}
      }
    }
    */

    void ComputeMinMaxArcs(sImageGraph *sg,
			   int *min, int *max){
      sAdjRel *A;
      int n,p,i,w;
      int u_x,u_y,v_x,v_y;
      
      n = sg->ncols*sg->nrows;
      A = sg->A;
      *max = INT_MIN;
      *min = INT_MAX;
      
      for(p=0; p<n; p++){
	u_x = p%sg->ncols;
	u_y = p/sg->ncols;
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  
	  if(v_x < 0 || v_x >= sg->ncols ||
	     v_y < 0 || v_y >= sg->nrows)
	    continue;
	  //q = v_x + sg->ncols*v_y;
	  w = (sg->n_link[p])[i];
	  if(w > *max)
	    *max = w;
	  if(w < *min)
	    *min = w;
	}
      }
    }


    bool IsUndirectedGraph(sImageGraph *sg){
      sAdjRel *A;
      int p,q,n,i,j,val_pq,val_qp;
      int u_x,u_y,v_x,v_y;
      
      n = sg->ncols*sg->nrows;
      A = sg->A;
      
      for(p=0; p<n; p++){
	u_x = p%sg->ncols;
	u_y = p/sg->ncols;
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  
	  if(v_x < 0 || v_x >= sg->ncols ||
	     v_y < 0 || v_y >= sg->nrows)
	    continue;
	  
	  q = v_x + sg->ncols*v_y;
	  if(p > q){
	    j = GetArcIndex(sg, q, p);
	    if(j == -1)
	      return false;
	    val_pq = (sg->n_link[p])[i];
	    val_qp = (sg->n_link[q])[j];
	    
	    if(val_pq != val_qp)
	      return false;
	  }
	}
      }
      return true;
    }

    
  } /*end ImageGraph namespace*/
} /*end gft namespace*/


