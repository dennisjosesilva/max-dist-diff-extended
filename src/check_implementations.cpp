#include <iostream>

#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>

#include <MaxDistComputer.hpp>
#include <NonDiffMaxDistComputer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define APPDEBUG

#ifdef APPDEBUG
  #include <fstream>
#endif

int main(int argc, char *argv[])
{
  using morphotree::uint8;
  using morphotree::uint32;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;
  using morphotree::Adjacency;
  using morphotree::Adjacency8C;
  using morphotree::UI32Point;
  using morphotree::Box;

  using morphotree::buildMaxTree;

  if (argc > 1) {
    int width, height, nchannels;
    uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 0);

    Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
    std::vector<uint8> f( data, data + domain.numberOfPoints() );

    std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
    MTree tree = buildMaxTree(f, adj);

    std::vector<uint32> diffDist = computeMaxDistanceAttribute(domain, f, tree);
    std::vector<uint32> nonDiffDist = computeNonDiffMaxDistanceAttribute(domain, f, tree);

    #ifdef APPDEBUG
      std::ofstream out{"dist.csv"};
      out << "node.id;non_diff_distance;diff_distance\n";
    #endif

    tree.traverseByLevel([&](NodePtr node){
      if (diffDist[node->id()] != nonDiffDist[node->id()]) {
        
        std::cout << "nonDiffDist[" << node->id() << "] = " << nonDiffDist[node->id()] << ", "
                  << "diffDist[" << node->id() << "] = " << diffDist[node->id()] << "\n";
      }
      #ifdef APPDEBUG
          out << node->id() << ";" << nonDiffDist[node->id()] << ";" 
              << diffDist[node->id()] <<"\n";
      #endif
    });

    #ifdef APPDEBUG
      out.close();
    #endif
  }

  return 0;
}