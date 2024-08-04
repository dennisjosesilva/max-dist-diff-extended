#include <vector>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/core/box.hpp>
#include <morphotree/attributes/attributeComputer.hpp>


class BasicAttributes
{
public:
  BasicAttributes();

  float area() const { return area_; }
  float volume() const { return volume_; }
  float level() const { return level_; }
  float meanLevel() const { return meanLevel_; }
  float varianceLevel() const { return varianceLevel_; }
  float width() const { return width_; }
  float height() const { return height_; }
  float rectangularity() const { return rectangularity_; }
  float ratioWH() const { return ratioWH_; }
  float moment02() const { return moment02_; }
  float moment20() const { return moment20_; }
  float moment11() const { return moment11_; }
  float inertia() const { return inertia_; }
  float orientation() const { return orientation_; }
  float lenMajorAxis() const { return lenMajorAxis_; }
  float lenMinorAxis() const { return lenMinorAxis_; }
  float eccentricity() const { return eccentricity_; }
  float compactness() const { return compactness_; }

private:
  float area_;
  float volume_;
  float level_;
  float meanLevel_;
  double varianceLevel_;
  float width_;
  float height_;
  float rectangularity_;
  float ratioWH_;
  float moment02_;
  float moment20_;
  float moment11_;
  float inertia_;
  float orientation_;
  float lenMajorAxis_;
  float lenMinorAxis_;
  float eccentricity_;
  float compactness_;

  friend class BasicAttributeComputer;
};


class BasicAttributeComputer
{
public:
  using uint8 = morphotree::uint8;
  using uint32 = morphotree::uint32;
  using Box = morphotree::Box;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;
  using AttributeComputer = morphotree::AttributeComputer<BasicAttributes, uint8>;

  BasicAttributeComputer(const Box &domain, const std::vector<uint8> &f);

  std::vector<BasicAttributes> computeAttribute(const MTree &tree);

private:
  const Box &domain_;
  const std::vector<uint8> &f_;

private:
  struct AuxAttributes
  {
    AuxAttributes(const Box &domain);

    int xmax;
    int ymax;
    int xmin;
    int ymin;
    unsigned long m10;
    unsigned long m01;
    unsigned long m20;
    unsigned long m02;
    unsigned long m11;
    unsigned long graySquared;
  }; 

private:
  void computeAtCNPs(std::vector<AuxAttributes> &auxAttrs, std::vector<BasicAttributes> &attrs,
    NodePtr node);

  void mergeToParent(std::vector<AuxAttributes> &auxAttrs, std::vector<BasicAttributes> &attrs,
    NodePtr node, NodePtr parent);

  void finalizeComputation(std::vector<AuxAttributes> &auxAttrs, std::vector<BasicAttributes> &attrs,
    NodePtr node);

  float normMoment(float area, float moment, int p, int q) const;
};



std::vector<BasicAttributes> computeBasicAttributes(
  const BasicAttributeComputer::Box &domain,
  const std::vector<BasicAttributeComputer::uint8> &f, 
  const BasicAttributeComputer::MTree &tree);
