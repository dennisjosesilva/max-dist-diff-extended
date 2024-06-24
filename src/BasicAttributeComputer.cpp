#include <BasicAttributeComputer.hpp>
#include <algorithm>
#include <cmath>

// ========================================================
// BASIC ATTRIBUTES
// ========================================================
BasicAttributes::BasicAttributes():
  area_{0.0f},
  volume_{0.0f},
  level_{0.0f},
  meanLevel_{0.0f},
  varianceLevel_{0.0f},
  width_{0.0f},
  height_{0.0f},
  rectangularity_{0.0f},
  ratioWH_{0.0f},
  moment02_{0.0f},
  moment20_{0.0f},
  moment11_{0.0f},
  inertia_{0.0f},
  lenMajorAxis_{0.0f},
  lenMinorAxis_{0.0f},
  eccentricity_{0.0f},
  compactness_{0.0f}
{}

BasicAttributeComputer::AuxAttributes::AuxAttributes(const Box &domain)
  : xmax{domain.left()},
    xmin{domain.right()},
    ymin{domain.bottom()},
    ymax{domain.top()},
    m10{0},
    m01{0},
    m20{0},
    m02{0},
    m11{0},
    graySquared{0}
{}

// =========================================================
// MAIN CLASS
// =========================================================
BasicAttributeComputer::BasicAttributeComputer(
  const Box &domain, const std::vector<uint8> &f)
  : domain_{domain}, f_{f}
{ }

std::vector<BasicAttributes> 
  BasicAttributeComputer::computeAttribute(const MTree &tree)
{
  std::vector<AuxAttributes> auxAttrs(tree.numberOfNodes(), AuxAttributes{domain_});
  std::vector<BasicAttributes> attrs(tree.numberOfNodes());
  
  tree.tranverse([&auxAttrs, &attrs, this](NodePtr node){
    computeAtCNPs(auxAttrs, attrs, node);

    if (node->parent() != nullptr) {
      mergeToParent(auxAttrs, attrs, node, node->parent());
    }

    // finalise computation
    finalizeComputation(auxAttrs, attrs, node);
  });

  return attrs;
}

void BasicAttributeComputer::computeAtCNPs(std::vector<AuxAttributes> &auxAttrs, 
  std::vector<BasicAttributes> &attrs, NodePtr node)
{
  using morphotree::I32Point;

  // Recover attributes
  BasicAttributes &nodeAttrs = attrs[node->id()];
  AuxAttributes &nodeAuxAttrs = auxAttrs[node->id()];

  nodeAttrs.area_ += node->cnps().size();
  nodeAttrs.volume_ += node->cnps().size() * static_cast<int>(node->level());
  nodeAttrs.level_ = node->level();

  nodeAuxAttrs.graySquared += std::pow(static_cast<unsigned long>(node->level()), 2) * node->cnps().size(); // computes \sum f(p)^2

  for (uint32 pidx : node->cnps())   {
    I32Point p = domain_.indexToPoint(pidx);
    nodeAuxAttrs.xmin = std::min(p.x(), nodeAuxAttrs.xmin);
    nodeAuxAttrs.xmax = std::max(p.x(), nodeAuxAttrs.xmax);
    nodeAuxAttrs.ymin = std::min(p.y(), nodeAuxAttrs.ymin);
    nodeAuxAttrs.ymax = std::max(p.y(), nodeAuxAttrs.ymax);
    nodeAuxAttrs.m10 += p.x();
    nodeAuxAttrs.m01 += p.y();
    nodeAuxAttrs.m11 += p.x() * p.y();
    nodeAuxAttrs.m20 += p.x() * p.x();
    nodeAuxAttrs.m02 += p.y() * p.y();
  }
}

