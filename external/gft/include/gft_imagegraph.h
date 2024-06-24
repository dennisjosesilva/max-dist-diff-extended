/*
IFT:
custo favoravel   0 
custo normal      > 0, < INT_MAX
aresta impossivel INT_MAX

GC:
custo favoravel   INT_MAX = CAP_MAX
custo normal      > 0, < INT_MAX
aresta impossivel 0
*/

#ifndef _GFT_IMAGEGRAPH_H_
#define _GFT_IMAGEGRAPH_H_

#include "gft_common.h"
#include "gft_image32.h"
#include "gft_cimage.h"
#include "gft_adjrel.h"
#include "gft_filtering.h"
#include "gft_queue.h"

namespace gft{

  /**
   * \brief Common definitions and functions to manipulate a sparse graph of an image.
   *
   * The data structure assumes that the arcs are defined by an adjacency relation invariant to translation.
   */
  namespace ImageGraph{

#define DISSIMILARITY 0
#define CAPACITY      1

    /**
     * \brief Structure representing a sparse graph of an image.
     * 
     * The data structure assumes that the nodes are the image pixels and
     * the arcs are defined by an adjacency relation invariant to translation.
     */
    struct sImageGraph {
      int type; /**< The type can be DISSIMILARITY or CAPACITY.*/
      int Wmax; /**< The highest arc weight value.*/
      int **n_link; /**< The array 'n-link' stores the weights of arcs between pairs of neighboring pixels.*/
      int ncols; /**< The number of columns in the image.*/
      int nrows; /**< The number of rows in the image.*/
      sAdjRel *A; /**< The adjacency relation of vertices in the graph. */
    };


    /**
     * \brief Defines an image graph where the arc weights between 
     * neighboring pixels p and q are computed as |I(p) - I(q)|+1, 
     * where I(p) is the image intensity at pixel p.
     */
    sImageGraph *ByEuclideanDistance(sImage32 *img, float r);
    /**
     * \brief Defines an image graph where the arc weights between 
     * neighboring pixels p and q are computed as the Euclidean distance 
     * (L2 norm), between the color vectors I(p) and I(q), plus one.
     */
    sImageGraph *ByEuclideanDistance(sCImage *cimg,  float r);
    /**
     * \brief Defines an image graph where the arc weights between 
     * neighboring pixels p and q are computed as the Euclidean distance 
     * (L2 norm), between the color vectors I(p) and I(q), plus one.
     */
    sImageGraph *ByEuclideanDistance(sCImage32f *cimg,  float r);
    
    sImageGraph *ByAccAbsDiff(sImage32 *img, float r, float R);
    sImageGraph *ByAccAbsDiff(sCImage *cimg, float r, float R);

    /**
     * \brief Defines an image graph where the arc weights between 
     * neighboring pixels p and q are computed as W(p) + W(q)+1, 
     * where W is an image of weights in pixel resolution. 
     * W(p) is usually calculated as the magnitude of the gradient 
     * at pixel p.
     */
    sImageGraph *ByWeightImage(sImage32 *W, float r);

    /**
     * \brief Corresponds to the dual CAPACITY version of function 'ByEuclideanDistance'.
     */
    sImageGraph *ByHomogeneityAffinity(sImage32 *img, float r);
    /**
     * \brief Corresponds to the dual CAPACITY version of function 'ByEuclideanDistance'.
     */
    sImageGraph *ByHomogeneityAffinity(sCImage *cimg, float r);
    
    /**
     * \brief Computes the weighted arithmetic mean of the corresponding arcs of two graphs.
     */
    sImageGraph  *WeightedMean(sImageGraph *G1, 
			       sImageGraph *G2,
			       float w2);
    sImageGraph  *ByLevel(sImage32 *obj, int T, float r, int Wmax);
    sImageGraph  *ByExclusion(sImage32 *Pobj,
			      sImage32 *Pbkg,
			      float r,
			      int Pmax);
    sImageGraph  *ByExclusion(sImage32 *Pobj,
			      float r,
			      int Pmax);
    
    /**
     * \brief Performs contrast enhancement by linear stretch of the arc weights.
     */
    void LinearStretch(sImageGraph *sg, 
		       int f1, int f2, 
		       int g1, int g2);

    /**
     * \brief Computes the saliency map of a hierarchy in a given file 
     * containing the history of pixel mergers.
     */
    sImageGraph  *SaliencyMap(char mergehistory[],
			      int ncols, int nrows);
    
