#include <morphotree/core/box.hpp>
#include <morphotree/core/alias.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>

#include <MaxDistComputer.hpp>

#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


#define ONLY_ATTRIBUTE_VALUE

int main(int argc, char *argv[])
{
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::Box;
  using morphotree::UI32Point;
  using morphotree::Adjacency;
  using morphotree::Adjacency8C;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using morphotree::buildMaxTree;
  using NodePtr = MTree::NodePtr;

  // check number of arguments from the command call
  if (argc < 3) {
    std::cerr << "usage error!\n";
    std::cerr << "usage: max_dist_attr_disp <image> <out_dir>\n";
    return -1;
  }

  // read image 
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);

  // convert to morphotree image format
  Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f{data, data + domain.numberOfPoints()};

  // build max-tree 
  std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
  MTree tree = buildMaxTree(f, adj);

  // compute attribute
  std::vector<uint32> maxDistAttr = computeMaxDistanceAttribute(domain, f, tree);

  tree.tranverse([&maxDistAttr, &domain, &argv](NodePtr node){
    #ifndef ONLY_ATTRIBUTE_VALUE
      std::vector<uint8> nodeImg(domain.numberOfPoints(), 0);
      for (uint32 pidx : node->reconstruct()) {
        nodeImg[pidx] = 255;
      }

      std::stringstream ss;
      ss << argv[2] << "/" << "nodeId=" << node->id()
        << "_maxDist=" << maxDistAttr[node->id()] << ".png";
      
      stbi_write_png(ss.str().c_str(), domain.width(), domain.height(), 1, nodeImg.data(), 0);
    #else
      std::cout << "max_dist[" << node->id() << "] = " << maxDistAttr[node->id()] << "\n";
    #endif
  });

  std::cout << "number of nodes: " << tree.numberOfNodes() << "\n";
  std::cout << "DONE\n";

  return 0;
}