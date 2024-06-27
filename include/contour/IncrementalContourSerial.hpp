#include <morphology/Adjacency.hpp>
#include <morphology/ErosionSerial.hpp>
#include <morphotree/core/box.hpp>
#include <morphotree/tree/mtree.hpp>
#include <unordered_set>
#include <array>

class IncrementalContourSerial 
{
public:
  using Box = morphotree::Box;
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;  
  using I32Point = morphotree::I32Point;
  using Contours = std::unordered_set<uint32>;

  IncrementalContourSerial(
    const Box &domain,
    const std::vector<uint8> &f,
    Adj adj);
  
  std::vector<Contours> compute(const MTree &tree) const;

private:
  std::array<std::vector<NodePtr>, 256> 
    extractLevelMap(const MTree &tree) const;

private:
  const Box &domain_;
  const std::vector<uint8> &f_;
  Adj adj_;  
};

std::vector<IncrementalContourSerial::Contours> extractContourSerial(
  const IncrementalContourSerial::Box &domain, 
  const std::vector<IncrementalContourSerial::uint8> &f,
  Adj adj,
  const IncrementalContourSerial::MTree &tree
);