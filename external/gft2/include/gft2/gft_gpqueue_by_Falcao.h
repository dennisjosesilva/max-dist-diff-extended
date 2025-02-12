#ifndef _GFT_GQUEUE_H_
#define _GFT_GQUEUE_H_

#include "gft2/gft_common.h"

namespace gft{
  namespace GQueue{

#define MINVALUE   0 /* define queue to remove node with minimum value */
#define MAXVALUE   1 /* define queue to remove node with maximum value */
#define FIFOBREAK 0  /* define queue to solve ambiguity by FIFO */
#define LIFOBREAK 1  /* define queue to solve ambiguity by LIFO */
#define QSIZE     32768

    struct sGQNode { 
      int  next;  /* next node */
      int  prev;  /* prev node */
      char color; /* WHITE=0, GRAY=1, BLACK=2 */ 
    };

    struct sGDoublyLinkedLists {
      sGQNode *elem;  /* all possible doubly-linked lists of 
			 the circular queue */
      int nelems;  /* total number of elements */
      int *value;   /* the value of the nodes in the graph */
    }; 

    struct sGCircularQueue { 
      int  *first;   /* list of the first elements of each 
			doubly-linked list */
      int  *last;    /* list of the last  elements of each 
			doubly-linked list  */
      int  nbuckets; /* number of buckets in the circular queue */
      int  minvalue;  /* minimum value of a node in queue */
      int  maxvalue;  /* maximum value of a node in queue */
      char tiebreak; /* 1 is LIFO, 0 is FIFO (default) */
      char removal_policy; /* 0 is MINVALUE and 1 is MAXVALUE */
    };

    struct sGQueue { /* Priority queue by Dial implemented as
			proposed by A. Falcao */
      sGCircularQueue C;
      sGDoublyLinkedLists L;
    };

    sGQueue *Create(int nbuckets, int nelems, int *value);
    void   Destroy(sGQueue **Q);
    void   Reset(sGQueue *Q);
    int    IsEmpty(sGQueue *Q);
    void   Insert(sGQueue **Q, int elem);
    int    Remove(sGQueue *Q);
    void   RemoveElem(sGQueue *Q, int elem);
    void   Update(sGQueue **Q, int elem, int newvalue);
    sGQueue *Grow(sGQueue **Q, int nbuckets);

    inline void SetTieBreak(sGQueue *Q, char tiebreak){ 
      Q->C.tiebreak = tiebreak; 
    }

    inline void SetRemovalPolicy(sGQueue *Q, char policy){ 
      Q->C.removal_policy = policy;
    }

  } //end GQueue namespace

  typedef GQueue::sGQueue sGQueue;

} //end gft namespace

#endif

