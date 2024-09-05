
#include "gft_graph.h"


namespace gft{
  namespace Graph{


    sGraph *Create(int nnodes, int outdegree, int *Wnodes){
      sGraph *graph = NULL;
      int i;
      graph = (sGraph *) calloc(1, sizeof(sGraph));
      if(graph == NULL){
	gft::Error((char *)MSG1,(char *)"Graph::Create");
      }
      graph->type = DISSIMILARITY;
      graph->nnodes = nnodes;
      graph->Wnodes = Wnodes;
      graph->nodes = (sGraphNode *) calloc(nnodes, sizeof(sGraphNode));
      if(graph->nodes == NULL){
	gft::Error((char *)MSG1,(char *)"Graph::Create");
      }
      for(i = 0; i < nnodes; i++){
	graph->nodes[i].outdegree = 0;
	graph->nodes[i].arraysize = outdegree;
	graph->nodes[i].id = i;
	graph->nodes[i].adjList = gft::AllocIntArray(outdegree);
	graph->nodes[i].Warcs = gft::AllocIntArray(outdegree);
      }
      return graph;
    }

    
    sGraph *Clone(sGraph *graph){
      sGraph *clone;
      int p,q,i;
      int w;
      clone = Create(graph->nnodes, 10, NULL);
      clone->type = graph->type;
      if(graph->Wnodes != NULL){
	clone->Wnodes = (int *)malloc(sizeof(int)*graph->nnodes);
	for(p = 0; p < graph->nnodes; p++){
	  clone->Wnodes[p] = graph->Wnodes[p];
	}
      }
      for(p = 0; p < graph->nnodes; p++){
	clone->nodes[p].id = graph->nodes[p].id;
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  q = graph->nodes[p].adjList[i];
	  w = graph->nodes[p].Warcs[i];
	  AddDirectedEdge(clone, p, q, w);
	}
      }
      return clone;
    }



    sGraph *Clone(sImageGraph *sg){
      sGraph *clone;
      sAdjRel *A;
      int ncols,nrows;
      int u_x,u_y,v_x,v_y;
      int p,q,i,n;
      int w;
      A     = sg->A;
      ncols = sg->ncols;
      nrows = sg->nrows;
      n = ncols*nrows;
      clone = Create(n, A->n-1, NULL);
      clone->type = sg->type;
      for(p = 0; p < n; p++){
	u_x = p%ncols;
	u_y = p/ncols;

	for(i = 1; i < A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(v_x >= 0 && v_x < ncols && 
	     v_y >= 0 && v_y < nrows){
	    q = v_x + v_y*ncols;
	    w = (sg->n_link[p])[i];
	    AddDirectedEdge(clone, p, q, w);
	  }
	}
      }
      return clone;
    }

    

