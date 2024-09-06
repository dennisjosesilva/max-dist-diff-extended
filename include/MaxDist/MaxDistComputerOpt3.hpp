#pragma once 

#include <morphotree/tree/mtree.hpp>
#include <morphotree/core/box.hpp>

#include <gft2/gft.h>

class MaxDistComputerOpt3
{
public:
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = MTree::NodePtr;
  using Box = morphotree::Box;
  using Adjacency = morphotree::Adjacency;
  using I32Point = morphotree::I32Point;

  MaxDistComputerOpt3(const Box &domain, const std::vector<uint8> &f);

  std::vector<uint32> computeAttribute(const MTree &tree) const;

private:
  std::array<std::vector<NodePtr>, 256> extractLevelMap(const MTree &tree) const;
  gft::sImage32 *createGFTImage() const;

  void insertNeighborsPQueue(
    uint32_t pdix,
    gft::sImage32 *bin,
    gft::sImage32 *cost,
    gft::sPQueue32 *Q) const;

private:
  const Box &domain_;
  const std::vector<uint8> &f_;
  const std::vector<I32Point> OFFSET_NEIGHBOR;
};

std::vector<morphotree::uint32> computeMaxDistanceAttributeOpt3(
  const morphotree::Box &domain,
  const std::vector<morphotree::uint8> &f,
  const MaxDistComputerOpt3::MTree &tree);