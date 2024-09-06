#include <MaxDist/edt_diff3.hpp>

// ============================================================
// Adaptive adjacency
// ===========================================================
struct sAdaptiveAdj *Create_AdaptiveAdj(int n)
{
  struct sAdaptiveAdj *N = NULL;
  N = (struct sAdaptiveAdj *)calloc(1, sizeof(struct sAdaptiveAdj));
  if (N == NULL) {
    printf("Error: insufficient memory in CreateAdaptiveAdj\n");
    exit(1);
  }
  N->dp = gft::AllocIntArray(n);
  N->ia = gft::AllocIntArray(n);
  N->n = n;
  N->np = n;
  return N;
}

void Destroy_AdaptiveAdj(struct sAdaptiveAdj **N)
{
  struct sAdaptiveAdj *aux;
  aux = *N;
  if (aux != NULL) {
    if (aux->dp != NULL) gft::FreeIntArray(&aux->dp);
    if (aux->ia != NULL) gft::FreeIntArray(&aux->ia);
    free(aux);
    *N = NULL;
  }
}

struct sAdaptiveAdjBank *Create_AdaptiveAdjBank(int n)
{
  struct sAdaptiveAdjBank *B = NULL;
  int i;
  B = (struct sAdaptiveAdjBank *) calloc(1, sizeof(struct sAdaptiveAdjBank));
  if (B == NULL) {
    printf("Error: insufficient memory in Create_AdaptiveAdjBank\n");
    exit(1);
  }
  B->nbank = n;
  B->N = (struct sAdaptiveAdj **) calloc(n, sizeof(struct sAdaptiveAdj *));
  if (B->N == NULL) {
    printf("Error: insufficient memory in Create_AdaptiveAdjBank\n");
    exit(1);
  }

  for (i = 0; i < n; i++) 
    B->N[i] = NULL;

  return B;
}

void Destroy_AdaptiveAdjBank(struct sAdaptiveAdjBank **B)
{
  struct sAdaptiveAdjBank *aux;
  int i;
  aux = *B;
  if (aux != NULL) {
    if (aux->N != NULL) {
      for (i = 0; i < aux->nbank; i++)
        Destroy_AdaptiveAdj(&(aux->N[i]));
      free(aux->N);
    }
    free(aux);
    *B = NULL;
  }
}

struct sAdaptiveAdjBank *AdaptiveAdj_8(int ncols)
{
    struct sAdaptiveAdjBank *B = NULL;
  B = Create_AdaptiveAdjBank(9);

  //All:
  B->N[0] = Create_AdaptiveAdj(8);
  B->N[0]->dp[0] =  1 + ncols*(-1);  B->N[0]->ia[0] = 5;
  B->N[0]->dp[1] =  1 + ncols*(0);   B->N[0]->ia[1] = 1;
  B->N[0]->dp[2] =  1 + ncols*(1);   B->N[0]->ia[2] = 6;
  B->N[0]->dp[3] = -1 + ncols*(-1);  B->N[0]->ia[3] = 7;
  B->N[0]->dp[4] = -1 + ncols*(0);   B->N[0]->ia[4] = 2;
  B->N[0]->dp[5] = -1 + ncols*(1);   B->N[0]->ia[5] = 8;
  B->N[0]->dp[6] =  0 + ncols*(-1);  B->N[0]->ia[6] = 3;
  B->N[0]->dp[7] =  0 + ncols*(1);   B->N[0]->ia[7] = 4;
  
  //Right:
  B->N[1] = Create_AdaptiveAdj(3);
  B->N[1]->dp[0] = 1 + ncols*(-1);   B->N[1]->ia[0] = 5;
  B->N[1]->dp[1] = 1 + ncols*(0);    B->N[1]->ia[1] = 1;
  B->N[1]->dp[2] = 1 + ncols*(1);    B->N[1]->ia[2] = 6;

  //Left:
  B->N[2] = Create_AdaptiveAdj(3);
  B->N[2]->dp[0] = -1 + ncols*(-1);  B->N[2]->ia[0] = 7;
  B->N[2]->dp[1] = -1 + ncols*(0);   B->N[2]->ia[1] = 2;
  B->N[2]->dp[2] = -1 + ncols*(1);   B->N[2]->ia[2] = 8;

  //Top:
  B->N[3] = Create_AdaptiveAdj(3);
  B->N[3]->dp[0] = -1 + ncols*(-1);  B->N[3]->ia[0] = 7;
  B->N[3]->dp[1] =  0 + ncols*(-1);  B->N[3]->ia[1] = 3;
  B->N[3]->dp[2] =  1 + ncols*(-1);  B->N[3]->ia[2] = 5;
  
  //Bottom:
  B->N[4] = Create_AdaptiveAdj(3);
  B->N[4]->dp[0] = -1 + ncols*(1);   B->N[4]->ia[0] = 8;
  B->N[4]->dp[1] =  0 + ncols*(1);   B->N[4]->ia[1] = 4;
  B->N[4]->dp[2] =  1 + ncols*(1);   B->N[4]->ia[2] = 6;

  //Top right:
  B->N[5] = Create_AdaptiveAdj(5);
  B->N[5]->dp[0] =  0 + ncols*(-1);  B->N[5]->ia[0] = 3;
  B->N[5]->dp[1] =  1 + ncols*(-1);  B->N[5]->ia[1] = 5;
  B->N[5]->dp[2] =  1 + ncols*(0);   B->N[5]->ia[2] = 1;
  B->N[5]->dp[3] = -1 + ncols*(-1);  B->N[5]->ia[3] = 7;
  B->N[5]->dp[4] =  1 + ncols*(1);   B->N[5]->ia[4] = 6;
  B->N[5]->np = 3;
  
