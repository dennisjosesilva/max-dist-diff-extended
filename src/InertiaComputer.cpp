#include <InertiaComputer.hpp>
#include <algorithm>
#include <cmath>


InertiaComputer::AuxAttributes::AuxAttributes()
  : area{0.0},
    m10{0.0},
    m01{0.0},
    m20{0.0},
    m02{0.0}
{}

InertiaComputer::InertiaComputer(const Box &domain)
  : domain_{domain}
{}

std::vector<double> InertiaComputer::computeAttribute(const MTree &tree)
{
  std::vector<double> inertia(tree.numberOfNodes(), 0.0);
  auxAttrs_.clear();
  auxAttrs_.resize(tree.numberOfNodes());

  tree.tranverse([this, &inertia](NodePtr node){
    computeAtCNPs(inertia, node);

    if (node->parent() != nullptr) 
      mergeToParent(inertia, node, node->parent());

    finalizeComputation(inertia, node);
  });

  return inertia;
}

void InertiaComputer::computeAtCNPs(std::vector<double> &inertia,
  NodePtr node)
{
  using morphotree::I32Point;

  AuxAttributes &nodeAuxAttrs = auxAttrs_[node->id()];

  nodeAuxAttrs.area += static_cast<double>(node->cnps().size());
  for (uint32 pidx : node->cnps()) {
    I32Point p = domain_.indexToPoint(pidx);
    nodeAuxAttrs.m10 += static_cast<double>(p.x());
    nodeAuxAttrs.m01 += static_cast<double>(p.y());
    nodeAuxAttrs.m20 += static_cast<double>(p.x() * p.x());
    nodeAuxAttrs.m02 += static_cast<double>(p.y() * p.y());
  }
}

void InertiaComputer::mergeToParent(std::vector<double> &inertia, 
  NodePtr node, NodePtr parent)
{
  AuxAttributes &nodeAuxAttrs = auxAttrs_[node->id()];
  AuxAttributes &parentAuxAttrs = auxAttrs_[parent->id()];

  parentAuxAttrs.area += nodeAuxAttrs.area;

  parentAuxAttrs.m10 += nodeAuxAttrs.m10;
  parentAuxAttrs.m01 += nodeAuxAttrs.m01;
  parentAuxAttrs.m20 += nodeAuxAttrs.m20;
  parentAuxAttrs.m02 += nodeAuxAttrs.m02;
}

void InertiaComputer::finalizeComputation(std::vector<double> &inertia,
  NodePtr node)
{
  const AuxAttributes &nodeAuxAttrs = auxAttrs_[node->id()];
  double &nodeInertia = inertia[node->id()];

  double xCentroid = nodeAuxAttrs.m10 / nodeAuxAttrs.area;
  double yCentroid = nodeAuxAttrs.m01 / nodeAuxAttrs.area;

  double xCentroid2 = xCentroid * xCentroid;
  double yCentroid2 = yCentroid * yCentroid;

  double M20 =  nodeAuxAttrs.m20 - (2.0 * nodeAuxAttrs.m10 * xCentroid) 
    + (nodeAuxAttrs.area * xCentroid2);

  double M02 =  nodeAuxAttrs.m02 - (2.0 * nodeAuxAttrs.m01 * yCentroid) 
    + (nodeAuxAttrs.area * yCentroid2);

  nodeInertia = M20 + M02; 
}

std::vector<double>computeInertia(const InertiaComputer::Box &domain, 
  const InertiaComputer::MTree &tree)
{
  return InertiaComputer(domain).computeAttribute(tree);
}