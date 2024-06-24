#ifndef _GFT_HEAP32_H_
#define _GFT_HEAP32_H_

#include "gft_common.h"
#include "gft_gpqueue_by_Falcao.h"

#include "gft_heap.h"

namespace gft{
  namespace Heap32{

    struct sHeap32 {
      int *cost;
      char *color;
      int *pixel;
      int *pos;
      int last;
      int n;
    };

    /* Auxiliary Functions */

    char IsFull(sHeap32 *H);
    char IsEmpty(sHeap32 *H);
    sHeap32 *Create(int n, int *cost);
    void Destroy(sHeap32 **H);

    void Insert_MaxPolicy(sHeap32 *H, int pixel);
    void Insert_MinPolicy(sHeap32 *H, int pixel);
    
    void Remove_MaxPolicy(sHeap32 *H, int *pixel);
    void Remove_MinPolicy(sHeap32 *H, int *pixel);

    void Get_MaxPolicy(sHeap32 *H, int *pixel);
    void Get_MinPolicy(sHeap32 *H, int *pixel);
    
    void Update_MaxPolicy(sHeap32 *H, int p, int value);
    void Update_MinPolicy(sHeap32 *H, int p, int value);

    void GoUp_MaxPolicy(sHeap32 *H, int i);
    void GoUp_MinPolicy(sHeap32 *H, int i);

    void GoDown_MaxPolicy(sHeap32 *H, int i);
    void GoDown_MinPolicy(sHeap32 *H, int i);

    void Reset(sHeap32 *H);

    void Delete_MaxPolicy(sHeap32 *H, int pixel);
    void Delete_MinPolicy(sHeap32 *H, int pixel);

    //int Debug(sHeap32 *H, int p);
    
  } //end Heap32 namespace

  typedef Heap32::sHeap32 sHeap32;

} //end gft namespace

#endif

