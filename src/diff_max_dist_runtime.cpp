#include <iostream>

#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>

#include <MaxDistComputer.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <chrono>

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

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;
  using time_point = std::chrono::time_point<high_resolution_clock>;

  if (argc > 1) {
    int width, height, nchannels;
    uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 0);
    
    Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
    std::vector<uint8> f{ data, data + domain.numberOfPoints() };

    std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
    MTree tree = buildMaxTree(f, adj);

    time_point start = high_resolution_clock::now();
    std::vector<uint32> maxDist = computeMaxDistanceAttribute(domain, f, tree);
    time_point end = high_resolution_clock::now();

    milliseconds timeElapsed = duration_cast<milliseconds>(end - start);
    std::cout << "nnodes: " << tree.numberOfNodes() << "\n"
              << "width: "  << domain.width() << "\n"
              << "height: " << domain.height() << "\n"
              << "npixels: " << domain.numberOfPoints() << "\n"
              << "time elapsed: " << timeElapsed.count() << "\n";
  }
  else {
    std::cerr << "Error, it needs to receive a path for image file.\n";
  }

  return 0;
} 