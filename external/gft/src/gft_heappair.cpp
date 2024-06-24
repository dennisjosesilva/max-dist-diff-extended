#include "gft_heappair.h"

namespace gft{
  namespace HeapPair{


    void GoUp_MaxPolicy_0(sHeapPair *H, int i){
      int j = HEAP_DAD(i);
      int p = H->pixel[i];
      float c = H->cost[p];
      while((i > 1)&&(H->cost[H->pixel[j]] < c)){
	H->pixel[i] = H->pixel[j];
	H->pos[H->pixel[j]] = i;
	i = j;
	j = HEAP_DAD(i);
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }

    
    void GoUp_MaxPolicy_1(sHeapPair *H, int i){
      int j = H->n+1 - HEAP_DAD(H->n+1 - i);
      int p = H->pixel[i];
      float c = H->cost[p];
      while((i < H->n)&&(H->cost[H->pixel[j]] < c)){
	H->pixel[i] = H->pixel[j];
	H->pos[H->pixel[j]] = i;
	i = j;
	j = H->n+1 - HEAP_DAD(H->n+1 - i);
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }
    
    

    void GoUp_MinPolicy_0(sHeapPair *H, int i){
      int j = HEAP_DAD(i);
      int p = H->pixel[i];
      float c = H->cost[p];
      while((i > 1)&&(H->cost[H->pixel[j]] > c)){
	H->pixel[i] = H->pixel[j];
	H->pos[H->pixel[j]] = i;
	i = j;
	j = HEAP_DAD(i);
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }    


    void GoUp_MinPolicy_1(sHeapPair *H, int i){
      int j = H->n+1 - HEAP_DAD(H->n+1 - i);
      int p = H->pixel[i];
      float c = H->cost[p];
      while((i < H->n)&&(H->cost[H->pixel[j]] > c)){
	H->pixel[i] = H->pixel[j];
	H->pos[H->pixel[j]] = i;
	i = j;
	j = H->n+1 - HEAP_DAD(H->n+1 - i);
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }


    
    void GoDown_MaxPolicy_0(sHeapPair *H, int i) {
      int j, left, right, p = H->pixel[i];
      float c = H->cost[p];
      j = i;
      while(1){
	i = j;
	left = HEAP_LEFTSON(i);
	right = left + 1;
	if(right <= H->last_0){
	  if(H->cost[H->pixel[right]] > H->cost[H->pixel[left]])
	    j = right;
	  else
	    j = left;
	}
	else if(left <= H->last_0)
	  j = left;
	else
	  break;
	
	if(H->cost[H->pixel[j]] > c) {
	  H->pixel[i] = H->pixel[j];
	  H->pos[H->pixel[j]] = i;
	}
	else break;
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }


    
    void GoDown_MaxPolicy_1(sHeapPair *H, int i) {
      int j, left, right, p = H->pixel[i];
      float c = H->cost[p];
      j = i;
      while(1){
	i = j;
	left = H->n+1 - HEAP_LEFTSON(H->n+1 - i);
	right = left - 1;
	if(right >= H->last_1){
	  if(H->cost[H->pixel[right]] > H->cost[H->pixel[left]])
	    j = right;
	  else
	    j = left;
	}
	else if(left >= H->last_1)
	  j = left;
	else
	  break;
	
	if(H->cost[H->pixel[j]] > c) {
	  H->pixel[i] = H->pixel[j];
	  H->pos[H->pixel[j]] = i;
	}
	else break;
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }
    
    

    void GoDown_MinPolicy_0(sHeapPair *H, int i) {
      int j, left, right, p = H->pixel[i];
      float c = H->cost[p];
      j = i;
      while(1){
	i = j;
	left = HEAP_LEFTSON(i);
	right = left + 1;
	if(right <= H->last_0){
	  if(H->cost[H->pixel[right]] < H->cost[H->pixel[left]])
	    j = right;
	  else
	    j = left;
	}
	else if(left <= H->last_0)
	  j = left;
	else
	  break;
	
	if(H->cost[H->pixel[j]] < c) {
	  H->pixel[i] = H->pixel[j];
	  H->pos[H->pixel[j]] = i;
	}
	else break;
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }



    void GoDown_MinPolicy_1(sHeapPair *H, int i) {
      int j, left, right, p = H->pixel[i];
      float c = H->cost[p];
      j = i;
      while(1){
	i = j;
	left = H->n+1 - HEAP_LEFTSON(H->n+1 - i);
	right = left - 1;
	if(right >= H->last_1){
	  if(H->cost[H->pixel[right]] < H->cost[H->pixel[left]])
	    j = right;
	  else
	    j = left;
	}
	else if(left >= H->last_1)
	  j = left;
	else
	  break;
	
	if(H->cost[H->pixel[j]] < c) {
	  H->pixel[i] = H->pixel[j];
	  H->pos[H->pixel[j]] = i;
	}
	else break;
      }
      H->pixel[i] = p;
      H->pos[p] = i;
    }    


    
   
    char IsFull(sHeapPair *H) {
      if (H->last_0 == H->last_1 - 1)
	return 1;
      else
	return 0;
    }
    
    char IsEmpty(sHeapPair *H) {
      if (H->last_0 == 0 && H->last_1 == H->n + 1)
	return 1;
      else
	return 0;
    }

    
    sHeapPair *Create(int n, float *cost) {
      sHeapPair *H = NULL;
      int i;
      
      if (cost == NULL) {
	fprintf(stdout,"Cannot create heap without cost map in HeapPair::Create");
	return NULL;
      }
      
      H = (sHeapPair *) malloc(sizeof(sHeapPair));
      if (H != NULL) {
	H->n       = n;
	H->cost    = cost;
	H->color   = (char *) malloc(sizeof(char) * n);
	H->pixel   = (int *) malloc(sizeof(int) * (n+1));
	H->pos     = (int *) malloc(sizeof(int) * n);
	H->last_0  = 0;
	H->last_1  = n+1;
	if (H->color == NULL || H->pos == NULL || H->pixel == NULL)
	  gft::Error((char *)MSG1,(char *)"HeapPair::Create");
	for (i = 0; i < H->n; i++)
	  H->color[i] = WHITE;
      }
      else
	gft::Error((char *)MSG1,(char *)"HeapPair::Create");

      return H;
    }

    
    void Destroy(sHeapPair **H) {
      sHeapPair *aux = *H;
      if (aux != NULL) {
	if (aux->pixel != NULL) free(aux->pixel);
	if (aux->color != NULL) free(aux->color);
	if (aux->pos != NULL)   free(aux->pos);
	free(aux);
	*H = NULL;
      }
    }

    
    void Insert_MaxPolicy_0(sHeapPair *H, int pixel) {
      H->last_0++;
      H->pixel[H->last_0] = pixel;
      H->color[pixel]     = GRAY;
      H->pos[pixel]       = H->last_0;
      GoUp_MaxPolicy_0(H, H->last_0);
    }


    void Insert_MaxPolicy_1(sHeapPair *H, int pixel) {
      H->last_1--;
      H->pixel[H->last_1] = pixel;
      H->color[pixel]     = GRAY;
      H->pos[pixel]       = H->last_1;
      GoUp_MaxPolicy_1(H, H->last_1);
    }
    
    
    void Insert_MinPolicy_0(sHeapPair *H, int pixel) {
      H->last_0++;
      H->pixel[H->last_0] = pixel;
      H->color[pixel]     = GRAY;
      H->pos[pixel]       = H->last_0;
      GoUp_MinPolicy_0(H, H->last_0);
    }

    void Insert_MinPolicy_1(sHeapPair *H, int pixel) {
      H->last_1--;
      H->pixel[H->last_1] = pixel;
      H->color[pixel]     = GRAY;
      H->pos[pixel]       = H->last_1;
      GoUp_MinPolicy_1(H, H->last_1);
    }
    
    
    void Remove_MaxPolicy_0(sHeapPair *H, int *pixel) {
      *pixel = H->pixel[1];
      H->color[*pixel] = BLACK;
      if(H->last_0 == 1){
	H->last_0 = 0;
      }
      else{ 
	H->pixel[1] = H->pixel[H->last_0];
	H->pos[H->pixel[1]] = 1;
	H->last_0--;
	GoDown_MaxPolicy_0(H, 1);
      }
    }


    void Remove_MaxPolicy_1(sHeapPair *H, int *pixel) {
      *pixel = H->pixel[H->n];
      H->color[*pixel] = BLACK;
      if(H->last_1 == H->n){
	H->last_1 = H->n+1;
      }
      else{ 
	H->pixel[H->n] = H->pixel[H->last_1];
	H->pos[H->pixel[H->n]] = H->n;
	H->last_1++;
	GoDown_MaxPolicy_1(H, H->n);
      }
    }
    


    void Remove_MinPolicy_0(sHeapPair *H, int *pixel) {
      *pixel = H->pixel[1];
      H->color[*pixel] = BLACK;
      if(H->last_0 == 1){
	H->last_0 = 0;
      }
      else{ 
	H->pixel[1] = H->pixel[H->last_0];
	H->pos[H->pixel[1]] = 1;
	H->last_0--;
	GoDown_MinPolicy_0(H, 1);
      }
    }


    void Remove_MinPolicy_1(sHeapPair *H, int *pixel) {
      *pixel = H->pixel[H->n];
      H->color[*pixel] = BLACK;
      if(H->last_1 == H->n){
	H->last_1 = H->n+1;
      }
      else{ 
	H->pixel[H->n] = H->pixel[H->last_1];
	H->pos[H->pixel[H->n]] = H->n;
	H->last_1++;
	GoDown_MinPolicy_1(H, H->n);
      }
    }

    

    void Get_MaxPolicy_0(sHeapPair *H, int *pixel){
      *pixel = H->pixel[1];
    }

    void Get_MaxPolicy_1(sHeapPair *H, int *pixel){
      *pixel = H->pixel[H->n];
    }
    
    
    void Get_MinPolicy_0(sHeapPair *H, int *pixel){
      *pixel = H->pixel[1];
    }

    void Get_MinPolicy_1(sHeapPair *H, int *pixel){
      *pixel = H->pixel[H->n];
    }


    
    void Update_MaxPolicy_0(sHeapPair *H, int p, float value){
      H->cost[p] = value;
      
      //if (H->color[p] == BLACK) printf("ferrou\n");
      
      if(H->color[p] == WHITE)
	Insert_MaxPolicy_0(H, p);
      else if(H->pos[p] <= H->last_0)
	GoUp_MaxPolicy_0(H, H->pos[p]);
      else{
	Delete_MaxPolicy_1(H, p);
	Insert_MaxPolicy_0(H, p);
      }
    }


    void Update_MaxPolicy_1(sHeapPair *H, int p, float value){
      H->cost[p] = value;
      
      //if (H->color[p] == BLACK) printf("ferrou\n");
      
      if(H->color[p] == WHITE)
	Insert_MaxPolicy_1(H, p);
      else if(H->pos[p] >= H->last_1)
	GoUp_MaxPolicy_1(H, H->pos[p]);
      else{
	Delete_MaxPolicy_0(H, p);
	Insert_MaxPolicy_1(H, p);
      }
    }
    

    void Update_MinPolicy_0(sHeapPair *H, int p, float value){
      H->cost[p] = value;
      
      //if (H->color[p] == BLACK) printf("ferrou\n");
      
      if(H->color[p] == WHITE)
	Insert_MinPolicy_0(H, p);
      else if(H->pos[p] <= H->last_0)
	GoUp_MinPolicy_0(H, H->pos[p]);
      else{
	Delete_MinPolicy_1(H, p);
	Insert_MinPolicy_0(H, p);	
      }
    }


    void Update_MinPolicy_1(sHeapPair *H, int p, float value){
      H->cost[p] = value;
      
      //if (H->color[p] == BLACK) printf("ferrou\n");
      
      if(H->color[p] == WHITE)
	Insert_MinPolicy_1(H, p);
      else if(H->pos[p] >= H->last_1)
	GoUp_MinPolicy_1(H, H->pos[p]);
      else{
	Delete_MinPolicy_0(H, p);
	Insert_MinPolicy_1(H, p);
      }
    }    
    
    
    void Reset(sHeapPair *H){
      int i;
      for (i=0; i < H->n; i++)
	H->color[i] = WHITE;
      H->last_0 = 0;
      H->last_1 = H->n+1;
    }



    void Delete_MaxPolicy(sHeapPair *H, int pixel){
      if(H->pos[pixel] <= H->last_0)
	Delete_MaxPolicy_0(H, pixel);
      else
	Delete_MaxPolicy_1(H, pixel);      
    }
    

    void Delete_MaxPolicy_0(sHeapPair *H, int pixel) {
      int q;
      //if (IsEmpty(H)) printf("error: delete pixel%d\n", pixel);
      H->color[pixel] = WHITE;
      q = H->pixel[H->last_0];
      if(pixel != q){
	H->pixel[H->pos[pixel]] = q;
	H->pos[q] = H->pos[pixel];
	H->last_0--;
	if(H->cost[pixel] > H->cost[q])
	  GoDown_MaxPolicy_0(H, H->pos[q]);
	else 
	  GoUp_MaxPolicy_0(H, H->pos[q]);
      }
      else
	H->last_0--;
    }


    void Delete_MaxPolicy_1(sHeapPair *H, int pixel) {
      int q;
      //if (IsEmpty(H)) printf("error: delete pixel%d\n", pixel);
      H->color[pixel] = WHITE;
      q = H->pixel[H->last_1];
      if(pixel != q){
	H->pixel[H->pos[pixel]] = q;
	H->pos[q] = H->pos[pixel];
	H->last_1++;
	if(H->cost[pixel] > H->cost[q])
	  GoDown_MaxPolicy_1(H, H->pos[q]);
	else 
	  GoUp_MaxPolicy_1(H, H->pos[q]);
      }
      else
	H->last_1++;
    }



    void Delete_MinPolicy(sHeapPair *H, int pixel){
      if(H->pos[pixel] <= H->last_0)
	Delete_MinPolicy_0(H, pixel);
      else
	Delete_MinPolicy_1(H, pixel);
    }

    

    void Delete_MinPolicy_0(sHeapPair *H, int pixel) {
      int q;
      //if (IsEmpty(H)) printf("error: delete pixel%d\n", pixel);
      H->color[pixel] = WHITE;
      q = H->pixel[H->last_0];
      if(pixel != q){
	H->pixel[H->pos[pixel]] = q;
	H->pos[q] = H->pos[pixel];
	H->last_0--;
	if(H->cost[pixel] < H->cost[q])
	  GoDown_MinPolicy_0(H, H->pos[q]);
	else 
	  GoUp_MinPolicy_0(H, H->pos[q]);
      }
      else
	H->last_0--;
    }



    void Delete_MinPolicy_1(sHeapPair *H, int pixel) {
      int q;
      //if (IsEmpty(H)) printf("error: delete pixel%d\n", pixel);
      H->color[pixel] = WHITE;
      q = H->pixel[H->last_1];
      if(pixel != q){
	H->pixel[H->pos[pixel]] = q;
	H->pos[q] = H->pos[pixel];
	H->last_1++;
	if(H->cost[pixel] < H->cost[q])
	  GoDown_MinPolicy_1(H, H->pos[q]);
	else 
	  GoUp_MinPolicy_1(H, H->pos[q]);
      }
      else
	H->last_1++;
    }
    
   
    
  } /*end HeapPair namespace*/
} /*end gft namespace*/


