#ifndef _GFT_HEAP_LEX_H_
#define _GFT_HEAP_LEX_H_

#include "gft_common.h"
#include "gft_heap.h"
#include "gft_gpqueue_by_Falcao.h"


namespace gft{
  namespace Heap_lex{

    struct sHeap_lex {
      float *cost1;
      float *cost2;
      char *color;
      int *pixel;
      int *pos;
      int last;
      int n;
    };

    char IsFull(sHeap_lex *H);
    char IsEmpty(sHeap_lex *H);
    sHeap_lex *Create(int n, float *cost1, float *cost2);
    void Destroy(sHeap_lex **H);

    void Insert_MaxPolicy(sHeap_lex *H, int pixel);
    void Insert_MinPolicy(sHeap_lex *H, int pixel);
    
    void Remove_MaxPolicy(sHeap_lex *H, int *pixel);
    void Remove_MinPolicy(sHeap_lex *H, int *pixel);

    void Get_MaxPolicy(sHeap_lex *H, int *pixel);
    void Get_MinPolicy(sHeap_lex *H, int *pixel);
    
    void Update_MaxPolicy(sHeap_lex *H, int p, float value1, float value2);
    void Update_MinPolicy(sHeap_lex *H, int p, float value1, float value2);

    void GoUp_MaxPolicy(sHeap_lex *H, int i);
    void GoUp_MinPolicy(sHeap_lex *H, int i);

    void GoDown_MaxPolicy(sHeap_lex *H, int i);
    void GoDown_MinPolicy(sHeap_lex *H, int i);

    void Reset(sHeap_lex *H);

    void Delete_MaxPolicy(sHeap_lex *H, int pixel);
    void Delete_MinPolicy(sHeap_lex *H, int pixel);

    
  } //end Heap_lex namespace

  typedef Heap_lex::sHeap_lex sHeap_lex;

} //end gft namespace

#endif

