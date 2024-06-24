#ifndef _GFT_HEAPPAIR_H_
#define _GFT_HEAPPAIR_H_

#include "gft_common.h"
#include "gft_gpqueue_by_Falcao.h"

#include "gft_heap.h"

namespace gft{
  namespace HeapPair{

    struct sHeapPair {
      float *cost;
      char *color;
      int *pixel;
      int *pos;
      int last_0;
      int last_1;
      int n;
    };

    /* Auxiliary Functions */

    char IsFull(sHeapPair *H);
    char IsEmpty(sHeapPair *H);
    sHeapPair *Create(int n, float *cost);
    void Destroy(sHeapPair **H);

    void Insert_MaxPolicy_0(sHeapPair *H, int pixel);
    void Insert_MaxPolicy_1(sHeapPair *H, int pixel);    
    void Insert_MinPolicy_0(sHeapPair *H, int pixel);
    void Insert_MinPolicy_1(sHeapPair *H, int pixel);    
    
    void Remove_MaxPolicy_0(sHeapPair *H, int *pixel);
    void Remove_MaxPolicy_1(sHeapPair *H, int *pixel);    
    void Remove_MinPolicy_0(sHeapPair *H, int *pixel);
    void Remove_MinPolicy_1(sHeapPair *H, int *pixel);    

    void Get_MaxPolicy_0(sHeapPair *H, int *pixel);
    void Get_MaxPolicy_1(sHeapPair *H, int *pixel);    
    void Get_MinPolicy_0(sHeapPair *H, int *pixel);
    void Get_MinPolicy_1(sHeapPair *H, int *pixel);    
    
    void Update_MaxPolicy_0(sHeapPair *H, int p, float value);
    void Update_MaxPolicy_1(sHeapPair *H, int p, float value);    
    void Update_MinPolicy_0(sHeapPair *H, int p, float value);
    void Update_MinPolicy_1(sHeapPair *H, int p, float value);    

    void GoUp_MaxPolicy_0(sHeapPair *H, int i);
    void GoUp_MaxPolicy_1(sHeapPair *H, int i);    
    void GoUp_MinPolicy_0(sHeapPair *H, int i);
    void GoUp_MinPolicy_1(sHeapPair *H, int i);
    
    void GoDown_MaxPolicy_0(sHeapPair *H, int i);
    void GoDown_MaxPolicy_1(sHeapPair *H, int i);    
    void GoDown_MinPolicy_0(sHeapPair *H, int i);
    void GoDown_MinPolicy_1(sHeapPair *H, int i);    

    void Reset(sHeapPair *H);

    void Delete_MaxPolicy(sHeapPair *H, int pixel);
    void Delete_MaxPolicy_0(sHeapPair *H, int pixel);    
    void Delete_MaxPolicy_1(sHeapPair *H, int pixel);
    void Delete_MinPolicy(sHeapPair *H, int pixel);
    void Delete_MinPolicy_0(sHeapPair *H, int pixel);    
    void Delete_MinPolicy_1(sHeapPair *H, int pixel);
    
  } //end HeapPair namespace

  typedef HeapPair::sHeapPair sHeapPair;

} //end gft namespace

#endif

