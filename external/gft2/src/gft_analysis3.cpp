
#include "gft2/gft_analysis3.h"

namespace gft{
  namespace Scene8{

    int ComputeBinCoG(sScene8 *bin){
      int p,n = bin->n;
      float sx = 0.0;
      float sy = 0.0;
      float sz = 0.0;
      int sn = 0;
      for(p=0; p<n; p++){
	if(bin->data[p] > 0){
	  sx += gft::Scene8::GetAddressX(bin, p);
	  sy += gft::Scene8::GetAddressY(bin, p);
	  sz += gft::Scene8::GetAddressZ(bin, p);
	  sn++;
	}
      }
      if(sn == 0)
	return 0;
      p = gft::Scene8::GetVoxelAddress(bin,
				       ROUND(sx/sn),
				       ROUND(sy/sn),
				       ROUND(sz/sn));
      return p;
    }
    

    sScene8 *GetObjBorders(sScene8 *scn, sAdjRel3 *A){
      sScene8 *hscn=NULL;
      int p,q,i;
      Voxel u,v;
      hscn = gft::Scene8::Create(scn);
      for (u.c.z=0; u.c.z < hscn->zsize; u.c.z++){
	for (u.c.y=0; u.c.y < hscn->ysize; u.c.y++){
	  for (u.c.x=0; u.c.x < hscn->xsize; u.c.x++){
	    p = gft::Scene8::GetVoxelAddress(scn, u);
	    if (scn->data[p] != 0) {
	      for (i=1; i < A->n; i++){
		v.v = u.v + A->d[i].v;
		if (gft::Scene8::IsValidVoxel(hscn, v)){
		  q = gft::Scene8::GetVoxelAddress(hscn, v);
		  if (scn->data[p] != scn->data[q]){
		    hscn->data[p] = scn->data[p];
		    break;
		  }
		} else {
		  hscn->data[p] = scn->data[p];
		  break;
		}
	      }
	    }
	  }
	}
      }
      return(hscn);
    }



    sScene8 *GetObjBorders(sScene8 *scn,
			   sAdjRel3 *A,
			   bool frame){ //include external frame?
      sScene8 *hscn=NULL;
      int p,q,i;
      Voxel u,v;
      if(frame == false){
	hscn = Create(scn);
	for(u.c.z=0; u.c.z<hscn->zsize; u.c.z++){
	  for(u.c.y=0; u.c.y<hscn->ysize; u.c.y++){
	    for(u.c.x=0; u.c.x<hscn->xsize; u.c.x++){
	      p = GetVoxelAddress(hscn,u);
	      if(scn->data[p] != 0){
		for(i=1; i<A->n; i++){
		  v.v = u.v + A->d[i].v;
		  if(IsValidVoxel(hscn,v)){
		    q = GetVoxelAddress(hscn,v);
		    if(scn->data[p] != scn->data[q]){
		      hscn->data[p] = scn->data[p];
		      break;
		    }
		  } 
		}
	      }
	    }
	  }
	}
	return(hscn);
      }
      else
	return GetObjBorders(scn, A);
    }

    

    sScene32 *Mask2EDT(sScene8 *bin, sAdjRel3 *A,
		       char side, int limit, char sign){
      sScene32 *Dx=NULL,*Dy=NULL,*Dz=NULL,*cost;
      sScene8 *cont;
      sPQueue32 *Q=NULL;
      int i,p,q,n;
      Voxel u,v;
      int *sq=NULL,tmp=INT_MAX,dx,dy,dz;
      sAdjRel3 *A6 = gft::AdjRel3::Spheric(1.0);
      
      n  = MAX( MAX(bin->xsize, bin->ysize), bin->zsize);
      sq = gft::AllocIntArray(n);
      for (i=0; i < n; i++) 
	sq[i]=i*i;

      cost = gft::Scene32::Create(bin->xsize, bin->ysize, bin->zsize);
      cont = gft::Scene8::GetObjBorders(bin, A6);
      Dx = gft::Scene32::Create(cost);
      Dy = gft::Scene32::Create(cost);
      Dz = gft::Scene32::Create(cost);
      n  = cost->n;

      Q = gft::PQueue32::Create(2*(bin->xsize+bin->ysize+bin->zsize),
				n, cost->data);
      
      switch (side) {
      case INTERIOR:
	for(p = 0; p < n; p++){
	  if (bin->data[p] != 0){
	    if (cont->data[p] > 0){
	      cost->data[p] = 0;
	      gft::PQueue32::InsertElem(&Q, p);
	    } else
	      cost->data[p] = INT_MAX;	  
	  }else{
	    if (cost->data[p] != INT_MIN)
	      cost->data[p] = 0;
	  }
	}
	break;
      case EXTERIOR:
	for(p = 0; p < n; p++){
	  if (bin->data[p] == 0){
	    if (cost->data[p]!=INT_MIN)
	      cost->data[p] = INT_MAX;	  
	  }else{
	    if (cont->data[p]>0){
	      cost->data[p]=0;    
	      gft::PQueue32::InsertElem(&Q, p);
	    }else
	      cost->data[p] = 0;
	  }
	}
	break;
      case BOTH:
      default:    
	for(p = 0; p < n; p++){
	  if (cont->data[p] > 0){
	    cost->data[p]=0;    
	    gft::PQueue32::InsertElem(&Q, p);
	  }else{ 
	    if (cost->data[p]!=INT_MIN)
	      cost->data[p]=INT_MAX;    
	  }
	}
      }
      Destroy(&cont);
      
      while(!gft::PQueue32::IsEmpty(Q)) {
	p = gft::PQueue32::RemoveMinFIFO(Q);
	u.c.x = gft::Scene32::GetAddressX(cost, p);
	u.c.y = gft::Scene32::GetAddressY(cost, p);
	u.c.z = gft::Scene32::GetAddressZ(cost, p);

	for (i=1; i < A->n; i++){
	  v.v = u.v + A->d[i].v;
	  if(!gft::Scene32::IsValidVoxel(cost, v))
	    continue;
	  q = gft::Scene32::GetVoxelAddress(cost, v);

	  if (cost->data[p] < cost->data[q]){
	    dx  = Dx->data[p] + abs(A->d[i].axis.x);
	    dy  = Dy->data[p] + abs(A->d[i].axis.y);
	    dz  = Dz->data[p] + abs(A->d[i].axis.z);

	    tmp = sq[dx] + sq[dy] + sq[dz];
	    if (tmp < cost->data[q] && tmp <= limit){
	      if (cost->data[q] == INT_MAX){
		cost->data[q]  = tmp;
		gft::PQueue32::InsertElem(&Q, q);
	      }
	      else
		gft::PQueue32::UpdateElem(&Q, q, tmp);
	      Dx->data[q] = dx;
	      Dy->data[q] = dy;
	      Dz->data[q] = dz;
	    }
	  }
	}
      }
      gft::PQueue32::Destroy(&Q);
      gft::AdjRel3::Destroy(&A6);
      free(sq);
      gft::Scene32::Destroy(&Dx);
      gft::Scene32::Destroy(&Dy);
      gft::Scene32::Destroy(&Dz);
      
      // Eliminate infinite values */
      n = cost->n;
      for (i=0; i<n; i++) {
	if (cost->data[i]==INT_MAX)
	  cost->data[i] = limit;
      }
      
      // sign scene
      if (sign != 0){
	n  = cost->n;
	if (side != INTERIOR)
	  for (i=0; i<n; i++) {
	    if (bin->data[i] == 0) {
	      cost->data[i] = -cost->data[i];
	    }
	  }
      }
      return(cost);
    }
    


