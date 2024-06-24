#pragma once 

#include <vector>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/core/box.hpp>

class InertiaComputer
{
public:
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using uint64 = unsigned long long;
  using Box = morphotree::Box;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;

  InertiaComputer(const Box &domain);

  std::vector<double> computeAttribute(const MTree &tree);

private:
  struct AuxAttributes
  {
    AuxAttributes();


    double area;
    double m10;
    double m01;
    double m02;
    double m20;
  };

  const Box &domain_;
  std::vector<AuxAttributes> auxAttrs_;
  

private:
  void computeAtCNPs(std::vector<double>& inertia, NodePtr node);

  void mergeToParent(std::vector<double>& inertia, NodePtr node,
    NodePtr parent);

  void finalizeComputation(std::vector<double>& inertia, NodePtr node);
};


std::vector<double>
  computeInertia(
    const InertiaComputer::Box &domain, 
    const InertiaComputer::MTree &tree);