    //------ ImageGraph Functions ------------
    /**
     * \brief Returns the index of a given arc (p, q) in the adjacency list of p.
     */
    int           GetArcIndex(sImageGraph *g,
			      int p, int q);
    //int           get_edge_index(int p, int q, 
    //				   sImageGraph *g);
    
    /**
     * \brief A destructor.
     */
    void          Destroy(sImageGraph **g);

    /**
     * \brief A constructor.
     *
     * A utility function that creates an image graph of 'ncols*nrows' vertices.
     */
    sImageGraph  *Create(int ncols, int nrows, 
			 sAdjRel *A);
    /**
     * \brief A copy constructor.
     */
    sImageGraph  *Clone(sImageGraph *g);

    /**
     * \brief Reads an image graph from a txt file on the disc.
     */
    sImageGraph  *ReadFromTxt(char *filename);
    /**
     * \brief Writes an image graph to a txt file on the disc. 
     */
    void          Write2Txt(sImageGraph *sg, 
			    char *filename);

    /**
     * \brief Changes the type of arc weights to DISSIMILARITY or CAPACITY. 
     * By changing the type, the arc weights are complemented/inverted.
     *
     * The arc weights may represent a DISSIMILARITY or a CAPACITY between 
     * neighboring pixels, as generally used by the path-cost functions of 
     * Image Foresting Transform or by the min-cut/max-flow algorithm, respectively.
     */
    void          ChangeType(sImageGraph *sg, 
			     int type);

    /**
     * \brief Replaces a given arc weight with a new value.
     */
    void          ChangeWeight(sImageGraph *sg,
			       int old_val, int new_val);

    /**
     * \brief Applies an increasing transformation to the arc weights to enhance their differences.
     */
    void          Pow(sImageGraph *sg, int power, int max);

    /**
     * \brief Computes the transpose of a directed graph.
     *
     * Transpose of a directed graph G is another directed graph on the 
     * same set of vertices with all of the arcs reversed compared to 
     * the orientation of the corresponding arcs in G. That is, if G 
     * contains an arc (u, v) with weight w(u, v)=k then its transpose G' 
     * contains an arc (v, u) with weight w'(v, u)=k and vice versa.
     */
    void Transpose(sImageGraph *sg);

    /**
     * \brief Generates an image for the approximate visualization of a graph.
     */    
    sImage32 *ArcWeightImage(sImageGraph *sg);
    /**
     * \brief Computes the Khalimsky grid of a saliency map.
     *
     * A saliency map can be visualized through Khalimsky grids, 
     * only when its image graph is given by the 4-adjacency relation.
     */
    sImage32 *KhalimskyGrid(sImageGraph *sg);

    /**
     * \brief Labels the connected components of an undirected graph 
     * considering only the edges with weights in the range [wmin, wmax]. 
     */
    sImage32 *LabelConnectedComponent(sImageGraph *sg,
				      int wmin, int wmax);
    
    /**
     * \brief Updates the weights of the arcs to incorporate the boundary polarity constraint.
     *
     * It is usually used before computing the OIFT 
     * (Oriented Image Foresting Transform).
     */
    void Orient2Digraph(sImageGraph *sg, 
			sImage32 *img,
			float per);
    /**
     * \brief Updates the weights of the arcs to incorporate the Geodesic Star Convexity constraint.
     *
     * It is usually used before computing the OIFT 
     * (Oriented Image Foresting Transform).
     */
    void Orient2DigraphInner(sImageGraph *sg, 
			     sImage32 *P_sum);
    /**
     * \brief Updates the weights of the arcs to incorporate the Geodesic Star Convexity constraint.
     *
     * It is usually used before computing the OIFT 
     * (Oriented Image Foresting Transform).
     */
    void Orient2DigraphOuter(sImageGraph *sg, 
			     sImage32 *P_sum);
    
    /**
     * \brief Updates the arc weights to incorporate the Hedgehog shape constraint.
     */
    void Convert2HedgehogDigraph(sImageGraph *sg,
				 sImage32 *cost,
				 float theta);

    /**
     * \brief Returns the values of the lowest and highest arc weight of the graph.
     */
    void ComputeMinMaxArcs(sImageGraph *sg,
			   int *min, int *max);
    
    //bool IsSymmetricDigraph(sImageGraph *sg);
    /**
     * \brief Tests if the graph is an undirected graph. That is,
     * it is a symmetric digraph and w(p, q) = w(q, p) for all arcs.
     */
    bool IsUndirectedGraph(sImageGraph *sg);
    
  } //end ImageGraph namespace

  typedef ImageGraph::sImageGraph sImageGraph;

} //end gft namespace



#endif

