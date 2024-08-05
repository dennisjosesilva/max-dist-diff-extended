#ifndef EDT_DIFF_HPP_INCLUDED
#define EDT_DIFF_HPP_INCLUDED

#include <queue>
#include <gft.h>
#include <vector>
#include <morphotree/core/alias.hpp>

void treeRemoval(const std::vector<morphotree::uint32> &toRemove, 
		 gft::sImage32 *bin,
		 gft::sPQueue32 *Q,
		 gft::sImage32 *root,
		 gft::sImage32 *pred,
		 gft::sImage32 *cost,
		 gft::sAdjRel *A);


void treeRemoval(int *R, int nR,
		 gft::sImage32 *bin,
		 gft::sPQueue32 *Q,
		 gft::sImage32 *root,
		 gft::sImage32 *pred,
		 gft::sImage32 *cost,
		 gft::sAdjRel *A); 



void removeSubTree(int q_in,
		   gft::sImage32 *bin,
		   gft::sPQueue32 *Q,
		   gft::sImage32 *root,
		   gft::sImage32 *pred,
		   gft::sImage32 *cost,
		   gft::sAdjRel *A); 



void EDT_DIFF(gft::sPQueue32 *Q,
	      gft::sAdjRel *A,
	      gft::sImage32 *bin,
	      gft::sImage32 *root,
	      gft::sImage32 *pred,
	      gft::sImage32 *cost,
	      gft::sImage32 *Bedt); 
        
#endif
