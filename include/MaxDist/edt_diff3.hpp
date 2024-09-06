#ifndef EDT_DIFF3_HPP_INCLUDED
#define EDT_DIFF3_HPP_INCLUDED

#include <queue>
#include <gft3/gft.h>
#include <vector>
#include <morphotree/core/alias.hpp>


// ============================================================
// Adaptive adjacency
// ===========================================================
struct sAdaptiveAdj
{
  int *dp;
  int *ia; // index for the next adj. relation
  int n;
  int np; // n for propagation
};

struct sAdaptiveAdjBank 
{
  struct sAdaptiveAdj  **N;
  int nbank;
};

struct sAdaptiveAdj *Create_AdaptiveAdj(int n);
void Destroy_AdaptiveAdj(struct sAdaptiveAdj **N);
struct sAdaptiveAdjBank *Create_AdaptiveAdjBank(int n);
void Destroy_AdaptiveAdjBank(struct sAdaptiveAdjBank **B);
struct sAdaptiveAdjBank *AdaptiveAdj_8(int ncols);
gft::sImage8 *Create_AdjMap(int ncols, int nrows);

// ===============================================================
// Functions
// ===============================================================

void treeRemoval3(
  const std::vector<morphotree::uint32> &toRemove,
  std::vector<int> &stack,
  gft::sImage32 *bin,
  gft::sPQueue32 *Q,
  gft::sImage32 *root,
  gft::sImage32 *cost,
  struct sAdaptiveAdjBank *AAB,
  gft::sImage8 *Amap,
  gft::sImage8 *O);

void EDT_DIFF3(gft::sPQueue32 *Q,
	      struct sAdaptiveAdjBank *AAB,
	      gft::sImage8 *Amap,
	      gft::sImage32 *bin,
	      gft::sImage32 *root,	      
	      gft::sImage32 *cost,
	      gft::sImage32 *Bedt,
        gft::sImage8 *O);

#endif