    sScene32 *LabelBinComp(sScene8 *bin, sAdjRel3 *A){
      sScene32 *label=NULL;
      int i,j,n,p,q,l=1;
      int px,py,pz,qx,qy,qz;
      int *FIFO=NULL;
      int first=0,last=0;
      
      label = gft::Scene32::Create(bin->xsize, bin->ysize, bin->zsize);
      n  = bin->n;
      FIFO  = gft::AllocIntArray(n);
      for(j=0; j < n; j++){
	if((bin->data[j] != 0)&&(label->data[j]==0)){
	  label->data[j]=l;
	  FIFO[last]=j;
	  last++;
	  while(first != last){
	    p = FIFO[first];
	    px = gft::Scene8::GetAddressX(bin, p);
	    py = gft::Scene8::GetAddressY(bin, p);
	    pz = gft::Scene8::GetAddressZ(bin, p);
	    first++;
	    for (i=1; i < A->n; i++){
	      qx = px + A->d[i].axis.x;
	      qy = py + A->d[i].axis.y;
	      qz = pz + A->d[i].axis.z;
	      if(gft::Scene8::IsValidVoxel(bin, qx,qy,qz)){
		q = gft::Scene8::GetVoxelAddress(bin, qx, qy, qz);
		if ((bin->data[q] != 0)&&(label->data[q] == 0)){
		  label->data[q] = label->data[p];
		  FIFO[last] = q;
		  last++;
		}
	      }
	    }
	  }
	  l++;
	  first=last=0;
	}
      }
      gft::FreeIntArray(&FIFO);
      return(label);
    }    


    void SelectLargestComp(sScene8 *bin){
      gft::sAdjRel3 *A=NULL;
      A = gft::AdjRel3::Spheric(1.0); //1.5
      SelectLargestComp(bin, A);
      gft::AdjRel3::Destroy(&A);
    }



    void SelectLargestComp(sScene8 *bin, gft::sAdjRel3 *A){
      gft::sScene32 *label=NULL;
      int Lmax;
      int *area=NULL;
      int imax,i,p,n=bin->n;
      
      if(gft::Scene8::GetMaxVal(bin)==0)
	return;
      label = gft::Scene8::LabelBinComp(bin, A);
      Lmax = gft::Scene32::GetMaxVal(label);
      area = (int *)gft::AllocIntArray(Lmax+1);
      
      for (p=0; p < n; p++)  
	if (label->data[p]>0)
	  area[label->data[p]]++;
      imax = 0;
      for (i=1; i <= Lmax; i++) 
	if (area[i]>area[imax])
	  imax = i;
      for (p=0; p < n; p++)  
	if (label->data[p]!=imax)
	  bin->data[p]=0;

      gft::Scene32::Destroy(&label);
      gft::FreeIntArray(&area);
    }
    

    int GetBinArea(sScene8 *bin){
      int area=0;
      int p,n = bin->n;
      for(p=0; p<n; p++){
	if(bin->data[p]>0)
	  area++;
      }
      return area;
    }    
    
    
  } /*end Scene8 namespace*/
} /*end gft namespace*/

