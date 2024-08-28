#ifndef EDT_DIFF2_HPP_INCLUDED
#define EDT_DIFF2_HPP_INCLUDED

#include <queue>
#include <gft2/gft.h>
#include <vector>
#include <morphotree/core/alias.hpp>

void treeRemoval2(
  const std::vector<morphotree::uint32> &toRemove,
  std::vector<int> &stack,
  gft::sImage32 *bin,
  gft::sPQueue32 *Q,
  gft::sImage32 *root,
  gft::sImage32 *pred,
  gft::sImage32 *cost,
  gft::sAdjPxl *N);

void removeSubTree2(int q_in,
  gft::sImage32 *bin,
  gft::sPQueue32 *Q,
  gft::sImage32 *root,
  gft::sImage32 *pred,
  gft::sImage32 *cost,
  gft::sAdjPxl *N);

void EDT_DIFF2(gft::sPQueue32 *Q,
	      gft::sAdjRel *A,
	      gft::sAdjPxl *N,
	      gft::sImage32 *bin,
	      gft::sImage32 *root,
	      gft::sImage32 *pred,
	      gft::sImage32 *cost,
	      gft::sImage32 *Bedt);

#endif