void BasicAttributeComputer::mergeToParent(std::vector<AuxAttributes> &auxAttrs, 
  std::vector<BasicAttributes> &attrs, NodePtr node, NodePtr parent)
{
  AuxAttributes &nodeAuxAttrs =  auxAttrs[node->id()];
  AuxAttributes &parentAuxAttrs = auxAttrs[parent->id()];

  BasicAttributes &nodeAttrs = attrs[node->id()];
  BasicAttributes &parentAttrs = attrs[parent->id()];

  parentAttrs.area_ += nodeAttrs.area_;
  parentAttrs.volume_ += nodeAttrs.volume_;

  parentAuxAttrs.graySquared += nodeAuxAttrs.graySquared;

  parentAuxAttrs.xmin = std::min(parentAuxAttrs.xmin, nodeAuxAttrs.xmin);
  parentAuxAttrs.xmax = std::max(parentAuxAttrs.xmax, nodeAuxAttrs.xmax);
  parentAuxAttrs.ymin = std::min(parentAuxAttrs.ymin, nodeAuxAttrs.ymin);
  parentAuxAttrs.ymax = std::max(parentAuxAttrs.ymax, nodeAuxAttrs.ymax);

  parentAuxAttrs.m01 += nodeAuxAttrs.m01;
  parentAuxAttrs.m10 += nodeAuxAttrs.m10;
  parentAuxAttrs.m11 += nodeAuxAttrs.m11;
  parentAuxAttrs.m20 += nodeAuxAttrs.m20;
  parentAuxAttrs.m02 += nodeAuxAttrs.m02;
}

void BasicAttributeComputer::finalizeComputation(std::vector<AuxAttributes> &auxAttrs,
  std::vector<BasicAttributes> &attrs, NodePtr node)
{
  BasicAttributes &nodeAttrs = attrs[node->id()];
  AuxAttributes &nodeAuxAttrs = auxAttrs[node->id()];

  float area = nodeAttrs.area_;
  float volume = nodeAttrs.volume_;
  float width = nodeAuxAttrs.xmax - nodeAuxAttrs.xmin + 1;
  float height = nodeAuxAttrs.ymax - nodeAuxAttrs.ymin + 1;

  nodeAttrs.meanLevel_ = volume / area;
  nodeAttrs.varianceLevel_ = (nodeAuxAttrs.graySquared / area) - std::pow((volume / area), 2); // variance
  nodeAttrs.width_ = width;
  nodeAttrs.height_ = height;
  nodeAttrs.rectangularity_ = area / (width * height);
  nodeAttrs.ratioWH_ =  static_cast<float>(std::max(width, height))  / static_cast<float>(std::min(width, height));
  
  // ======================
  // Central moments
  // ======================
  float xCentroid = nodeAuxAttrs.m10 / area;
  float yCentroid = nodeAuxAttrs.m01 / area;

  nodeAttrs.moment20_ = nodeAuxAttrs.m20 - (nodeAuxAttrs.m10 * xCentroid);
  nodeAttrs.moment02_ = nodeAuxAttrs.m02 - (nodeAuxAttrs.m01 * yCentroid);
  nodeAttrs.moment11_ = nodeAuxAttrs.m11 - (xCentroid * nodeAuxAttrs.m01);

  float moment20 = nodeAttrs.moment20_;
  float moment02 = nodeAttrs.moment02_;
  float moment11 = nodeAttrs.moment11_;

  if (area > 1) {
    float a1 = moment20 + moment02 + std::sqrt(std::pow(moment20 - moment02, 2) + 4.0f * std::pow(moment11, 2));
    float a2 = moment20 + moment02 - std::sqrt(std::pow(moment20 - moment02, 2) + 4.0f * std::pow(moment11, 2));
    nodeAttrs.inertia_ = normMoment(area, moment02, 0, 2) + normMoment(area, moment20, 2, 0); //hu
    nodeAttrs.orientation_ = 0.5f * std::atan2(2.f * moment11, moment20 - moment02);
    
    nodeAttrs.lenMajorAxis_ = std::sqrt((2 * a1) / area);
    nodeAttrs.lenMinorAxis_ = std::sqrt((2 * a2) / area);

    nodeAttrs.eccentricity_ = (a2 != 0 ? a1 / a2 : a1 / 0.1f);

    nodeAttrs.compactness_ = (1.0f / (2.0f * M_PI)) * (area / (moment20 + moment02)); 
  }
}

float BasicAttributeComputer::normMoment(float area, float moment, int p, int q) const
{
  return moment / std::pow( area, (p + q + 2.0) / 2.0);
}

// =====================================================================================
// UTIL FUNCTION
// =====================================================================================
std::vector<BasicAttributes> computeBasicAttributes(
  const BasicAttributeComputer::Box &domain,
  const std::vector<BasicAttributeComputer::uint8> &f, 
  const BasicAttributeComputer::MTree &tree)
{
  return BasicAttributeComputer(domain, f).computeAttribute(tree);
}
