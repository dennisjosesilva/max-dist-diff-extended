#include <morphotree/core/box.hpp>
#include <morphotree/core/alias.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/adjacency/adjacency4c.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/attributes/extinctionValues/ExtinctionValueLeavesComputer.hpp>
#include <morphotree/filtering/extinctionFilter.hpp>

#include <InertiaComputer.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

// #define APPDEBUG

int main(int argc, char *argv[])
{
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::Box;
  using morphotree::UI32Point;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using morphotree::Adjacency4C;
  using morphotree::Adjacency8C;
  using morphotree::Adjacency;
  using morphotree::buildMaxTree;
  using morphotree::AreaComputer;
  using morphotree::ExtinctionValueLeavesComputer;
  using NodePtr = MTree::NodePtr;
  using ExtinctionValueComputer = ExtinctionValueLeavesComputer<uint8, double>;
  using ExtinctionValueMapType = typename ExtinctionValueComputer::MapType;
  using morphotree::extinctionFilter;
  using morphotree::iextinctionFilter;

  uint32 nleaves = 15;
  uint32 tarea = 0;

  // check number of arguments from the command call
  if (argc < 4) 
  {
    std::cerr << "Usage error!\n";
    std::cerr << "usage: extinction_filter_inertia <image> <image_out> <area> [nleaves]";
    return -1;
  }

  if (argc > 4)
    nleaves = atoi(argv[4]);

  // get area threshold
  tarea = atoi(argv[3]);

  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);

  // convert to morphotree image format
  Box domain = Box::fromSize(
    {static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f(data, data + domain.numberOfPoints());

  // build max-tree
  std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
  MTree maxtree = buildMaxTree(f, adj);

  // perform area filter if needed
  if (tarea > 0) {
    std::vector<uint32> area = AreaComputer<uint8>().computeAttribute(maxtree);
    maxtree.idirectFilter([&area, tarea](NodePtr node) { return area[node->id()] > tarea; });      
  }

  std::cout << "Number of nodes: " << maxtree.numberOfNodes() << std::endl;

  // Extract leaves
  std::vector<NodePtr> leaves;
  maxtree.tranverse([&leaves](NodePtr node) {
    if (node->children().empty()) // node is leaf
      leaves.push_back(node);
  });

  // Extinction value
  std::vector<double> inertia = computeInertia(domain, maxtree);

  #ifdef APPDEBUG
    ExtinctionValueMapType extVals = ExtinctionValueComputer().compute(maxtree, inertia);
    for (auto &leftVal : extVals) {
      std::cout << "inertia[" << leftVal.first << "] = " << leftVal.second << "\n"; 
    }
  #endif

  iextinctionFilter(maxtree, inertia, nleaves);

  std::cout << "FILTER\n";
  std::cout << "Number of nodes: " << maxtree.numberOfNodes() << std::endl;

  // record filtered into the disk
  std::vector<uint8> out = maxtree.reconstructImage();
  stbi_write_png(argv[2], domain.width(), domain.height(), 1, out.data(), 0);
  stbi_image_free(data);

  return 0;
}