#pragma once

#include <morphotree/tree/mtree.hpp>
#include <morphotree/core/box.hpp>

#include <gft2/gft.h>

class ExactMaxDistComputer
{
public:
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = MTree::NodePtr;
  using Box = morphotree::Box;
  using Adjacency = morphotree::Adjacency;
  using I32Point = morphotree::I32Point;

  ExactMaxDistComputer(const Box &domain, 
    const std::vector<uint8> &f);
  
  std::vector<uint32> computeAttribute(const MTree &tree) const;

private:
  const Box &domain_;
  const std::vector<uint8> &f_;

private:
  std::array<std::vector<NodePtr>, 256> extractLevelMap(const MTree &tree) const;
  int EDT_f(int x, int i, int *g) const;
  int EDT_Sep(int i, int u, int *g, int ncols) const;
};


std::vector<morphotree::uint32> computeExactMaxDistanceAttribute(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const ExactMaxDistComputer::MTree &tree);