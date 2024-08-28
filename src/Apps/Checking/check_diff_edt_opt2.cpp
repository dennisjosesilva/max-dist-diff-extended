#include <morphotree/core/box.hpp>
#include <morphotree/core/io.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>

#include <Morphology/Adjacency.hpp>
#include <Morphology/ErosionSerial.hpp>

#include <MaxDist/MaxDistComputerOpt2.hpp>
#include <MaxDist/MaxDistComputer.hpp>
#include <iostream>

#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#include <chrono>

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::printImageIntoConsoleWithCast;
  using MTree = MaxDistComputerOpt2::MTree;
  using NodePtr = MaxDistComputerOpt2::NodePtr;
  using morphotree::buildMaxTree;
  using morphotree::Adjacency8C;

  using std::chrono::high_resolution_clock;
  using std::chrono::duration_cast;
  using std::chrono::duration;
  using std::chrono::milliseconds;
  using time_point = std::chrono::time_point<high_resolution_clock>;

  std::vector<uint8> f;
  Box domain;

  // check number of arguments from the command call
  if (argc < 2) {
    f = {
      0, 0, 0, 0, 0, 0, 0,
      0, 4, 4, 4, 7, 7, 7,
      0, 7, 7, 4, 7, 4, 7,
      0, 7, 4, 4, 7, 4, 7,
      0, 4, 4, 4, 7, 4, 7,
      0, 7, 7, 4, 7, 7, 7,
      0, 0, 0, 0, 0, 0, 0
    };
    
    domain = Box::fromSize({7, 7});
  }
  else {
    int width, height, nchannels;
    uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);
    domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
    f = std::vector<uint8>(data, data + domain.numberOfPoints());
  }

  MTree tree = buildMaxTree(f, std::make_shared<Adjacency8C>(domain));

  // compute maximum distance transform with time duration computed
  time_point start = high_resolution_clock::now();
  std::vector<uint32> maxDist = computeMaxDistanceAttribute(domain, f, tree);
  time_point end = high_resolution_clock::now();

  milliseconds timeElapsedMaxDist = duration_cast<milliseconds>(end - start);

  // compute maximum distance transform with time duration computed
  start = high_resolution_clock::now();
  std::vector<uint32> maxDistOpt = computeMaxDistanceAttributeOpt2(domain, f, tree);
  end = high_resolution_clock::now();

  milliseconds timeElapsedMaxDistOpt = duration_cast<milliseconds>(end - start);

  if (argc < 2) {
    tree.tranverse([&maxDist, &maxDistOpt, &domain](NodePtr node) {
      std::vector<bool> nodeImg = node->reconstruct(domain);
      std::cout << "maxDist[" << node->id() << "] = " << maxDist[node->id()] << "\n";
      std::cout << "maxDistOpt[" << node->id() << "] = " << maxDistOpt[node->id()] << "\n";
      printImageIntoConsoleWithCast<int>(nodeImg, domain);
      std::cout << std::endl;
    });
  }
  else {
    bool checked = true;
    tree.tranverse([&checked, &domain, &maxDist, &maxDistOpt](NodePtr node){
      uint32 nodeMaxDist = maxDist[node->id()];
      uint32 nodeMaxDistOpt = maxDistOpt[node->id()];

      if (nodeMaxDist != nodeMaxDistOpt) {
        std::cout << "ERROR: node.id= " << node->id() << ", "
                  << "MaxDist[" << node->id() << "] = " << nodeMaxDist << ", "
                  << "MaxDistOpt[" << node->id() << "] = " << nodeMaxDistOpt << "\n";
        checked = false;
      }
    });

    if (checked)
      std::cout << "\nCHECKING DONE SUCCESSFULLY\n";
    else 
      std::cout << "\nCHECKING DONE FAILED\n";

    std::cout << "MaxDist time elapsed: " << timeElapsedMaxDist.count() << "\n";
    std::cout << "MaxDistOpt time elapsed: " << timeElapsedMaxDistOpt.count() << "\n";
  }

  return 0;
}
