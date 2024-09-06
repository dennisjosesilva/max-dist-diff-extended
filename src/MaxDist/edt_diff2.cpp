
#include <queue>
#include <gft2/gft.h>
#include <vector>
#include <morphotree/core/alias.hpp>

#include <MaxDist/edt_diff2.hpp>


void treeRemoval2(
  const std::vector<morphotree::uint32> &toRemove,
  std::vector<int> &stack,
  gft::sImage32 *bin,
  gft::sPQueue32 *Q,
  gft::sImage32 *root,
  gft::sImage32 *pred,
  gft::sImage32 *cost,
  gft::sAdjPxl *N)
{
  int i, p, q, top = -1;

  for(morphotree::uint32 pidx : toRemove){
    p = static_cast<int>(pidx);

    cost->data[p] = INT_MAX;
    // pred->data[p] = NIL;
    // root->data[p] = p;
    Q->L.elem[p].color = WHITE;    
    top++;
    stack[top] = p;
  }
  //while(!path.empty()){
  while(top > -1) {
    p = stack[top];
    top--;

    for (i=1; i < N->n; i++){
      q = p + N->dp[i];
      
      if(p == pred->data[q]){
        cost->data[q] = INT_MAX;
        pred->data[q] = NIL;
        // root->data[q] = q;
        Q->L.elem[q].color = WHITE;
        //path.push(q);
        top++;
        stack[top] = q;
      }
      else if(bin->data[q] > 0 &&
        cost->data[root->data[q]] != INT_MAX) {
        if(cost->data[q] != INT_MAX &&
          Q->L.elem[q].color != GRAY){
          Q->L.elem[q].color = WHITE;
          //gft::PQueue32::InsertElem(&Q, q);
          gft::PQueue32::FastInsertElem(Q, q);
          //printf("Frontier: %d\n", q);
        }
      }
    }
  }
}

void removeSubTree2(int q_in,		   
		   gft::sPQueue32 *Q,		   
		   gft::sImage32 *pred,
		   gft::sImage32 *cost,
		   gft::sAdjPxl *N){
  std::queue<int> path, frontier_path;
  int i, p, q;
  
  path.push(q_in);
  //frontier_path.push(q_in);
  
  while(!path.empty()){
    p = path.front();
    path.pop();
    
    if(Q->L.elem[p].color == GRAY)
      gft::PQueue32::FastRemoveElem(Q, p);
      //gft::PQueue32::RemoveElem(Q, p);
    Q->L.elem[p].color = WHITE;
    cost->data[p] = INT_MAX;
    pred->data[p] = NIL;
    
    for (i=1; i < N->n; i++){
      q = p + N->dp[i];
      
      if (p == pred->data[q])
        path.push(q);
      else if(Q->L.elem[q].color != GRAY && cost->data[q] != INT_MAX)
        frontier_path.push(q);
    }
  }
  
  while(!frontier_path.empty()){
    p = frontier_path.front();
    frontier_path.pop();
    if(cost->data[p] != INT_MAX && Q->L.elem[p].color != GRAY) {
      Q->L.elem[p].color = WHITE;
      //gft::PQueue32::InsertElem(&Q, p);
      gft::PQueue32::FastInsertElem(Q, p);
    }
  }
}




void EDT_DIFF2(gft::sPQueue32 *Q,
	      gft::sAdjRel *A,
	      gft::sAdjPxl *N,
	      gft::sImage32 *bin,
	      gft::sImage32 *root,
	      gft::sImage32 *pred,
	      gft::sImage32 *cost,
	      gft::sImage32 *Bedt){
  gft::Pixel u,v,t;
  int i, p, q, r;
  int tmp,dx,dy;
  /*
  for(i = 0; i < nSeeds; i++){
    p = Seeds[i];
    if(Q->L.elem[p].color == WHITE){
      root->data[p] = p;
      pred->data[p] = NIL;
      cost->data[p] = 0;
      //gft::PQueue32::InsertElem(&Q, p);
      gft::PQueue32::FastInsertElem(Q, p);
    }
  }
  */
  
  while(!gft::PQueue32::IsEmpty(Q)){
    //printf("nadded: %d\n", Q->nadded);
    //p = gft::PQueue32::RemoveMinFIFO(Q);
    p = gft::PQueue32::FastRemoveMinFIFO(Q);
    //printf("p: %d\n",p);
    u.x = p % cost->ncols;
    u.y = p / cost->ncols;

    r = root->data[p];
    t.x = r % cost->ncols;
    t.y = r / cost->ncols;

    Bedt->data[r] = MAX(Bedt->data[r], cost->data[p]);
    
    for (i=1; i < A->n; i++){
      v.x = u.x + A->dx[i];
      v.y = u.y + A->dy[i];
      
      if(v.x >= 0 && v.x < cost->ncols &&
	 v.y >= 0 && v.y < cost->nrows){
	q = v.x + v.y * cost->ncols;
	
	//if(cost->data[p] < cost->data[q]){
	dx  = v.x - t.x; //Dx->data[p] + abs(A->dx[i]);
	dy  = v.y - t.y; //Dy->data[p] + abs(A->dy[i]);
	tmp = SQUARE(dx) + SQUARE(dy);

	if(tmp < cost->data[q]){
	  if(Q->L.elem[q].color != GRAY){ //if (cost->data[q] == INT_MAX){
	    cost->data[q]  = tmp;
	    //gft::PQueue32::InsertElem(&Q, q);
	    gft::PQueue32::FastInsertElem(Q, q);
	  }
	  else
	    gft::PQueue32::FastUpdateElem(Q, q, tmp);
	  //gft::PQueue32::UpdateElem(&Q, q, tmp);
	    
	  pred->data[q] = p;
	  root->data[q] = root->data[p];
	  //Dx->data[q] = dx;
	  //Dy->data[q] = dy;
	}
	else if(pred->data[q] == p){
	  if(tmp > cost->data[q] ||
	     root->data[p] != root->data[q]){
	    removeSubTree2(q, Q, pred, cost, N);
	    break;
	  }
	}
	//}
      }
    }
  }
}