  //Bottom right:
  B->N[6] = Create_AdaptiveAdj(5);
  B->N[6]->dp[0] =  0 + ncols*(1);   B->N[6]->ia[0] = 4;
  B->N[6]->dp[1] =  1 + ncols*(1);   B->N[6]->ia[1] = 6;
  B->N[6]->dp[2] =  1 + ncols*(0);   B->N[6]->ia[2] = 1;
  B->N[6]->dp[3] = -1 + ncols*(1);   B->N[6]->ia[3] = 8;
  B->N[6]->dp[4] =  1 + ncols*(-1);  B->N[6]->ia[4] = 5;
  B->N[6]->np = 3;
  
  //Top left:
  B->N[7] = Create_AdaptiveAdj(5);
  B->N[7]->dp[0] =  0 + ncols*(-1);  B->N[7]->ia[0] = 3;
  B->N[7]->dp[1] = -1 + ncols*(-1);  B->N[7]->ia[1] = 7;
  B->N[7]->dp[2] = -1 + ncols*(0);   B->N[7]->ia[2] = 2;
  B->N[7]->dp[3] =  1 + ncols*(-1);  B->N[7]->ia[3] = 5;
  B->N[7]->dp[4] = -1 + ncols*(1);   B->N[7]->ia[4] = 8;
  B->N[7]->np = 3;
  
  //Bottom left:
  B->N[8] = Create_AdaptiveAdj(5);
  B->N[8]->dp[0] =  0 + ncols*(1);   B->N[8]->ia[0] = 4;
  B->N[8]->dp[1] = -1 + ncols*(1);   B->N[8]->ia[1] = 8;
  B->N[8]->dp[2] = -1 + ncols*(0);   B->N[8]->ia[2] = 2;
  B->N[8]->dp[3] =  1 + ncols*(1);   B->N[8]->ia[3] = 6;
  B->N[8]->dp[4] = -1 + ncols*(-1);  B->N[8]->ia[4] = 7;
  B->N[8]->np = 3;
  
  return B;
}

gft::sImage8 *Create_AdjMap(int ncols, int nrows)
{
  gft::sImage8 *map = NULL;
  int i;
  map = gft::Image8::Create(ncols, nrows);
  for (i = 1; i < nrows; i++) {
    map->array[i][0] = 1;
    map->array[i][ncols-1] = 2;
  }

  for (i = 1; i < ncols-1; i++) {
    map->array[0][i] = 4;
    map->array[nrows-1][i] = 3;
  }

  map->array[0][0] = 6;
  map->array[0][ncols-1] = 8;
  map->array[nrows-1][0] = 5;
  map->array[nrows-1][ncols-1] = 7;
  return map;
}

// ===============================================================
// Functions
// ===============================================================
void treeRemoval3(
  const std::vector<morphotree::uint32> &toRemove,
  std::vector<int> &stack,
  gft::sImage32 *bin,
  gft::sPQueue32 *Q,
  gft::sImage32 *root,
  gft::sImage32 *cost,
  struct sAdaptiveAdjBank *AAB,
  gft::sImage8 *Amap,
  gft::sImage8 *O)
{
  struct sAdaptiveAdj *AA;
  int i, p, q, top = -1;
  
  for (morphotree::uint32 pidx : toRemove) {
    p = static_cast<int>(pidx);

    O->data[p] = 1;
    cost->data[p] = INT_MAX;
    Q->L.elem[p].color = WHITE;
    top++;
    stack[top] = p;
  }

  while (top > -1) {
    p = stack[top];
    top--;

    AA = AAB->N[Amap->data[p]];
    for (i = 0; i < AA->n; i++) {
      q = p + AA->dp[i];

      if (cost->data[root->data[q]] == INT_MAX) {
        if (O->data[q] == 0) {
          O->data[q] = 1;
          cost->data[q] = INT_MAX;
          Q->L.elem[q].color = WHITE;
          top++;
          stack[top] = q;
        }
      }
      else if (bin->data[q] > 0 && 
        Q->L.elem[q].color != GRAY) {
          Q->L.elem[q].color = WHITE;
          gft::PQueue32::FastInsertElem(Q, q);
        }
    }
  }
}

void EDT_DIFF3(gft::sPQueue32 *Q,
	      struct sAdaptiveAdjBank *AAB,
	      gft::sImage8 *Amap,
	      gft::sImage32 *bin,
	      gft::sImage32 *root,	      
	      gft::sImage32 *cost,
	      gft::sImage32 *Bedt,
        gft::sImage8 *O){
  struct sAdaptiveAdj *AA;
  gft::Pixel t;
  int i, p, q, r;
  int tmp,dx,dy;
  
  while(!gft::PQueue32::IsEmpty(Q)){
    p = gft::PQueue32::FastRemoveMinFIFO(Q);
    O->data[p] = 0;

    r = root->data[p];
    t.x = r % cost->ncols;
    t.y = r / cost->ncols;

    Bedt->data[r] = MAX(Bedt->data[r], cost->data[p]);
    
    AA = AAB->N[Amap->data[p]];
    for(i = 0; i < AA->np; i++){
      q = p + AA->dp[i];
      
      dx  = q % cost->ncols - t.x;
      dy  = q / cost->ncols - t.y;
      tmp = SQUARE(dx) + SQUARE(dy);
      
      if(tmp < cost->data[q] && O->data[q] == 1){
	if(Q->L.elem[q].color != GRAY){
	  cost->data[q]  = tmp;
	  gft::PQueue32::FastInsertElem(Q, q);
	}
	else
	  gft::PQueue32::FastUpdateElem(Q, q, tmp);
	
	root->data[q] = r;
	Amap->data[q] = AA->ia[i];
      }
    }
  }
}