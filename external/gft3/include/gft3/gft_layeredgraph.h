
#ifndef _GFT_LAYEREDGRAPH_H_
#define _GFT_LAYEREDGRAPH_H_

#include "gft3/gft_common.h"
#include "gft3/gft_stack.h"
#include "gft3/gft_graph.h"
#include "gft3/gft_imagegraph.h"
#include "gft3/gft_scene32.h"
#include "gft3/gft_adjrel3.h"


namespace gft{
  namespace LayeredGraph{

    struct sLayeredGraph {
      int nlayers;
      int nnodesperlayer;
      sGraph *graph;
    };

    sLayeredGraph *Create(int nlayers, int nnodesperlayer);
    sLayeredGraph *Create(int nlayers, int nnodesperlayer, int outdegree);
    void Destroy(sLayeredGraph **lg);

    //2D images:
    void SetArcs(sLayeredGraph *lg, sImageGraph *sg, int l);
    void SetArcs(sLayeredGraph *lg,
		 int l_orig, int l_dest,
		 int ncols,
		 int w, float r);
    void SetArcs(sLayeredGraph *lg,
		 int l_orig, int l_dest,
		 int ncols,
		 int w, float r, float r_neighborhood);

    //3D images:
    void SetArcs(sLayeredGraph *lg, sScene32 *sw, int l);
    void SetArcs(sLayeredGraph *lg, sScene32 *sw,
		 sScene32 *scn, int pol, int l);
    void SetArcs(sLayeredGraph *lg,
		 int l_orig, int l_dest,
		 sScene32 *scn,
		 int w, float r);
    
    //ND images:
    void SetArcs(sLayeredGraph *lg, sGraph *g, int l);

    void SetArcs(sLayeredGraph *lg,
		 int l_orig, int l_dest,
		 int w);
    
    void TransposeLayer(sLayeredGraph *lg, int l);

    void RemoveCuttingEdges(sLayeredGraph *lg, int l,
			    int *label, int lb);

    /**
     * \brief Returns the total number of arcs in the graph.
     */
    int GetNumberOfArcs(sLayeredGraph *lg);

    /**
     * \brief Returns the total number of intra-layer arcs in the graph.
     */
    int GetNumberOfIntraLayerArcs(sLayeredGraph *lg);

    /**
     * \brief Returns the total number of inter-layer arcs in the graph.
     */
    int GetNumberOfInterLayerArcs(sLayeredGraph *lg);

    /**
     * \brief Returns the maximum number of arcs per node in the graph.
     */
    int GetMaxNumberOfArcsPerNode(sLayeredGraph *lg);

    
  } //end LayeredGraph namespace

  typedef LayeredGraph::sLayeredGraph sLayeredGraph;

} //end gft namespace

    
#endif


    
