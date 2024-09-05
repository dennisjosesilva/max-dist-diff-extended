#ifndef _GFT_PQUEUE8_H_
#define _GFT_PQUEUE8_H_

#include "gft3/gft_common.h"

namespace gft{
  namespace PQueue8{

    struct sPQNode { 
      int  next;  //!< Next node.
      int  prev;  //!< Previous node.
      char color; //!< WHITE=0, GRAY=1, BLACK=2.
    };
    
    struct sPQDoublyLinkedLists {
      sPQNode *elem; //!< All possible doubly-linked lists of the circular queue.
      int nelems;   //!< Total number of elements.
      uchar *value;   //!< The value of the nodes in the graph.
    }; 

    struct sPQCircularQueue { 
      int  *first;   //!< List of the first elements of each doubly-linked list.
      int  *last;    //!< List of the last  elements of each doubly-linked list.
      int  nbuckets; //!< Number of buckets in the circular queue.
      uchar  minvalue; //!< Minimum value of a node in queue.
      uchar  maxvalue; //!< Maximum value of a node in queue.
    };


    /**
     * \brief Priority queue by Dial implemented as proposed by 
     * A.X. Falcao with circular and growing features.
     */
    struct sPQueue8 { 
      sPQCircularQueue C;
      sPQDoublyLinkedLists L;
      int nadded;      //!< Number of elements added.
    };


    sPQueue8   *Create(int nbuckets, int nelems, uchar *value);
    void        Destroy(sPQueue8 **Q);
    sPQueue8   *Grow(sPQueue8 **Q, int nbuckets);
    void        Reset(sPQueue8 *Q);
    inline bool IsEmpty(sPQueue8 *Q){ 
      return (Q->nadded==0); 
    }
    inline bool IsFull(sPQueue8 *Q){ 
      return (Q->nadded==(Q->L).nelems); 
    }
    

    /**
     * Generic version with circular and growing features.
     */
    void   InsertElem(sPQueue8 **Q, int elem);
    /**
     * Generic version with circular and growing features.
     */
    void   RemoveElem(sPQueue8 *Q, int elem);
    /**
     * Generic version with circular and growing features.
     */
    void   UpdateElem(sPQueue8 **Q, int elem, uchar newvalue);
    /**
     * Generic version with circular and growing features.
     */
    int    RemoveMinFIFO(sPQueue8 *Q);
    /**
     * Generic version with circular and growing features.
     */
    int    RemoveMinLIFO(sPQueue8 *Q);
    /**
     * Generic version with circular and growing features.
     */
    int    RemoveMaxFIFO(sPQueue8 *Q);
    /**
     * Generic version with circular and growing features.
     */
    int    RemoveMaxLIFO(sPQueue8 *Q);


    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    void   FastInsertElem(sPQueue8 *Q, int elem);
    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    void   FastRemoveElem(sPQueue8 *Q, int elem);
    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    void   FastUpdateElem(sPQueue8 *Q, int elem, uchar newvalue);
    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    int    FastRemoveMinFIFO(sPQueue8 *Q);

    int    FastGetMinFIFO(sPQueue8 *Q);

    int    FastGetMinVal(sPQueue8 *Q);
    int    FastGetMaxVal(sPQueue8 *Q);
    
    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    int    FastRemoveMinLIFO(sPQueue8 *Q);
    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    int    FastRemoveMaxFIFO(sPQueue8 *Q);
    /**
     * Faster version to be used when values 
     * are in fixed range [0, nbuckets-1] (e.g., watershed transform).
     */
    int    FastRemoveMaxLIFO(sPQueue8 *Q);

    void   FastInsertElemAsFirst(sPQueue8 *Q, int elem);
    
  } //end PQueue32 namespace

  typedef PQueue8::sPQueue8 sPQueue8;  

} //end gft namespace

#endif


