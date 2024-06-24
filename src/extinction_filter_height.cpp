#include <morphotree/core/box.hpp>
#include <morphotree/core/alias.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency4c.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/filtering/extinctionFilter.hpp>

#include <iostream>
#include <limits>

#include <MaxDistComputer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

//#define APPDEBUG


// Height Attribute Computer
namespace mt = morphotree;
template<class ValueType>
class HeightComputer : public mt::AttributeComputer<ValueType, ValueType>
{
public:
  using AttrType = ValueType;
  using TreeType = typename mt::AttributeComputer<ValueType, ValueType>::TreeType;
  using NodePtr = typename TreeType::NodePtr;

  std::vector<ValueType> initAttributes(const TreeType &tree) override;
  void computeInitialValue(std::vector<ValueType> &attr, NodePtr node) override;
  void mergeToParent(std::vector<ValueType> &attr, NodePtr node, NodePtr parent) override;
  void finaliseComputation(std::vector<ValueType> &attr, NodePtr node) override;

private:
  std::vector<ValueType> highest_;
};

template<class ValueType>
std::vector<ValueType> HeightComputer<ValueType>::initAttributes(
  const HeightComputer<ValueType>::TreeType &tree)
{
  highest_.resize(tree.numberOfNodes(), std::numeric_limits<ValueType>::min());
  return std::vector<ValueType>(tree.numberOfNodes(), 
    std::numeric_limits<ValueType>::min());
}

template<class ValueType>
void HeightComputer<ValueType>::computeInitialValue(std::vector<ValueType> &attr,
  NodePtr node)
{
  highest_[node->id()] = std::max(highest_[node->id()], node->level());  
}

template<class ValueType>
void HeightComputer<ValueType>::mergeToParent(std::vector<ValueType> &attr, 
  NodePtr node, NodePtr parent)
{
  highest_[parent->id()] = std::max(highest_[parent->id()], highest_[node->id()]);
}

template<class ValueType>
void HeightComputer<ValueType>::finaliseComputation(std::vector<ValueType> &attr,
  NodePtr node)
{
  attr[node->id()] = highest_[node->id()] - node->level();
}

int main(int argc, char *argv[])
{
  // import morphotree types
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::Box;
  using morphotree::UI32Point;
  using morphotree::MorphologicalTree;
  using morphotree::Adjacency4C;
  using morphotree::Adjacency8C;
  using morphotree::Adjacency;
  using morphotree::buildMaxTree;
  using morphotree::AreaComputer;
  using morphotree::ExtinctionValueLeavesComputer;
  using NodePtr = MorphologicalTree<uint8>::NodePtr;
  using ExtinctionValueComputer = ExtinctionValueLeavesComputer<uint8, uint32>;
  using ExtinctionValueMapType = typename ExtinctionValueComputer::MapType;
  using morphotree::extinctionFilter;
  using morphotree::iextinctionFilter;

  uint32 nleaves = 15;
  uint32 tarea = 0;

  // check number of arguments from the command call
  if (argc < 4) {
    std::cerr << "usage error!\n";
    std::cerr << "usage: extinction_filter_max_dist <image> <out_img> <area> [nleaves] \n";
    return -1;
  }

  if (argc > 4) {
    nleaves = atoi(argv[4]);
  }

  // get area threshold
  tarea = atoi(argv[3]);

  // read image
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);
  //stbi_image_free(data);

  // convert to morphotree image format
  Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f(data, data + domain.numberOfPoints());

  // build max-tree
  std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
  MorphologicalTree<uint8> maxtree = buildMaxTree(f, adj);

  // perform area filter if needed.
  if (tarea > 0) {
    std::vector<uint32> area = AreaComputer<uint8>().computeAttribute(maxtree);
    maxtree.idirectFilter([&area, tarea](NodePtr node) { return area[node->id()] > tarea; });
  }

  std::cout << "Number of nodes: " << maxtree.numberOfNodes() << std::endl;

  // Extract leaves
  std::vector<NodePtr> leaves;
  maxtree.tranverse([&leaves](NodePtr node){
    if (node->children().empty()) // node is leaf
      leaves.push_back(node);
  });

  // Extinction value
  //std::vector<uint32> maxDist = AreaComputer<uint8>().computeAttribute(maxtree);
  std::vector<uint8> heightExt = HeightComputer<uint8>().computeAttribute(maxtree);
  
  #ifdef APPDEBUG
    //print extinction value 
    ExtinctionValueMapType extVals = ExtinctionValueComputer().compute(maxtree, heightExt);
    for (auto& leafVal : extVals) {
      std::cout << "height[" << leafVal.first << "] = " << leafVal.second << "\n";
    }
  #endif

  // perform extinction filter
  iextinctionFilter(maxtree, heightExt, nleaves);

  std::cout << "FILTER\n";
  std::cout << "Number of nodes: " << maxtree.numberOfNodes() << std::endl;

  // record filtered image into the disk
  std::vector<uint8> out = maxtree.reconstructImage();
  stbi_write_png(argv[2], domain.width(), domain.height(), 1, out.data(), 0);
  stbi_image_free(data);

  return 0;
}