#ifndef _GFT_HEAP32FI_LEX_H_
#define _GFT_HEAP32FI_LEX_H_

#include "gft_common.h"
#include "gft_heap.h"
#include "gft_gpqueue_by_Falcao.h"


namespace gft{
  namespace Heap32fi_lex{

    struct sHeap32fi_lex {
      float *cost1;
      int   *cost2;
      char *color;
      int *pixel;
      int *pos;
      int last;
      int n;
    };

    char IsFull(sHeap32fi_lex *H);
    char IsEmpty(sHeap32fi_lex *H);
    sHeap32fi_lex *Create(int n, float *cost1, int *cost2);
    void Destroy(sHeap32fi_lex **H);

    void Insert_MaxPolicy(sHeap32fi_lex *H, int pixel);
    void Insert_MinPolicy(sHeap32fi_lex *H, int pixel);
    
    void Remove_MaxPolicy(sHeap32fi_lex *H, int *pixel);
    void Remove_MinPolicy(sHeap32fi_lex *H, int *pixel);

    void Get_MaxPolicy(sHeap32fi_lex *H, int *pixel);
    void Get_MinPolicy(sHeap32fi_lex *H, int *pixel);
    
    void Update_MaxPolicy(sHeap32fi_lex *H, int p, float value1, int value2);
    void Update_MinPolicy(sHeap32fi_lex *H, int p, float value1, int value2);

    void GoUp_MaxPolicy(sHeap32fi_lex *H, int i);
    void GoUp_MinPolicy(sHeap32fi_lex *H, int i);

    void GoDown_MaxPolicy(sHeap32fi_lex *H, int i);
    void GoDown_MinPolicy(sHeap32fi_lex *H, int i);

    void Reset(sHeap32fi_lex *H);

    void Delete_MaxPolicy(sHeap32fi_lex *H, int pixel);
    void Delete_MinPolicy(sHeap32fi_lex *H, int pixel);

    
  } //end Heap32fi_lex namespace

  typedef Heap32fi_lex::sHeap32fi_lex sHeap32fi_lex;

} //end gft namespace

#endif

