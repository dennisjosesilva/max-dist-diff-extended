#ifndef _GFT_HEAP32FIIF_LEX_H_
#define _GFT_HEAP32FIIF_LEX_H_

#include "gft_common.h"
#include "gft_heap.h"
#include "gft_gpqueue_by_Falcao.h"


namespace gft{
  namespace Heap32fiif_lex{

    struct sHeap32fiif_lex {
      float *cost1;
      int   *cost2;
      int   *cost3;
      float *cost4;
      char *color;
      int *pixel;
      int *pos;
      int last;
      int n;
    };

    char IsFull(sHeap32fiif_lex *H);
    char IsEmpty(sHeap32fiif_lex *H);
    sHeap32fiif_lex *Create(int n,
			    float *cost1, int   *cost2,
			    int   *cost3, float *cost4);
    void Destroy(sHeap32fiif_lex **H);

    void Insert_MaxPolicy(sHeap32fiif_lex *H, int pixel);
    void Insert_MinPolicy(sHeap32fiif_lex *H, int pixel);
    
    void Remove_MaxPolicy(sHeap32fiif_lex *H, int *pixel);
    void Remove_MinPolicy(sHeap32fiif_lex *H, int *pixel);

    void Get_MaxPolicy(sHeap32fiif_lex *H, int *pixel);
    void Get_MinPolicy(sHeap32fiif_lex *H, int *pixel);
    
    void Update_MaxPolicy(sHeap32fiif_lex *H, int p,
			  float value1, int   value2,
			  int   value3, float value4);
    void Update_MinPolicy(sHeap32fiif_lex *H, int p,
			  float value1, int   value2,
			  int   value3, float value4);

    void GoUp_MaxPolicy(sHeap32fiif_lex *H, int i);
    void GoUp_MinPolicy(sHeap32fiif_lex *H, int i);

    void GoDown_MaxPolicy(sHeap32fiif_lex *H, int i);
    void GoDown_MinPolicy(sHeap32fiif_lex *H, int i);

    void Reset(sHeap32fiif_lex *H);

    void Delete_MaxPolicy(sHeap32fiif_lex *H, int pixel);
    void Delete_MinPolicy(sHeap32fiif_lex *H, int pixel);

    
  } //end Heap32fiif_lex namespace

  typedef Heap32fiif_lex::sHeap32fiif_lex sHeap32fiif_lex;

} //end gft namespace

#endif

