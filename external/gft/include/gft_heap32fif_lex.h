#ifndef _GFT_HEAP32FIF_LEX_H_
#define _GFT_HEAP32FIF_LEX_H_

#include "gft_common.h"
#include "gft_heap.h"
#include "gft_gpqueue_by_Falcao.h"


namespace gft{
  namespace Heap32fif_lex{

    struct sHeap32fif_lex {
      float *cost1;
      int   *cost2;
      float *cost3;
      char *color;
      int *pixel;
      int *pos;
      int last;
      int n;
    };

    char IsFull(sHeap32fif_lex *H);
    char IsEmpty(sHeap32fif_lex *H);
    sHeap32fif_lex *Create(int n, float *cost1, int *cost2, float *cost3);
    void Destroy(sHeap32fif_lex **H);

    void Insert_MaxPolicy(sHeap32fif_lex *H, int pixel);
    void Insert_MinPolicy(sHeap32fif_lex *H, int pixel);
    
    void Remove_MaxPolicy(sHeap32fif_lex *H, int *pixel);
    void Remove_MinPolicy(sHeap32fif_lex *H, int *pixel);

    void Get_MaxPolicy(sHeap32fif_lex *H, int *pixel);
    void Get_MinPolicy(sHeap32fif_lex *H, int *pixel);
    
    void Update_MaxPolicy(sHeap32fif_lex *H, int p,
			  float value1, int value2, float value3);
    void Update_MinPolicy(sHeap32fif_lex *H, int p,
			  float value1, int value2, float value3);

    void GoUp_MaxPolicy(sHeap32fif_lex *H, int i);
    void GoUp_MinPolicy(sHeap32fif_lex *H, int i);

    void GoDown_MaxPolicy(sHeap32fif_lex *H, int i);
    void GoDown_MinPolicy(sHeap32fif_lex *H, int i);

    void Reset(sHeap32fif_lex *H);

    void Delete_MaxPolicy(sHeap32fif_lex *H, int pixel);
    void Delete_MinPolicy(sHeap32fif_lex *H, int pixel);

    
  } //end Heap32fif_lex namespace

  typedef Heap32fif_lex::sHeap32fif_lex sHeap32fif_lex;

} //end gft namespace

#endif

