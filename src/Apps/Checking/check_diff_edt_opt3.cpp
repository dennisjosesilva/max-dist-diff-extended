#include <morphotree/core/box.hpp>
#include <morphotree/core/io.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>

#include <Morphology/Adjacency.hpp>
#include <Morphology/ErosionSerial.hpp>

#include <MaxDist/MaxDistComputerOpt3.hpp>
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
  using MTree = MaxDistComputerOpt3::MTree;
  using NodePtr = MaxDistComputerOpt3::NodePtr;
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
  std::vector<uint32> maxDistOpt3 = computeMaxDistanceAttributeOpt3(domain, f, tree);
  end = high_resolution_clock::now();

  milliseconds timeElapsedMaxDistOpt3 = duration_cast<milliseconds>(end - start);

  if (argc < 2) {
    tree.tranverse([&maxDist, &maxDistOpt3, &domain](NodePtr node){
      std::vector<bool> nodeImg = node->reconstruct(domain);
      std::cout << "maxDist[" << node->id() << "] = " << maxDist[node->id()] << "\n";
      std::cout << "maxDistOpt3[" << node->id() << "] = " << maxDistOpt3[node->id()] << "\n";
      printImageIntoConsoleWithCast<int>(nodeImg, domain);
      std::cout << "\n";
    });
  }
  else {
    bool checked = true;
    tree.tranverse([&checked, &domain, &maxDist, &maxDistOpt3](NodePtr node){
      uint32 nodeMaxDist = maxDist[node->id()];
      uint32 nodeMaxDistOpt3 = maxDistOpt3[node->id()];

      if (nodeMaxDist != nodeMaxDistOpt3) {
        std::cout << "ERROR: node.id= " << node->id() << ", "
                  << "MaxDist[" << node->id() << "] = " << nodeMaxDist << ", "
                  << "MaxDistOpt3[" << node->id() << "] = " << nodeMaxDistOpt3 << "\n";
        checked = false;
      }
    });

    if (checked)
      std::cout << "\nCHECKING DONE SUCCESSFULLY\n";
    else
      std::cout << "\nCHECKING DONE FAILED\n";

    std::cout << "MaxDist time elapsed: " << timeElapsedMaxDist.count() << "\n";
    std::cout << "MaxDistOpt3 time elapsed: " << timeElapsedMaxDistOpt3.count() << "\n";
  }

  return 0;
}