    sGraph *ByWeightImage(sScene32 *W, float r){
      sGraph *g=NULL;
      sAdjRel3 *A=AdjRel3::Spheric(r);
      int p,q,i;
      int weight;
      int u_x,u_y,u_z,v_x,v_y,v_z;
      
      g = Create(W->n, A->n-1, NULL);
      g->type = DISSIMILARITY;
      
      for(p  =0; p < W->n; p++){
	u_x = gft::Scene32::GetAddressX(W, p);
	u_y = gft::Scene32::GetAddressY(W, p);
	u_z = gft::Scene32::GetAddressZ(W, p);
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->d[i].axis.x;
	  v_y = u_y + A->d[i].axis.y;
	  v_z = u_z + A->d[i].axis.z;	  
	  if(Scene32::IsValidVoxel(W,v_x,v_y,v_z)){
	    q = gft::Scene32::GetVoxelAddress(W, v_x,v_y,v_z);
	    weight = W->data[p] + W->data[q] + 1;
	    //gft::Graph::AddEdge(g, p, q, weight);
	    gft::Graph::AddDirectedEdge(g, p, q, weight);
	  }
	}
      }
      gft::AdjRel3::Destroy(&A);
      return g;
    }




    sGraph *ByEuclideanDistance(sScene32 *scn, float r){
      sGraph *g=NULL;
      sAdjRel3 *A=AdjRel3::Spheric(r);
      int p,q,i;
      int weight;
      int u_x,u_y,u_z,v_x,v_y,v_z;
      
      g = Create(scn->n, A->n-1, NULL);
      g->type = DISSIMILARITY;
      
      for(p  =0; p < scn->n; p++){
	u_x = gft::Scene32::GetAddressX(scn, p);
	u_y = gft::Scene32::GetAddressY(scn, p);
	u_z = gft::Scene32::GetAddressZ(scn, p);
	
	for(i=1; i<A->n; i++){
	  v_x = u_x + A->d[i].axis.x;
	  v_y = u_y + A->d[i].axis.y;
	  v_z = u_z + A->d[i].axis.z;
	  if(Scene32::IsValidVoxel(scn,v_x,v_y,v_z)){
	    q = gft::Scene32::GetVoxelAddress(scn, v_x,v_y,v_z);

	    weight = abs(scn->data[p]-scn->data[q]) + 1;
	    //gft::Graph::AddEdge(g, p, q, weight);
	    gft::Graph::AddDirectedEdge(g, p, q, weight);
	  }
	}
      }
      gft::AdjRel3::Destroy(&A);
      return g;
    }


    

    void Orient2Digraph(sGraph *graph, 
			int *data,
			float per){
      int p,q,i,w;
      float alpha;
      alpha = per/100.0;
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  q = graph->nodes[p].adjList[i];
	  w = graph->nodes[p].Warcs[i];
	  if(w == INT_MAX || w == 0) continue;

	  if(data[p] > data[q])
	    w = MAX(ROUND(w*(1.0 + alpha)), 1);
	  else if(data[p] < data[q])
	    w = MAX(ROUND(w*(1.0 - alpha)), 1);
	  graph->nodes[p].Warcs[i] = w;
	}
      }
    }


    void Orient2DigraphOuter(sGraph *graph,
			     int *P_sum){
      int p,q,i,new_value;
      if(graph->type == DISSIMILARITY)
	new_value = 0;
      else
	new_value = INT_MAX;      

      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  q = graph->nodes[p].adjList[i];
	  if(P_sum[p] == q)
	    graph->nodes[p].Warcs[i] = new_value;
	}
      }
    }
    
    
    sGraph *Transpose(sGraph *graph){
      sGraph *transp;
      int p,q,i;
      int w;
      transp = Create(graph->nnodes, 10, NULL);
      transp->type = graph->type;
      if(graph->Wnodes != NULL){
	transp->Wnodes = (int *)malloc(sizeof(int)*graph->nnodes);
	for(p = 0; p < graph->nnodes; p++){
	  transp->Wnodes[p] = graph->Wnodes[p];
	}
      }
      for(p = 0; p < graph->nnodes; p++){
	transp->nodes[p].id = graph->nodes[p].id;
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  q = graph->nodes[p].adjList[i];
	  w = graph->nodes[p].Warcs[i];
	  AddDirectedEdge(transp, q, p, w);
	}
      }
      return transp;
    }    
    

    void   Destroy(sGraph **graph){
      sGraph *aux;
      int i;
      if(graph != NULL){
	aux = *graph;
	if(aux != NULL){
	  if(aux->nodes != NULL){
	    for(i = 0; i < aux->nnodes; i++){
	      if(aux->nodes[i].adjList != NULL)
		gft::FreeIntArray(&aux->nodes[i].adjList);
	      if(aux->nodes[i].Warcs != NULL)
		gft::FreeIntArray(&aux->nodes[i].Warcs);
	    }
	    free(aux->nodes);
	  }
	  free(aux);
	  *graph = NULL;
	}
      }
    }


    int GetNumberOfArcs(sGraph *graph){
      int p, Narcs = 0;
      for(p = 0; p < graph->nnodes; p++)
	Narcs += graph->nodes[p].outdegree;
      return Narcs;
    }


    int GetArcWeight(sGraph *graph, int src, int dest){
      sGraphNode *s;
      s = &graph->nodes[src];
      int n,i;
      n = s->outdegree;
      for(i = 0; i < n; i++){
	if(s->adjList[i] == dest){
	  //printf("s->Warcs %f\n",s->Warcs[i]);
	  return s->Warcs[i];
	}
      }
      gft::Warning((char *)"Invalid node", (char *)"Graph::GetArcWeight");
      return -1;
    }


    int GetArcIndex(sGraph *graph, int src, int dest){
      sGraphNode *s;
      s = &graph->nodes[src];
      int n,i;
      n = s->outdegree;
      for(i = 0; i < n; i++){
	if(s->adjList[i] == dest){
	  return i;
	}
      }
      return NIL;
    }

    
    void AddEdge(sGraph *graph, int src, int dest, int w){
      AddDirectedEdge(graph, src,  dest, w);
      AddDirectedEdge(graph, dest, src,  w);  
    }
    
    
    void AddDirectedEdge(sGraph *graph, int src, int dest, int w){
      sGraphNode *s;
      s = &graph->nodes[src];
      s->outdegree++;
      if(s->outdegree > s->arraysize){
	s->adjList = (int *)realloc(s->adjList, sizeof(int)*s->outdegree);
	if(s->adjList == NULL)
	  gft::Error((char *)MSG1,(char *)"Graph::AddDirectedEdge");
	s->Warcs = (int *)realloc(s->Warcs, sizeof(int)*s->outdegree);
	if(s->Warcs == NULL)
	  gft::Error((char *)MSG1,(char *)"Graph::AddDirectedEdge");
	s->arraysize = s->outdegree;
      }
      s->adjList[s->outdegree-1] = dest;
      s->Warcs[s->outdegree-1] = w;
      //printf("W_graph %f\n",s->Warcs[s->outdegree-1]);
    }
    
    
    void UpdateEdge(sGraph *graph, int src, int dest, int w){
      UpdateDirectedEdge(graph, src,  dest, w);
      UpdateDirectedEdge(graph, dest, src,  w);  
    }
    

    void UpdateDirectedEdge(sGraph *graph, int src, int dest, int w){
      sGraphNode *s;
      int i, find = false;
      s = &graph->nodes[src];

      for(i = 0; i < s->outdegree; i++){
	if(dest == s->adjList[i]){
	  s->Warcs[i] = w;
	  find = true;
	  break;
	}
      }
      if(!find)
	AddDirectedEdge(graph, src, dest, w);
    }


    void UpdateDirectedEdgeIfHigher(sGraph *graph, int src, int dest, int w){
      sGraphNode *s;
      int i, find = false;
      s = &graph->nodes[src];
      for(i = 0; i < s->outdegree; i++){
	if(dest == s->adjList[i]){
	  if(s->Warcs[i] < w)
	    s->Warcs[i] = w;
	  find = true;
	  break;
	}
      }
      if(!find)
	AddDirectedEdge(graph, src, dest, w);
    }


    void AddDirectedEdgeIfNonexistent(sGraph *graph, int src, int dest, int w){
      sGraphNode *s;
      int i, find = false;
      s = &graph->nodes[src];
      for(i = 0; i < s->outdegree; i++){
	if(dest == s->adjList[i]){
	  find = true;
	  break;
	}
      }
      if(!find)
	AddDirectedEdge(graph, src, dest, w);
    }


    void RemoveEdge(sGraph *graph, int src, int dest){
      RemoveDirectedEdge(graph, src,  dest);
      RemoveDirectedEdge(graph, dest, src);
    }

    
    void RemoveDirectedEdge(sGraph *graph, int src, int dest){
      sGraphNode *s;
      int i,j;
      bool find = false;
      s = &graph->nodes[src];
      for(i = 0; i < s->outdegree; i++){
	if(s->adjList[i] == dest){
	  j = i;
	  find = true;
	  break;
	}
      }
      if(find){
	s->adjList[j] = s->adjList[s->outdegree-1];
	s->Warcs[j] = s->Warcs[s->outdegree-1];
	s->outdegree--;
      }
    }
    

    int GetMaximumArc(sGraph *graph){
      int w,Wmax = INT_MIN;
      int p,i;
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  w = graph->nodes[p].Warcs[i];
	  if(w > Wmax && w != INT_MAX)
	    Wmax = w;
	}
      }
      return Wmax;
    }


    int GetMinimumArc(sGraph *graph){
      int w,Wmin = INT_MAX;
      int p,i;
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  w = graph->nodes[p].Warcs[i];
	  if(w < Wmin && w != INT_MIN)
	    Wmin = w;
	}
      }
      return Wmin;
    }
    

    void ChangeWeight(sGraph *graph,
		      int old_val, int new_val){
      int p,i;
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  if(graph->nodes[p].Warcs[i] == old_val)
	    graph->nodes[p].Warcs[i] = new_val;
	}
      }
    }
    

    // Increasing transformation.
    void Pow(sGraph *graph,
	     int power, int max){
      int w,Wmax = GetMaximumArc(graph);
      int p,i;
      long double v,m;
      if(Wmax<=1)  return;
      if(power<=0) return;
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  w = graph->nodes[p].Warcs[i];
	  if(w == INT_MAX || w == 0) continue;
	  v = (double)w;
	  m = powl((v/Wmax),power);
	  graph->nodes[p].Warcs[i] = MAX(ROUND(m*max), 1);
	}
      }
    }

 
    
    int Tarjan_aux(sGraph *graph, int *index, int *lowlink, int *label,
		   int i, int id, int *lb){
      //pilhas usadas para simular a recursao:
      sStack *S_i = NULL;
      sStack *S_j = NULL;
      //pilha do algoritmo de Tarjan:
      sStack *S = NULL;
      int *IsInS = NULL;
      int j, t, k, tmp, flag = 0;
      enum action {f_in, f_out, f_end} act;

      IsInS = gft::AllocIntArray(graph->nnodes);
      S_i = gft::Stack::Create(graph->nnodes);
      S_j = gft::Stack::Create(graph->nnodes);
      S   = gft::Stack::Create(graph->nnodes);
      act = f_in;
      //---------------------------
      j = 0;
      gft::Stack::Push(S_i, i);
      gft::Stack::Push(S_j, j);
      index[i] = id;
      lowlink[i] = id;
      id++;
      gft::Stack::Push(S, i);
      IsInS[i] = 1;

      //--------------------------
      do{
	switch(act){
	case f_in:
	  if(j < graph->nodes[i].outdegree){
	    k = graph->nodes[i].adjList[j];
	    j = 0;
	    i = k;
	  }
	  else{
	    act = f_out;
	    break;
	  }

	  if(index[i] != NIL){
	    flag = 1;
	    if(IsInS[i]){
	      //node.lowlink = min(node.lowlink, n.index);
	      tmp = gft::Stack::Pop(S_i);
	      if(lowlink[tmp] > index[i])
		lowlink[tmp] = index[i];
	      gft::Stack::Push(S_i, tmp);
	    }
	    act = f_out;
	  }
	  else{
	    index[i] = id;
	    lowlink[i] = id;
	    id++;
	    gft::Stack::Push(S, i);
	    IsInS[i] = 1;
	    gft::Stack::Push(S_i, i);
	    gft::Stack::Push(S_j, j);
	  }
	  break;
	case f_out:
	  if(gft::Stack::IsEmpty(S_i))
	    act = f_end;
	  else{
	    tmp = lowlink[i];
	    i = gft::Stack::Pop(S_i);
	    j = gft::Stack::Pop(S_j);

	    if(lowlink[i] > tmp && !flag)
	      lowlink[i] = tmp;

	    flag = 0;
	    j++;
	    if(j < graph->nodes[i].outdegree){
	      gft::Stack::Push(S_i, i);
	      gft::Stack::Push(S_j, j);
	      act = f_in;
	    }
	    else{
	      //if we are in the root of the component
	      if(lowlink[i] == index[i]){
		do{
		  if(gft::Stack::IsEmpty(S))
		    break;
		  t = gft::Stack::Pop(S);
		  IsInS[t] = 0;
		  label[t] = *lb;
		}while(t != i);
		(*lb)++;
	      }
	    }
	  }
	  break;
	case f_end:
	  break;
	}
      }while(act != f_end);

      gft::FreeIntArray(&IsInS);
      gft::Stack::Destroy(&S_i);
      gft::Stack::Destroy(&S_j);
      gft::Stack::Destroy(&S);
      return id;
    }

    
    /*Tarjan's algorithm is a procedure for finding strongly connected components of a directed graph.*/
    int *Tarjan(sGraph *graph){
      int *index = NULL;
      int *lowlink = NULL;
      int *label = NULL;
      int i,id = 0,lb = 1;

      index   = gft::AllocIntArray(graph->nnodes);
      lowlink = gft::AllocIntArray(graph->nnodes);
      label   = gft::AllocIntArray(graph->nnodes);
      for(i = 0; i < graph->nnodes; i++)
	index[i] = NIL;
      	
      for(i = 0; i < graph->nnodes; i++){
	if(index[i] != NIL) continue;
	id = Tarjan_aux(graph, index, lowlink, label, i, id, &lb);
      }

      gft::FreeIntArray(&index);
      gft::FreeIntArray(&lowlink);
      return label;
    }


    int *Tarjan(sGraph *graph, int p){
      int *index = NULL;
      int *lowlink = NULL;
      int *label = NULL;
      int id = 0,lb = 1, i;

      index   = gft::AllocIntArray(graph->nnodes);
      lowlink = gft::AllocIntArray(graph->nnodes);
      label   = gft::AllocIntArray(graph->nnodes);
      for(i = 0; i < graph->nnodes; i++)
	index[i] = NIL;

      id = Tarjan_aux(graph, index, lowlink, label, p, id, &lb);

      lb = label[p];
      for(i = 0; i < graph->nnodes; i++){
	if(label[i] != lb)
	  label[i] = 0;
	else
	  label[i] = 1;
      }
      
      gft::FreeIntArray(&index);
      gft::FreeIntArray(&lowlink);
      return label;
    }


    int *Tarjan(sGraph *graph, int *V, int n){
      int *index = NULL;
      int *lowlink = NULL;
      int *label = NULL;
      int i,id = 0,lb = 1;

      index   = gft::AllocIntArray(graph->nnodes);
      lowlink = gft::AllocIntArray(graph->nnodes);
      label   = gft::AllocIntArray(graph->nnodes);
      for(i = 0; i < graph->nnodes; i++)
	index[i] = NIL;
      	
      for(i = 0; i < n; i++){
	if(index[V[i]] != NIL) continue;
	id = Tarjan_aux(graph, index, lowlink, label, V[i], id, &lb);
      }

      //----------------------------
      int *lb_map;
      int Lmax = 0,l;
      for(i = 0; i < graph->nnodes; i++){
	if(label[i] == NIL)
	  label[i] = 0;
	if(label[i] > Lmax)
	  Lmax = label[i];
      }
      lb_map = gft::AllocIntArray(Lmax+1);

      for(i = 0; i < n; i++)
	lb_map[label[V[i]]] = 1;
      
      l = 0;
      for(i = 1; i < Lmax+1; i++){
	if(lb_map[i] > 0){
	  l++;
	  lb_map[i] = l;
	}
      }

      for(i = 0; i < graph->nnodes; i++)
	label[i] = lb_map[label[i]];

      gft::FreeIntArray(&lb_map);
      //----------------------------      

      gft::FreeIntArray(&index);
      gft::FreeIntArray(&lowlink);
      return label;
    }


    void ChangeType(sGraph *graph, int type){
      int w, Wmax;
      int p, i;
      if(graph->type==type)
	return;

      Wmax = INT_MAX;
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  w = graph->nodes[p].Warcs[i];
	  if(w == 0 || w == INT_MAX) continue;
	  if(w > Wmax)
	    Wmax = w;
	}
      }
      for(p = 0; p < graph->nnodes; p++){
	for(i = 0; i < graph->nodes[p].outdegree; i++){
	  w = graph->nodes[p].Warcs[i];
	  if(w == INT_MAX)
	    graph->nodes[p].Warcs[i] = 0;
	  else if(w == 0)
	    graph->nodes[p].Warcs[i] = INT_MAX;
	  else
	    graph->nodes[p].Warcs[i] = Wmax - w + 1;
	}
      }
      if(graph->type==DISSIMILARITY)
	graph->type = CAPACITY;
      else
	graph->type = DISSIMILARITY;
    }


    void HedgehogDigraph(sGraph *graph,
			 sImage32 *cost,
			 float theta,
			 float r){
      sImage32 *gradx=NULL,*grady=NULL;
      sKernel *Kx,*Ky;
      sAdjRel *A;
      int ncols,nrows;
      int u_x,u_y,v_x,v_y;
      int i,p,q,n,q_min = 0;
      float mag,pq_x,pq_y,cosine;
      float inner_product1, inner_product2, min_inner_product;
      float Dd_px,Dd_py,Dd_qx,Dd_qy;
      float *Dpq;
      bool found;
      int newvalue_1, newvalue_2;
      if(graph->type == DISSIMILARITY){
	newvalue_1 = 0;
	newvalue_2 = INT_MAX;
      }
      else{
	newvalue_1 = INT_MAX;
	newvalue_2 = 0;
      }
      A = AdjRel::Circular(r);
      ncols = cost->ncols;
      nrows = cost->nrows;
      n = ncols*nrows;

      cosine = cosf(theta*(PI/180.0) + PI/2.0);
      //--------------------
      Dpq = (float *)malloc(A->n*sizeof(float));
      for(i = 1; i < A->n; i++){
	Dpq[i] = sqrtf(SQUARE(A->dx[i]) + SQUARE(A->dy[i]));
      }
      //--------------------

      /*
      if(sg->type == DISSIMILARITY)
	newval = 0;
      else
	newval = sg->Wmax;
      */
      
      Ky = Kernel::Make("3,3,-1.0,-2.0,-1.0,0.0,0.0,0.0,1.0,2.0,1.0");
      Kx = Kernel::Make("3,3,-1.0,0.0,1.0,-2.0,0.0,2.0,-1.0,0.0,1.0");
      gradx = LinearFilter(cost, Kx);
      grady = LinearFilter(cost, Ky);

      for(p = 0; p < n; p++){
	u_x = p%ncols;
	u_y = p/ncols;

	Dd_px = gradx->data[p];
	Dd_py = grady->data[p];
	mag = sqrtf(SQUARE(Dd_px) + SQUARE(Dd_py));
	if(mag != 0.0){
	  Dd_px /= mag;
	  Dd_py /= mag;
	}

	found = false;
	min_inner_product = 2.0;
	for(i = 1; i < A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(v_x >= 0 && v_x < ncols && 
	     v_y >= 0 && v_y < nrows){
	    q = v_x + v_y*ncols;

	    Dd_qx = gradx->data[q];
	    Dd_qy = grady->data[q];
	    mag = sqrtf(SQUARE(Dd_qx) + SQUARE(Dd_qy));
	    if(mag != 0.0){
	      Dd_qx /= mag;
	      Dd_qy /= mag;
	    }
	    
	    pq_x = A->dx[i]/Dpq[i];
	    pq_y = A->dy[i]/Dpq[i];
	    inner_product1 = Dd_px * pq_x + Dd_py * pq_y;
	    inner_product2 = Dd_qx * pq_x + Dd_qy * pq_y;

	    if(inner_product1 < min_inner_product){
	      min_inner_product = inner_product1;
	      q_min = q;
	    }
	    
	    if(inner_product1 <= cosine or inner_product2 <= cosine){
	      UpdateDirectedEdge(graph, p, q, newvalue_1);
	      AddDirectedEdgeIfNonexistent(graph, q, p, newvalue_2);
	      found = true;
	    }
	  }
	}

	if(!found){
	  UpdateDirectedEdge(graph, p, q_min, newvalue_1);
	  AddDirectedEdgeIfNonexistent(graph, q_min, p, newvalue_2);
	}
      }
      
      free(Dpq);
      Destroy(&gradx);
      Destroy(&grady);
      Kernel::Destroy(&Kx);
      Kernel::Destroy(&Ky);
      AdjRel::Destroy(&A);
    }



    void LocalBandReduction(sGraph **graph,
			    sImage32 *cost,
			    int delta){
      LocalBandReduction(graph, cost->data, delta);
    }



    void LocalBandReduction(sGraph **graph,
			    sScene32 *cost,
			    int delta){
      LocalBandReduction(graph, cost->data, delta);
    }
   

    void LocalBandReduction(sGraph **graph,
			    int *cost,
			    int delta){
      int p,q,w,wtp,i,cmin,s,t;
      int value_1, value_2;
      gft::sGraph *g,*ng;
      gft::sStack *S,*R;
      gft::sBMap *B,*T;
      if(graph == NULL) return;
      g = *graph;
      S = gft::Stack::Create(g->nnodes);
      R = gft::Stack::Create(g->nnodes);
      B = gft::BMap::Create(g->nnodes);
      T = gft::BMap::Create(g->nnodes);
      if(g->type == DISSIMILARITY){
	value_1 = 0;
	value_2 = INT_MAX;
      }
      else{
	value_1 = INT_MAX;
	value_2 = 0;
      }
      
      for(p = 0; p < g->nnodes; p++){
	cmin = INT_MAX;
	for(i = 0; i < g->nodes[p].outdegree; i++){
	  q = g->nodes[p].adjList[i];
	  w = g->nodes[p].Warcs[i];
	  if(w == value_1){
	    gft::Stack::Push(S, q);
	    gft::BMap::Set1(B, q);
	    gft::BMap::Set1(T, q);
	    cmin = MIN(cmin, cost[q]);
	  }
	}
	while(!gft::Stack::IsEmpty(S)){
	  s = gft::Stack::Pop(S);
	  gft::Stack::Push(R, s);
	  for(i = 0; i < g->nodes[s].outdegree; i++){
	    t = g->nodes[s].adjList[i];
	    w = g->nodes[s].Warcs[i];
	    if(w == value_1){
	      if(gft::BMap::Get(T, t) == 1){
		wtp = gft::Graph::GetArcWeight(g, t, p);
		if(wtp == value_2)
		  gft::Graph::UpdateEdge(g, p, t, NIL);
	      }
	      else if(cost[t] >= cmin + delta && gft::BMap::Get(B, t) == 0){
		gft::Stack::Push(S, t);
		gft::BMap::Set1(B, t);
	      }
	    }
	  }
	}
	while(!gft::Stack::IsEmpty(R)){
	  s = gft::Stack::Pop(R);
	  gft::BMap::Set0(B, s);
	  gft::BMap::Set0(T, s);
	}
      }

      ng = Create(g->nnodes, 10, NULL);
      ng->type = g->type;
      if(g->Wnodes != NULL){
	ng->Wnodes = (int *)malloc(sizeof(int)*g->nnodes);
	for(p = 0; p < g->nnodes; p++)
	  ng->Wnodes[p] = g->Wnodes[p];
      }
      for(p = 0; p < g->nnodes; p++){
	ng->nodes[p].id = g->nodes[p].id;
	for(i = 0; i < g->nodes[p].outdegree; i++){
	  q = g->nodes[p].adjList[i];
	  w = g->nodes[p].Warcs[i];
	  if(w != NIL)
	    AddDirectedEdge(ng, p, q, w);
	}
      }
      gft::Graph::Destroy(&g);
      *graph = ng;
      gft::Stack::Destroy(&S);
      gft::Stack::Destroy(&R);
      gft::BMap::Destroy(&B);
      gft::BMap::Destroy(&T);
    }


    
    
    void LocalBandConstraint(sGraph *graph,
			     sImage32 *cost,
			     int delta,
			     float r){
      sAdjRel *A;
      int ncols,nrows;
      int u_x,u_y,v_x,v_y;
      int i,p,q,n;
      int newvalue_1, newvalue_2;
      if(graph->type == DISSIMILARITY){
	newvalue_1 = 0;
	newvalue_2 = INT_MAX;
      }
      else{
	newvalue_1 = INT_MAX;
	newvalue_2 = 0;
      }
      
      A = AdjRel::Circular(r);
      ncols = cost->ncols;
      nrows = cost->nrows;
      n = ncols*nrows;

      for(p = 0; p < n; p++){
	u_x = p%ncols;
	u_y = p/ncols;

	for(i = 1; i < A->n; i++){
	  v_x = u_x + A->dx[i];
	  v_y = u_y + A->dy[i];
	  if(v_x >= 0 && v_x < ncols && 
	     v_y >= 0 && v_y < nrows){
	    q = v_x + v_y*ncols;
	    if(cost->data[p] >= cost->data[q] + delta){
	      UpdateDirectedEdge(graph, p, q, newvalue_1);
	      AddDirectedEdgeIfNonexistent(graph, q, p, newvalue_2);	      
	    }
	  }
	}
      }
      AdjRel::Destroy(&A);
    }




    void LocalBandConstraint(sGraph *graph,
			     sScene32 *cost,
			     int delta,
			     float r){
      sAdjRel3 *A;
      int u_x,u_y,u_z,v_x,v_y,v_z;
      int i,p,q;
      int newvalue_1, newvalue_2;
      if(graph->type == DISSIMILARITY){
	newvalue_1 = 0;
	newvalue_2 = INT_MAX;
      }
      else{
	newvalue_1 = INT_MAX;
	newvalue_2 = 0;
      }
      
      A = AdjRel3::Spheric(r);

      for(p = 0; p < cost->n; p++){
	u_x = gft::Scene32::GetAddressX(cost, p);
	u_y = gft::Scene32::GetAddressY(cost, p);
	u_z = gft::Scene32::GetAddressZ(cost, p);

	for(i = 1; i < A->n; i++){
	  v_x = u_x + A->d[i].axis.x;
	  v_y = u_y + A->d[i].axis.y;
	  v_z = u_z + A->d[i].axis.z;	  
	  if(v_x >= 0 && v_x < cost->xsize && 
	     v_y >= 0 && v_y < cost->ysize &&
	     v_z >= 0 && v_z < cost->zsize){
	    q = gft::Scene32::GetVoxelAddress(cost, v_x, v_y, v_z);
	    if(cost->data[p] >= cost->data[q] + delta){
	      UpdateDirectedEdge(graph, p, q, newvalue_1);
	      AddDirectedEdgeIfNonexistent(graph, q, p, newvalue_2);
	    }
	  }
	}
      }
      AdjRel3::Destroy(&A);
    }

    


    /*Region Adjacency Graph (RAG)*/
    sGraph *RAG(sCImage *cimg,
		sImage32 *label){
      sAdjRel *A;
      sGraph *graph = NULL;
      float *color[3];
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      int *size;
      float w;
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      color[0] = gft::AllocFloatArray(nnodes);
      color[1] = gft::AllocFloatArray(nnodes);
      color[2] = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(color[0] == NULL ||
	 color[1] == NULL ||
	 color[2] == NULL ||
	 size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	color[0][l] += cimg->C[0]->data[p];
	color[1][l] += cimg->C[1]->data[p];
	color[2][l] += cimg->C[2]->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0){
	  color[0][l] /= size[l];
	  color[1][l] /= size[l];
	  color[2][l] /= size[l];
	}
	else{
	  printf("Empty regions in RAG\n");
	}
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Neighborhood_4();
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = sqrtf(SQUARE(color[0][lp] - color[0][lq]) +
			SQUARE(color[1][lp] - color[1][lq]) +
			SQUARE(color[2][lp] - color[2][lq]));
	      gft::Graph::UpdateEdge(graph, lp, lq, ROUND(w));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&color[0]);
      gft::FreeFloatArray(&color[1]);
      gft::FreeFloatArray(&color[2]);
      gft::AdjRel::Destroy(&A);
      return graph;
    }



    /*Region Adjacency Graph (RAG)*/
    sGraph *RAG_lab(sCImage *cimg,
		    sImage32 *label){
      sCImage32f *cimg_lab;
      sAdjRel *A;
      sGraph *graph = NULL;
      float *color[3];
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      int *size;
      float w,wpq,wqp;
      cimg_lab = CImage32f::RGB2Lab(cimg);
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      color[0] = gft::AllocFloatArray(nnodes);
      color[1] = gft::AllocFloatArray(nnodes);
      color[2] = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(color[0] == NULL ||
	 color[1] == NULL ||
	 color[2] == NULL ||
	 size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	color[0][l] += cimg_lab->C[0]->data[p];
	color[1][l] += cimg_lab->C[1]->data[p];
	color[2][l] += cimg_lab->C[2]->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0){
	  color[0][l] /= size[l];
	  color[1][l] /= size[l];
	  color[2][l] /= size[l];
	}
	else{
	  printf("Empty regions in RAG\n");
	}
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Neighborhood_4();
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;

	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = sqrtf(SQUARE(color[0][lp] - color[0][lq]) +
			SQUARE(color[1][lp] - color[1][lq]) +
			SQUARE(color[2][lp] - color[2][lq]));
	      wpq = wqp = w;
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&color[0]);
      gft::FreeFloatArray(&color[1]);
      gft::FreeFloatArray(&color[2]);
      gft::AdjRel::Destroy(&A);
      gft::CImage32f::Destroy(&cimg_lab);
      return graph;
    }



    /*Region Adjacency Graph (RAG)*/
    sGraph *RAG_lab(sCImage *cimg,
		    sImage32 *label,
		    int c1,
		    int c2,
		    float polarity){
      sCImage32f *cimg_lab;
      sAdjRel *A;
      sGraph *graph = NULL;
      float *color[3];
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      int *size;
      float w,wpq,wqp,wc1c2,wc1p,wc2p,wc1q,wc2q;
      double l1,a1,b1;
      double l2,a2,b2;
      cimg_lab = CImage32f::RGB2Lab(cimg);
      gft::Color::RGB2Lab(gft::Color::Channel0(c1),
			  gft::Color::Channel1(c1),
			  gft::Color::Channel2(c1),
			  l1, a1, b1);
      gft::Color::RGB2Lab(gft::Color::Channel0(c2),
			  gft::Color::Channel1(c2),
			  gft::Color::Channel2(c2),
			  l2, a2, b2);
      wc1c2 = sqrtf(SQUARE(a1 - a2) +
		    SQUARE(b1 - b2));
      wc1c2 /= 2.0;
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      color[0] = gft::AllocFloatArray(nnodes);
      color[1] = gft::AllocFloatArray(nnodes);
      color[2] = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(color[0] == NULL ||
	 color[1] == NULL ||
	 color[2] == NULL ||
	 size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	color[0][l] += cimg_lab->C[0]->data[p];
	color[1][l] += cimg_lab->C[1]->data[p];
	color[2][l] += cimg_lab->C[2]->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0){
	  color[0][l] /= size[l];
	  color[1][l] /= size[l];
	  color[2][l] /= size[l];
	}
	else{
	  printf("Empty regions in RAG\n");
	}
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Neighborhood_4();
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;

	wc1p = sqrtf(SQUARE(a1 - color[1][lp]) +
		     SQUARE(b1 - color[2][lp]));
	wc2p = sqrtf(SQUARE(a2 - color[1][lp]) +
		     SQUARE(b2 - color[2][lp]));

	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = sqrtf(SQUARE(color[0][lp] - color[0][lq]) +
			SQUARE(color[1][lp] - color[1][lq]) +
			SQUARE(color[2][lp] - color[2][lq]));

	      wc1q = sqrtf(SQUARE(a1 - color[1][lq]) +
			   SQUARE(b1 - color[2][lq]));
	      wc2q = sqrtf(SQUARE(a2 - color[1][lq]) +
			   SQUARE(b2 - color[2][lq]));
	      
	      if(wc1p + wc2q < 2.0*wc1c2){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(wc1q + wc2p < 2.0*wc1c2){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w*(1.0 - polarity);
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&color[0]);
      gft::FreeFloatArray(&color[1]);
      gft::FreeFloatArray(&color[2]);
      gft::AdjRel::Destroy(&A);
      gft::CImage32f::Destroy(&cimg_lab);
      return graph;
    }



    /*Region Adjacency Graph (RAG)*/
    sGraph *RAG_lab(sCImage *cimg,
		    sImage32 *label,
		    int c1,
		    int c2,
		    float polarity,
		    float R){
      sCImage32f *cimg_lab;
      sAdjRel *A;
      sGraph *graph = NULL;
      float *color[3];
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      int *size;
      float w,wpq,wqp,wc1c2,wc1p,wc2p,wc1q,wc2q;
      double l1,a1,b1;
      double l2,a2,b2;
      cimg_lab = CImage32f::RGB2Lab(cimg);
      gft::Color::RGB2Lab(gft::Color::Channel0(c1),
			  gft::Color::Channel1(c1),
			  gft::Color::Channel2(c1),
			  l1, a1, b1);
      gft::Color::RGB2Lab(gft::Color::Channel0(c2),
			  gft::Color::Channel1(c2),
			  gft::Color::Channel2(c2),
			  l2, a2, b2);
      wc1c2 = sqrtf(SQUARE(a1 - a2) +
		    SQUARE(b1 - b2));
      wc1c2 /= 2.0;
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      color[0] = gft::AllocFloatArray(nnodes);
      color[1] = gft::AllocFloatArray(nnodes);
      color[2] = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(color[0] == NULL ||
	 color[1] == NULL ||
	 color[2] == NULL ||
	 size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	color[0][l] += cimg_lab->C[0]->data[p];
	color[1][l] += cimg_lab->C[1]->data[p];
	color[2][l] += cimg_lab->C[2]->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0){
	  color[0][l] /= size[l];
	  color[1][l] /= size[l];
	  color[2][l] /= size[l];
	}
	else{
	  printf("Empty regions in RAG\n");
	}
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Circular(R);
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;

	wc1p = sqrtf(SQUARE(a1 - color[1][lp]) +
		     SQUARE(b1 - color[2][lp]));
	wc2p = sqrtf(SQUARE(a2 - color[1][lp]) +
		     SQUARE(b2 - color[2][lp]));

	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = sqrtf(SQUARE(color[0][lp] - color[0][lq]) +
			SQUARE(color[1][lp] - color[1][lq]) +
			SQUARE(color[2][lp] - color[2][lq]));

	      wc1q = sqrtf(SQUARE(a1 - color[1][lq]) +
			   SQUARE(b1 - color[2][lq]));
	      wc2q = sqrtf(SQUARE(a2 - color[1][lq]) +
			   SQUARE(b2 - color[2][lq]));
	      
	      if(wc1p + wc2q < 2.0*wc1c2){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(wc1q + wc2p < 2.0*wc1c2){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w*(1.0 - polarity);
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&color[0]);
      gft::FreeFloatArray(&color[1]);
      gft::FreeFloatArray(&color[2]);
      gft::AdjRel::Destroy(&A);
      gft::CImage32f::Destroy(&cimg_lab);
      return graph;
    }


    /*Region Adjacency Graph (RAG)*/
    sGraph *RAG_lab(sCImage *cimg,
		    sImage32 *label,
		    float polarity){
      sCImage32f *cimg_lab;
      sAdjRel *A;
      sGraph *graph = NULL;
      float *color[3];
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      int *size;
      float w,wpq,wqp;
      cimg_lab = CImage32f::RGB2Lab(cimg);
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      color[0] = gft::AllocFloatArray(nnodes);
      color[1] = gft::AllocFloatArray(nnodes);
      color[2] = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(color[0] == NULL ||
	 color[1] == NULL ||
	 color[2] == NULL ||
	 size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	color[0][l] += cimg_lab->C[0]->data[p];
	color[1][l] += cimg_lab->C[1]->data[p];
	color[2][l] += cimg_lab->C[2]->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0){
	  color[0][l] /= size[l];
	  color[1][l] /= size[l];
	  color[2][l] /= size[l];
	}
	else{
	  printf("Empty regions in RAG\n");
	}
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Neighborhood_4();
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;

	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = sqrtf(SQUARE(color[0][lp] - color[0][lq]) +
			SQUARE(color[1][lp] - color[1][lq]) +
			SQUARE(color[2][lp] - color[2][lq]));
	      if(color[0][lp] > color[0][lq]){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(color[0][lp] < color[0][lq]){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w;
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&color[0]);
      gft::FreeFloatArray(&color[1]);
      gft::FreeFloatArray(&color[2]);
      gft::AdjRel::Destroy(&A);
      gft::CImage32f::Destroy(&cimg_lab);
      return graph;
    }



    /*Region Adjacency Graph (RAG)*/
    sGraph *RAG_lab(sCImage *cimg,
		    sImage32 *label,
		    float polarity,
		    float R){
      sCImage32f *cimg_lab;
      sAdjRel *A;
      sGraph *graph = NULL;
      float *color[3];
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      int *size;
      float w,wpq,wqp;
      cimg_lab = CImage32f::RGB2Lab(cimg);
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      color[0] = gft::AllocFloatArray(nnodes);
      color[1] = gft::AllocFloatArray(nnodes);
      color[2] = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(color[0] == NULL ||
	 color[1] == NULL ||
	 color[2] == NULL ||
	 size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	color[0][l] += cimg_lab->C[0]->data[p];
	color[1][l] += cimg_lab->C[1]->data[p];
	color[2][l] += cimg_lab->C[2]->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0){
	  color[0][l] /= size[l];
	  color[1][l] /= size[l];
	  color[2][l] /= size[l];
	}
	else{
	  printf("Empty regions in RAG\n");
	}
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Circular(R);
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;

	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = sqrtf(SQUARE(color[0][lp] - color[0][lq]) +
			SQUARE(color[1][lp] - color[1][lq]) +
			SQUARE(color[2][lp] - color[2][lq]));
	      if(color[0][lp] > color[0][lq]){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(color[0][lp] < color[0][lq]){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w;
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&color[0]);
      gft::FreeFloatArray(&color[1]);
      gft::FreeFloatArray(&color[2]);
      gft::AdjRel::Destroy(&A);
      gft::CImage32f::Destroy(&cimg_lab);
      return graph;
    }

    
    
    sGraph *RAG(sImage32 *img,
		sImage32 *label){
      sAdjRel *A;
      sGraph *graph = NULL;
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      float *value = NULL;
      int *size;
      float w;
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      value = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(value == NULL || size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	value[l] += img->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0)
	  value[l] /= size[l];
	else
	  printf("Empty regions in RAG\n");
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Neighborhood_4();
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = fabsf(value[lp] - value[lq]);
	      gft::Graph::UpdateEdge(graph, lp, lq, ROUND(w));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&value);
      gft::AdjRel::Destroy(&A);
      return graph;
    }


    
    sGraph *RAG(sImage32 *img,
		sImage32 *label,
		float polarity){
      sAdjRel *A;
      sGraph *graph = NULL;
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      float *value = NULL;
      int *size;
      float w,wpq,wqp;
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      value = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(value == NULL || size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	value[l] += img->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0)
	  value[l] /= size[l];
	else
	  printf("Empty regions in RAG\n");
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Neighborhood_4();
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = fabsf(value[lp] - value[lq]);
	      if(value[lp] > value[lq]){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(value[lp] < value[lq]){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w;
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&value);
      gft::AdjRel::Destroy(&A);
      return graph;
    }



    sGraph *RAG(sImage32 *img,
		sImage32 *label,
		float polarity,
		float R){
      sAdjRel *A;
      sGraph *graph = NULL;
      int nnodes, p, l, q, i, px, py, qx, qy, lp, lq;
      float *value = NULL;
      int *size;
      float w,wpq,wqp;
      nnodes = gft::Image32::GetMaxVal(label) + 1;
      value = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(value == NULL || size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	value[l] += img->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0)
	  value[l] /= size[l];
	else
	  printf("Empty regions in RAG\n");
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel::Circular(R);
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      w = fabsf(value[lp] - value[lq]);
	      if(value[lp] > value[lq]){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(value[lp] < value[lq]){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w;
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&value);
      gft::AdjRel::Destroy(&A);
      return graph;
    }

    

    /*
    sGraph *RAG(sImage32 *img,
		sImage32 *label,
		float polarity,
		float curvature_R,
		float exponent){
      gft::sGraph *G;
      gft::sAdjRel *A;
      double *curv;
      double **sum_curv_x;
      double **sum_curv_y;
      double curv_pq, curv_pq_x, curv_pq_y, mean_weight, c;
      int **n_curv;
      int p, q, n, w;
      int i, j, k, px, py, qx, qy, lp, lq, nlqs;
      int *lqs;
      bool found;
      double *V_curv_x, *V_curv_y;
      G = RAG(img, label, polarity);
      curv = gft::LLW::Curvature(img, label, curvature_R,
				 &V_curv_x, &V_curv_y);

      //-------------------------
      gft::sImage32 *tmp;
      tmp = gft::Image32::Create(label);
      for(p = 0; p < label->n; p++){
      	tmp->data[p] = ROUND(curv[p]*200.0);
      }
      gft::Image32::Write(tmp, (char *)"curvature.pgm");
      gft::Image32::Destroy(&tmp);
      //-------------------------
      gft::FreeDoubleArray(&curv);
      
      sum_curv_x = (double **)calloc(G->nnodes, sizeof(double *));
      sum_curv_y = (double **)calloc(G->nnodes, sizeof(double *));
      n_curv = (int **)calloc(G->nnodes, sizeof(int *));
      for(p = 0; p < G->nnodes; p++){
	n = G->nodes[p].outdegree;
	sum_curv_x[p] = (double*)calloc(n, sizeof(double));
	sum_curv_y[p] = (double*)calloc(n, sizeof(double));
	n_curv[p]   = (int*)calloc(n, sizeof(int));
      }

      A = gft::AdjRel::Neighborhood_4();
      lqs = (int*)calloc(A->n, sizeof(int));

      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	nlqs = 0;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      found = false;
	      for(j = 0; j < nlqs; j++){
		if(lqs[j] == lq){
		  found = true;
		  break;
		}
	      }
	      if(found == false){
		lqs[nlqs] = lq;
		nlqs++;
	      }
	    }
	  }
	}
	for(j = 0; j < nlqs; j++){
	  lq = lqs[j];
	  k = gft::Graph::GetArcIndex(G, lp, lq);
	  sum_curv_x[lp][k] += V_curv_x[p];
	  sum_curv_y[lp][k] += V_curv_y[p];
	  n_curv[lp][k] += 1;

	  k = gft::Graph::GetArcIndex(G, lq, lp);
	  sum_curv_x[lq][k] += V_curv_x[p];
	  sum_curv_y[lq][k] += V_curv_y[p];
	  n_curv[lq][k] += 1;	  
	}
      }
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  sum_curv_x[p][i] /= (double)n_curv[p][i];
	  sum_curv_y[p][i] /= (double)n_curv[p][i];
	}
      }

      mean_weight = 0.0;
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq_x = sum_curv_x[p][i];
	  curv_pq_y = sum_curv_y[p][i];	  
	  curv_pq = sqrt(curv_pq_x*curv_pq_x +
	  		 curv_pq_y*curv_pq_y);
	  mean_weight += pow(1.0 - curv_pq, exponent);
	}
      }

      mean_weight /= gft::Graph::GetNumberOfArcs(G);
      printf("mean_weight: %f\n", mean_weight);
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq_x = sum_curv_x[p][i];
	  curv_pq_y = sum_curv_y[p][i];	  
	  curv_pq = sqrt(curv_pq_x*curv_pq_x +
	  		 curv_pq_y*curv_pq_y);
	  w = G->nodes[p].Warcs[i];
	  G->nodes[p].Warcs[i] = ROUND((100.0*w*pow(1.0 - curv_pq, exponent))/mean_weight);
	}
      }
      printf("max: %d\n", gft::Graph::GetMaximumArc(G));
      printf("min: %d\n", gft::Graph::GetMinimumArc(G));      
      
      free(lqs);
      gft::AdjRel::Destroy(&A);
      for(p = 0; p < G->nnodes; p++){
	free(sum_curv_x[p]);
	free(sum_curv_y[p]);
	free(n_curv[p]);
      }
      free(sum_curv_x);
      free(sum_curv_y);
      free(n_curv);
      gft::FreeDoubleArray(&V_curv_x);
      gft::FreeDoubleArray(&V_curv_y);
      return G;
    }
    */   

    
    
    void RAG(sGraph *G,
	     sImage32 *img,
	     sImage32 *label,
	     float curvature_R,
	     float exponent,
	     float powermean){
      gft::sAdjRel *A;
      double *curv;
      double **sum_curv;
      double curv_pq, mean_weight, c;
      int **n_curv;
      int p, q, n, w;
      int i, j, k, px, py, qx, qy, lp, lq, nlqs;
      int *lqs;
      bool found;
      curv = gft::LLW::Curvature(img, label, curvature_R, NULL, NULL);

      //-------------------------
      gft::sImage32 *tmp;
      tmp = gft::Image32::Create(label);
      for(p = 0; p < label->n; p++){
      	tmp->data[p] = ROUND(curv[p]*200.0);
      }
      gft::Image32::Write(tmp, (char *)"curvature.pgm");
      gft::Image32::Destroy(&tmp);
      //-------------------------
      
      sum_curv = (double **)calloc(G->nnodes, sizeof(double *));
      n_curv = (int **)calloc(G->nnodes, sizeof(int *));
      for(p = 0; p < G->nnodes; p++){
	n = G->nodes[p].outdegree;
	sum_curv[p] = (double*)calloc(n, sizeof(double));
	n_curv[p]   = (int*)calloc(n, sizeof(int));
      }

      A = gft::AdjRel::Neighborhood_4();
      lqs = (int*)calloc(A->n, sizeof(int));

      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	nlqs = 0;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      found = false;
	      for(j = 0; j < nlqs; j++){
		if(lqs[j] == lq){
		  found = true;
		  break;
		}
	      }
	      if(found == false){
		lqs[nlqs] = lq;
		nlqs++;
	      }
	    }
	  }
	}
	for(j = 0; j < nlqs; j++){
	  c = pow(curv[p], powermean);

	  lq = lqs[j];
	  k = gft::Graph::GetArcIndex(G, lp, lq);
	  sum_curv[lp][k] += c;
	  n_curv[lp][k] += 1;

	  k = gft::Graph::GetArcIndex(G, lq, lp);
	  sum_curv[lq][k] += c;
	  n_curv[lq][k] += 1;	  
	}
      }
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq = (sum_curv[p][i]/(double)n_curv[p][i]);
	  if(powermean == 2.0)
	    sum_curv[p][i] = sqrt(curv_pq);
	  else
	    sum_curv[p][i] = pow(curv_pq, 1.0/powermean);
	}
      }

      mean_weight = 0.0;
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq = sum_curv[p][i];
	  mean_weight += pow(1.0 - curv_pq, exponent);
	}
      }

      mean_weight /= gft::Graph::GetNumberOfArcs(G);
      printf("mean_weight: %f\n", mean_weight);
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq = gft::LinearStretch(sum_curv[p][i],
				       0.05, 1.0,
				       0.0,  1.0);
	  w = G->nodes[p].Warcs[i];
	  //G->nodes[p].Warcs[i] = ROUND(curv_pq*200.0);
	  G->nodes[p].Warcs[i] = ROUND((100.0*w*pow(1.0 - curv_pq, exponent))/mean_weight);
	}
      }
      printf("max: %d\n", gft::Graph::GetMaximumArc(G));
      printf("min: %d\n", gft::Graph::GetMinimumArc(G));      
      
      free(lqs);
      gft::AdjRel::Destroy(&A);
      for(p = 0; p < G->nnodes; p++){
	free(sum_curv[p]);
	free(n_curv[p]);
      }
      free(sum_curv);
      free(n_curv);
      gft::FreeDoubleArray(&curv);
    }
    



    void RAG(sGraph *G,
	     sImage32 *img,
	     sImage32 *label,
	     float curvature_R,
	     float exponent,
	     float powermean,
	     bool convexonly){
      gft::sAdjRel *A;
      double *curv;
      double **sum_curv_x;
      double **sum_curv_y;
      double curv_pq, curv_pq_x, curv_pq_y, mean_weight, c;
      int **n_curv;
      int p, q, n, w, Pc, Qc, Pc_x, Pc_y, Qc_x, Qc_y;
      int i, j, k, px, py, qx, qy, lp, lq, nlqs;
      int *lqs, *CoG;
      bool found;
      double *V_curv_x, *V_curv_y;
      double dotprod;

      if(!convexonly){
	RAG(G,
	    img,
	    label,
	    curvature_R,
	    exponent,
	    powermean);
	return;
      }

      CoG = gft::Image32::ComputeCoG(label);
      curv = gft::LLW::Curvature(img, label, curvature_R,
				 &V_curv_x, &V_curv_y);

      //-------------------------
      gft::sImage32 *tmp;
      tmp = gft::Image32::Create(label);
      for(p = 0; p < label->n; p++){
      	tmp->data[p] = ROUND(curv[p]*200.0);
	//	tmp->data[p] = ROUND(200.0*sqrt(V_curv_x[p]*V_curv_x[p] +
	//				V_curv_y[p]*V_curv_y[p]));
      }
      gft::Image32::Write(tmp, (char *)"curvature.pgm");
      gft::Image32::Destroy(&tmp);
      //-------------------------
      gft::FreeDoubleArray(&curv);
      
      sum_curv_x = (double **)calloc(G->nnodes, sizeof(double *));
      sum_curv_y = (double **)calloc(G->nnodes, sizeof(double *));
      n_curv = (int **)calloc(G->nnodes, sizeof(int *));
      for(p = 0; p < G->nnodes; p++){
	n = G->nodes[p].outdegree;
	sum_curv_x[p] = (double*)calloc(n, sizeof(double));
	sum_curv_y[p] = (double*)calloc(n, sizeof(double));
	n_curv[p]   = (int*)calloc(n, sizeof(int));
      }

      A = gft::AdjRel::Neighborhood_4();
      lqs = (int*)calloc(A->n, sizeof(int));

      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	nlqs = 0;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      found = false;
	      for(j = 0; j < nlqs; j++){
		if(lqs[j] == lq){
		  found = true;
		  break;
		}
	      }
	      if(found == false){
		lqs[nlqs] = lq;
		nlqs++;
	      }
	    }
	  }
	}
	for(j = 0; j < nlqs; j++){
	  lq = lqs[j];

	  Pc = CoG[lp];
	  Pc_x = Pc%label->ncols;
	  Pc_y = Pc/label->ncols;
	  Qc = CoG[lq];
	  Qc_x = Qc%label->ncols;
	  Qc_y = Qc/label->ncols;

	  dotprod = ((Qc_x - Pc_x)*V_curv_x[p] +
		     (Qc_y - Pc_y)*V_curv_y[p]);
	  if(dotprod > 0.0){
	    k = gft::Graph::GetArcIndex(G, lp, lq);
	    sum_curv_x[lp][k] += V_curv_x[p];
	    sum_curv_y[lp][k] += V_curv_y[p];
	    n_curv[lp][k] += 1;
	  }
	  if(dotprod < 0.0){
	    k = gft::Graph::GetArcIndex(G, lq, lp);
	    sum_curv_x[lq][k] += V_curv_x[p];
	    sum_curv_y[lq][k] += V_curv_y[p];
	    n_curv[lq][k] += 1;
	  }
	}
      }
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  if(n_curv[p][i] > 0){
	    sum_curv_x[p][i] /= (double)n_curv[p][i];
	    sum_curv_y[p][i] /= (double)n_curv[p][i];
	  }
	}
      }

      mean_weight = 0.0;
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq_x = sum_curv_x[p][i];
	  curv_pq_y = sum_curv_y[p][i];	  
	  curv_pq = sqrt(curv_pq_x*curv_pq_x +
	  		 curv_pq_y*curv_pq_y);
	  mean_weight += pow(1.0 - curv_pq, exponent);
	}
      }

      mean_weight /= gft::Graph::GetNumberOfArcs(G);
      printf("mean_weight: %f\n", mean_weight);
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq_x = sum_curv_x[p][i];
	  curv_pq_y = sum_curv_y[p][i];	  
	  curv_pq = sqrt(curv_pq_x*curv_pq_x +
	  		 curv_pq_y*curv_pq_y);
	  curv_pq = gft::LinearStretch(curv_pq,
				       0.05, 1.0,
				       0.0,  1.0);
	  w = G->nodes[p].Warcs[i];
	  /*
	  Pc = CoG[p];
	  Pc_x = Pc%label->ncols;
	  Pc_y = Pc/label->ncols;
	  q  = G->nodes[p].adjList[i];
	  Qc = CoG[q];
	  Qc_x = Qc%label->ncols;
	  Qc_y = Qc/label->ncols;

	  dotprod = ((Qc_x - Pc_x)*curv_pq_x +
		     (Qc_y - Pc_y)*curv_pq_y);
	  if(dotprod > 0.0 || !convexonly)
	  */
	  G->nodes[p].Warcs[i] = ROUND((100.0*w*pow(1.0 - curv_pq, exponent))/mean_weight);
	    /*
	    else
	    G->nodes[p].Warcs[i] = ROUND((100.0*w)/mean_weight);
	    */
	}
      }
      printf("max: %d\n", gft::Graph::GetMaximumArc(G));
      printf("min: %d\n", gft::Graph::GetMinimumArc(G));      
      
      free(lqs);
      gft::AdjRel::Destroy(&A);
      for(p = 0; p < G->nnodes; p++){
	free(sum_curv_x[p]);
	free(sum_curv_y[p]);
	free(n_curv[p]);
      }
      free(sum_curv_x);
      free(sum_curv_y);
      free(n_curv);
      free(CoG);
      gft::FreeDoubleArray(&V_curv_x);
      gft::FreeDoubleArray(&V_curv_y);
    }
    
    
    /*
    sGraph *RAG(sImage32 *img,
		sImage32 *label,
		float polarity,
		float curvature_R,
		bool convexonly,
		float exponent,
		float powermean){
      gft::sGraph *G;
      gft::sAdjRel *A;
      double *curv;
      double **sum_curv;
      double **sum_curv_x;
      double **sum_curv_y;
      double curv_pq, curv_pq_x, curv_pq_y, mean_weight, c;
      int **n_curv;
      int p, q, n, w, Pc, Qc, Pc_x, Pc_y, Qc_x, Qc_y;
      int i, j, k, px, py, qx, qy, lp, lq, nlqs;
      int *lqs, *CoG;
      bool found;
      double *V_curv_x, *V_curv_y;
      double dotprod;
      G = RAG(img, label, polarity);
      CoG = gft::Image32::ComputeCoG(label);
      curv = gft::LLW::Curvature(img, label, curvature_R,
				 &V_curv_x, &V_curv_y);

      //-------------------------
      gft::sImage32 *tmp;
      tmp = gft::Image32::Create(label);
      for(p = 0; p < label->n; p++){
      	tmp->data[p] = ROUND(curv[p]*200.0);
	//	tmp->data[p] = ROUND(200.0*sqrt(V_curv_x[p]*V_curv_x[p] +
	//				V_curv_y[p]*V_curv_y[p]));
      }
      gft::Image32::Write(tmp, (char *)"curvature.pgm");
      gft::Image32::Destroy(&tmp);
      //-------------------------
      
      sum_curv   = (double **)calloc(G->nnodes, sizeof(double *));
      sum_curv_x = (double **)calloc(G->nnodes, sizeof(double *));
      sum_curv_y = (double **)calloc(G->nnodes, sizeof(double *));
      n_curv = (int **)calloc(G->nnodes, sizeof(int *));
      for(p = 0; p < G->nnodes; p++){
	n = G->nodes[p].outdegree;
	sum_curv[p]   = (double*)calloc(n, sizeof(double));
	sum_curv_x[p] = (double*)calloc(n, sizeof(double));
	sum_curv_y[p] = (double*)calloc(n, sizeof(double));
	n_curv[p]   = (int*)calloc(n, sizeof(int));
      }

      A = gft::AdjRel::Neighborhood_4();
      lqs = (int*)calloc(A->n, sizeof(int));

      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	px = p%label->ncols;
	py = p/label->ncols;
	nlqs = 0;
	for(i = 1; i < A->n; i++){
	  qx = px + A->dx[i];
	  qy = py + A->dy[i];
	  if(gft::Image32::IsValidPixel(label, qx, qy)){
	    q = qx + qy*label->ncols;
	    lq = label->data[q];
	    if(lp != lq){
	      found = false;
	      for(j = 0; j < nlqs; j++){
		if(lqs[j] == lq){
		  found = true;
		  break;
		}
	      }
	      if(found == false){
		lqs[nlqs] = lq;
		nlqs++;
	      }
	    }
	  }
	}
	for(j = 0; j < nlqs; j++){
	  c = pow(curv[p], powermean);

	  lq = lqs[j];
	  k = gft::Graph::GetArcIndex(G, lp, lq);
	  sum_curv[lp][k] += c;
	  sum_curv_x[lp][k] += V_curv_x[p];
	  sum_curv_y[lp][k] += V_curv_y[p];
	  n_curv[lp][k] += 1;

	  k = gft::Graph::GetArcIndex(G, lq, lp);
	  sum_curv[lq][k] += c;
	  sum_curv_x[lq][k] += V_curv_x[p];
	  sum_curv_y[lq][k] += V_curv_y[p];
	  n_curv[lq][k] += 1;	  
	}
      }
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq = (sum_curv[p][i]/(double)n_curv[p][i]);
	  if(powermean == 2.0)
	    sum_curv[p][i] = sqrt(curv_pq);
	  else
	    sum_curv[p][i] = pow(curv_pq, 1.0/powermean);

	  sum_curv_x[p][i] /= (double)n_curv[p][i];
	  sum_curv_y[p][i] /= (double)n_curv[p][i];	  
	}
      }

      mean_weight = 0.0;
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq = sum_curv[p][i];
	  mean_weight += pow(1.0 - curv_pq, exponent);
	}
      }

      mean_weight /= gft::Graph::GetNumberOfArcs(G);
      printf("mean_weight: %f\n", mean_weight);
      
      for(p = 0; p < G->nnodes; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  curv_pq = sum_curv[p][i];
	  curv_pq_x = sum_curv_x[p][i];
	  curv_pq_y = sum_curv_y[p][i];	  
	  //curv_pq = sqrt(curv_pq_x*curv_pq_x +
	  //		 curv_pq_y*curv_pq_y);
	  w = G->nodes[p].Warcs[i];

	  Pc = CoG[p];
	  Pc_x = Pc%label->ncols;
	  Pc_y = Pc/label->ncols;
	  q  = G->nodes[p].adjList[i];
	  Qc = CoG[q];
	  Qc_x = Qc%label->ncols;
	  Qc_y = Qc/label->ncols;

	  dotprod = ((Qc_x - Pc_x)*curv_pq_x +
		     (Qc_y - Pc_y)*curv_pq_y);
	  if(dotprod > 0.0 || !convexonly)
	    G->nodes[p].Warcs[i] = ROUND((100.0*w*pow(1.0 - curv_pq, exponent))/mean_weight);
	  else
	    G->nodes[p].Warcs[i] = ROUND((100.0*w)/mean_weight);
	}
      }
      printf("max: %d\n", gft::Graph::GetMaximumArc(G));
      printf("min: %d\n", gft::Graph::GetMinimumArc(G));      
      
      free(lqs);
      gft::AdjRel::Destroy(&A);
      for(p = 0; p < G->nnodes; p++){
	free(sum_curv[p]);
	free(sum_curv_x[p]);
	free(sum_curv_y[p]);
	free(n_curv[p]);
      }
      free(sum_curv);
      free(sum_curv_x);
      free(sum_curv_y);
      free(n_curv);
      free(CoG);
      gft::FreeDoubleArray(&curv);
      gft::FreeDoubleArray(&V_curv_x);
      gft::FreeDoubleArray(&V_curv_y);
      return G;
    }
    */
    
    
    sGraph *RAG(sScene32 *scn,
		sScene32 *label){
      sAdjRel3 *A;
      sGraph *graph = NULL;
      int nnodes, p, l, q, i, lp, lq;
      float *value = NULL;
      int *size;
      float w;
      Voxel u,v;
      nnodes = gft::Scene32::GetMaximumValue(label) + 1;
      value = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(value == NULL || size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	value[l] += scn->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0)
	  value[l] /= size[l];
	else
	  printf("Empty regions in RAG\n");
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel3::Spheric(1.0);
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	u.c.x = gft::Scene32::GetAddressX(label, p);
	u.c.y = gft::Scene32::GetAddressY(label, p);
	u.c.z = gft::Scene32::GetAddressZ(label, p);
	for(i = 1; i < A->n; i++){
	  v.v = u.v + A->d[i].v;
	  if(gft::Scene32::IsValidVoxel(label, v)){
	    q = gft::Scene32::GetVoxelAddress(label, v);
	    lq = label->data[q];
	    if(lp != lq){
	      w = fabsf(value[lp] - value[lq]);
	      gft::Graph::UpdateEdge(graph, lp, lq, ROUND(w));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&value);
      gft::AdjRel3::Destroy(&A);
      return graph;
    }



    sGraph *RAG(sScene32 *scn,
		sScene32 *label,
		float polarity){
      sAdjRel3 *A;
      sGraph *graph = NULL;
      int nnodes, p, l, q, i, lp, lq;
      float *value = NULL;
      int *size;
      float w,wpq,wqp;
      Voxel u,v;
      nnodes = gft::Scene32::GetMaximumValue(label) + 1;
      value = gft::AllocFloatArray(nnodes);
      size = gft::AllocIntArray(nnodes);
      if(value == NULL || size == NULL){
	printf("Error\n");
	exit(1);
      }  
      for(p = 0; p < label->n; p++){
	l = label->data[p];
	size[l] += 1;
	value[l] += scn->data[p];
      }
      for(l = 0; l < nnodes; l++){
	if(size[l] > 0)
	  value[l] /= size[l];
	else
	  printf("Empty regions in RAG\n");
      }
      //-------------
      graph = gft::Graph::Create(nnodes, 10, NULL);
      A = gft::AdjRel3::Spheric(1.0);
      for(p = 0; p < label->n; p++){
	lp = label->data[p];
	u.c.x = gft::Scene32::GetAddressX(label, p);
	u.c.y = gft::Scene32::GetAddressY(label, p);
	u.c.z = gft::Scene32::GetAddressZ(label, p);
	for(i = 1; i < A->n; i++){
	  v.v = u.v + A->d[i].v;
	  if(gft::Scene32::IsValidVoxel(label, v)){
	    q = gft::Scene32::GetVoxelAddress(label, v);
	    lq = label->data[q];
	    if(lp != lq){
	      w = fabsf(value[lp] - value[lq]);
	      if(value[lp] > value[lq]){
		wpq = w*(1.0 + polarity);
		wqp = w*(1.0 - polarity);
	      }
	      else if(value[lp] < value[lq]){
		wpq = w*(1.0 - polarity);
		wqp = w*(1.0 + polarity);
	      }
	      else{
		wpq = wqp = w;
	      }
	      gft::Graph::UpdateDirectedEdge(graph, lp, lq, ROUND(wpq));
	      gft::Graph::UpdateDirectedEdge(graph, lq, lp, ROUND(wqp));
	    }
	  }
	}
      }
      //-------------
      gft::FreeIntArray(&size);
      gft::FreeFloatArray(&value);
      gft::AdjRel3::Destroy(&A);
      return graph;
    }



    sCImage *DrawRAG(sGraph *G,
		     sImage32 *label, float scale,
		     float R, float r, float w, float h){
      sCImage *draw;
      sImage32 *slabel;
      int p,q,i,l,lp,lq,Lmax,weight,Wmax,Cpx,Cpy,Cqx,Cqy,color,dx,dy;
      float dlm,dlx,dly,dtx,dty;
      int *CoG;
      //float *Sx,*Sy;
      //int *size;

      slabel = gft::Image32::Scale(label, scale, scale, gft::none);
      draw = gft::CImage::ColorizeLabel(slabel);
      Lmax = gft::Image32::GetMaxVal(label);

      if(Lmax+1 != G->nnodes){
	gft::Warning((char*)"Incompatible graph and image of labels.",
		     (char*)"DrawRAG");
	return draw;
      }
      /*
      size = (int*)  calloc(Lmax+1, sizeof(int));
      Sx   = (float*)calloc(Lmax+1, sizeof(float));
      Sy   = (float*)calloc(Lmax+1, sizeof(float));
      if(size == NULL || Sx == NULL || Sy == NULL)
	gft::Error((char*)MSG1, (char*)"DrawRAG");

      for(p = 0; p < slabel->n; p++){
	l = slabel->data[p];
	size[l]++;
	Sx[l] += p%slabel->ncols;
	Sy[l] += p/slabel->ncols;
      }
      for(l = 0; l <= Lmax; l++){
	if(size[l] > 0){
	  Sx[l] /= size[l];
	  Sy[l] /= size[l];
	}
      }
      */
      CoG = gft::Image32::ComputeCoG(slabel);
      
      Wmax = GetMaximumArc(G);
      for(lp = 0; lp < G->nnodes; lp++){
	for(i = 0; i < G->nodes[lp].outdegree; i++){
	  lq = G->nodes[lp].adjList[i];
	  weight = G->nodes[lp].Warcs[i];
	  Cpx = CoG[lp] % slabel->ncols; //ROUND(Sx[lp]);
	  Cpy = CoG[lp] / slabel->ncols; //ROUND(Sy[lp]);
	  Cqx = CoG[lq] % slabel->ncols; //ROUND(Sx[lq]);
	  Cqy = CoG[lq] / slabel->ncols; //ROUND(Sy[lq]);
	  color = ROUND(((float)weight/(float)Wmax)*255.0);
	  color = gft::Color::Triplet(color, color, color);

	  // A vector along the arc:
	  dlm = hypot(Cqx - Cpx, Cqy - Cpy);
	  dlx = (Cqx - Cpx)/dlm;
	  dly = (Cqy - Cpy)/dlm;
	  // A vector orthogonal to the arc:
	  dtx = -dly;
	  dty =  dlx;
	  
	  Cpx = ROUND(Cpx + dlx*R); Cpy = ROUND(Cpy + dly*R);
	  Cqx = ROUND(Cqx - dlx*R); Cqy = ROUND(Cqy - dly*R);
	  dx = ROUND(dtx*w); dy = ROUND(dty*w);
	  gft::CImage::DrawArrow(draw,
				 Cpx + dx, Cpy + dy,
				 Cqx + dx, Cqy + dy,
	  			 r, w, h, color);
	}
      }

      for(lp = 0; lp < G->nnodes; lp++){
	Cpx = CoG[lp] % slabel->ncols; //ROUND(Sx[lp]);
	Cpy = CoG[lp] / slabel->ncols; //ROUND(Sy[lp]);
	gft::CImage::DrawCircle(draw, Cpx, Cpy, R, 0x000000);
      }

      /*
      free(size);
      free(Sx);
      free(Sy);
      */
      free(CoG);
      return draw;
    }

    

    sGraph *Split(sGraph **G, int *label, int lb){
      sGraph *G1 = NULL, *G2 = NULL;
      int *index = NULL;
      int p,q, nlb,i,j,w,lp,lq;

      if(G == NULL){
	printf("Error\n");
	return NULL;
      }
      if((*G)->Wnodes != NULL){
	printf("Split does not support Wnodes\n");
	return NULL;
      }
	  
      nlb = 0;
      for(p = 0; p < (*G)->nnodes; p++)
	if(label[p] == lb)
	  nlb++;

      if((*G)->nnodes == nlb || nlb == 0){
	printf("Invalid split\n");
	return NULL;
      }
      
      G1 = Create(nlb,
		  (*G)->nodes[0].arraysize, NULL);
      G2 = Create((*G)->nnodes - nlb,
		  (*G)->nodes[0].arraysize, NULL);
      G1->type = (*G)->type;
      G2->type = (*G)->type;
      
      index = (int *)malloc((*G)->nnodes*sizeof(int));

      i = j = 0;
      for(p = 0; p < (*G)->nnodes; p++){
	if(label[p] == lb){
	  G1->nodes[i].id = (*G)->nodes[p].id;
	  index[p] = i;
	  i++;
	}
	else{
	  G2->nodes[j].id = (*G)->nodes[p].id;
	  index[p] = j;
	  j++;
	}
      }

      for(p = 0; p < (*G)->nnodes; p++){
	lp = label[p];
	for(i = 0; i < (*G)->nodes[p].outdegree; i++){
	  q = (*G)->nodes[p].adjList[i];
	  w = (*G)->nodes[p].Warcs[i];
	  lq = label[q];
	  if(lp == lb && lq == lb)
	    AddDirectedEdge(G1, index[p], index[q], w);
	  else if(lp != lb && lq != lb)
	    AddDirectedEdge(G2, index[p], index[q], w);
	}
      }
      
      free(index);
      Destroy(G);
      *G = G2;
      return G1;
    }    




    void LinearStretch(sGraph *G,
		       int f1, int f2,
		       int g1, int g2){
      int p,n,i,v;
      float a=1.0;
      
      n = G->nnodes;
      if (f1 != f2) 
	a = (float)(g2-g1)/(float)(f2-f1);
      else
	gft::Error((char *)"Invalid input value",
		   (char *)"Graph::LinearStretch");
      
      for(p = 0; p < n; p++){
	for(i = 0; i < G->nodes[p].outdegree; i++){
	  v = G->nodes[p].Warcs[i];
	  if(v == INT_MAX || v == 0)
	    continue;
	  
	  if(v < f1)
	    G->nodes[p].Warcs[i] = g1;
	  else if(v > f2)
	    G->nodes[p].Warcs[i] = g2;
	  else
	    G->nodes[p].Warcs[i] = MIN(ROUND(a*(v-f1)+g1),g2);
	}
      }
    }



    sGraph *WeightedMean(sGraph *G1, 
			 sGraph *G2,
			 float w2){
      sGraph *G;
      int i,p,q,n;
      int weight,v1,v2,worst;
      
      if(G1->nnodes != G2->nnodes ||
	 G1->type != G2->type){
	gft::Error((char*)"Incompatible graphs",
		   (char*)"Graph::WeightedMean");
      }
      n = G1->nnodes;
      G = Create(n, G1->nodes[0].outdegree, NULL);
      G->type = G1->type;

      if(G1->type == DISSIMILARITY)
	worst = INT_MAX;
      else
	worst = 0;
      
      for(p=0; p<n; p++){
	for(i = 0; i < G1->nodes[p].outdegree; i++){
	  q = G1->nodes[p].adjList[i];
	  v1 = G1->nodes[p].Warcs[i];
	  v2 = GetArcWeight(G2, p, q);

	  if(v1 == INT_MAX || v2 == INT_MAX)
	    weight = INT_MAX;
	  else if(v1 == 0 || v2 == 0)
	    weight = 0;
	  else
	    weight = ROUND(w2*v2 +(1.0-w2)*v1);

	  AddDirectedEdge(G, p, q, weight);
	}
      }
      return G;
    }

    
    
  } /*end Graph namespace*/
} /*end gft namespace*/


