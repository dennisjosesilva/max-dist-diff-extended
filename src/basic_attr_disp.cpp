#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <BasicAttributeComputer.hpp>

#include <iostream>
#include <string>
#include <functional>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, char *argv[]) 
{
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::int32;
  using morphotree::Adjacency;
  using morphotree::Adjacency8C;
  using morphotree::Box;
  using morphotree::buildMaxTree;
  using MTree = morphotree::MorphologicalTree<uint8>;
  using NodePtr = typename MTree::NodePtr;

  if (argc != 3) {
    std::cerr << "Error.\n"
              << "Usage: basic_attr_disp <image> <csv_out>\n";
    return -1;
  }

  // read input image 
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);
    
  Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f{data, data + domain.numberOfPoints()};

  std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
  MTree tree = buildMaxTree(f, adj);

  std::vector<BasicAttributes> attrs = computeBasicAttributes(domain, f, tree);

  std::ofstream fout{argv[2]};

  fout << "ID;"
       << "AREA;"
       << "VOLUME;"
       << "LEVEL;"
       << "MEAN_LEVEL;"
       << "VARIANCE_LEVEL;"
       << "WIDTH;"
       << "HEIGHT;"
       << "RECTANGULARITY;"
       << "RATIO_WB;"
       << "MOMENT_02;"
       << "MOMENT_20;"
       << "MOMENT_11;"
       << "INERTIA;"
       << "ORIENTATION;"
       << "LEN_MAJOR_AXIS;"
       << "LEN_MINOR_AXIS;"
       << "ECCENTRICITY;"
       << "COMPACTNESS\n";

  tree.traverseByLevel([&attrs, &fout](NodePtr node){
    const BasicAttributes &nattrs = attrs[node->id()];

    fout << node->id() << ";"
         << nattrs.area() << ";" 
         << nattrs.volume() << ";" 
         << nattrs.level() << ";" 
         << nattrs.meanLevel() << ";" 
         << nattrs.varianceLevel() << ";" 
         << nattrs.width() << ";" 
         << nattrs.height() << ";" 
         << nattrs.rectangularity() << ";" 
         << nattrs.ratioWH() << ";" 
         << nattrs.moment02() << ";" 
         << nattrs.moment20() << ";" 
         << nattrs.moment11() << ";" 
         << nattrs.inertia() << ";" 
         << nattrs.orientation() << ";" 
         << nattrs.lenMajorAxis() << ";" 
         << nattrs.lenMinorAxis() << ";" 
         << nattrs.eccentricity() << ";" 
         << nattrs.compactness() << "\n";
  });

  fout.close();

  std::cout << "DONE\n";

  return 0;
}
