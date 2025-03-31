#include <morphotree/core/box.hpp>
#include <morphotree/core/alias.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include "morphotree/tree/treeOfShapes/tos.hpp"
#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency4c.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/filtering/extinctionFilter.hpp>

#include <iostream>

#include <MaxDist/MaxDistComputer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// #define APPDEBUG

int main(int argc, char *argv[])
{
  // import morphotree types
  using morphotree::uint8;
  using morphotree::uint32;
  using I32Point = morphotree::I32Point;
  using morphotree::Box;
  using morphotree::UI32Point;
  using morphotree::MorphologicalTree;
  using morphotree::Adjacency4C;
  using morphotree::Adjacency8C;
  using morphotree::Adjacency;
  using morphotree::buildMaxTree;
  using morphotree::buildTreeOfShapes;
  using morphotree::AreaComputer;
  using morphotree::ExtinctionValueLeavesComputer;
  using NodePtr = MorphologicalTree<uint8>::NodePtr;
  using ExtinctionValueComputer = ExtinctionValueLeavesComputer<uint8, uint32>;
  using ExtinctionValueMapType = typename ExtinctionValueComputer::MapType;
  using morphotree::extinctionFilter;
  using morphotree::iextinctionFilter;

  uint32 maxDist = 0;
  uint32 treeType = 0;

  // check number of arguments from the command call
  if (argc < 4) {
    std::cerr << "usage error!\n";
    std::cerr << "usage: direct_filter_max_dist <image> <out_img> <tree-type: 0=maxtree; 1=mintree; 2=ToS> <maxDist> \n";
    return -1;
  }

  if (argc > 3) {
    maxDist = atoi(argv[4]);
  }

  // get area threshold
  treeType = atoi(argv[3]);

  // read image
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);
  //stbi_image_free(data);

  // convert to morphotree image format
  Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f(data, data + domain.numberOfPoints());

  std::shared_ptr<MorphologicalTree<uint8>> tree = nullptr;
  if (treeType == 0) {
    std::cout << "Tree type: MaxTree" << std::endl;
    auto adj = std::make_shared<Adjacency8C>(domain);
    tree = std::make_shared<MorphologicalTree<uint8>>(buildMaxTree(f, adj));
  } else if (treeType == 1) {
    std::cout << "Tree type: MinTree" << std::endl;
    auto adj = std::make_shared<Adjacency8C>(domain);
    tree = std::make_shared<MorphologicalTree<uint8>>(buildMinTree(f, adj));
  } else {
    std::cout << "Tree type: Tree of shapes" << std::endl;
    I32Point pInfty = {0, 0};
    tree = std::make_shared<MorphologicalTree<uint8>>(buildTreeOfShapes(domain, f, pInfty));
  }
  
  std::cout << "Number of nodes: " << tree->numberOfNodes() << std::endl;
  std::vector<uint32> maxDistAttrs = computeMaxDistanceAttribute(domain, f, *tree);
  tree->idirectFilter([&maxDistAttrs, &maxDist](NodePtr node) {
      return maxDistAttrs[node->id()] > maxDist;
  });
  
  std::cout << "Filtered tree - number of nodes: " << tree->numberOfNodes() << std::endl;

  // record filtered image into the disk
  std::vector<uint8> out = tree->reconstructImage();
  stbi_write_png(argv[2], domain.width(), domain.height(), 1, out.data(), 0);
  stbi_image_free(data);

  return 0;
}