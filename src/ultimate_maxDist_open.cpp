#include <morphotree/core/box.hpp>
#include <morphotree/core/alias.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency4c.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <morphotree/attributes/areaComputer.hpp>
#include <morphotree/residues/UltimateAttributeOpening.hpp>

#include <iostream>
#include <string>

#include <MaxDistComputer.hpp>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

#define APPDEBUG


int main(int argc, char *argv[])
{
  // import morphotree types
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::Box;
  using morphotree::UI32Point;
  using morphotree::MorphologicalTree;
  using morphotree::Adjacency8C;
  using morphotree::Adjacency;
  using morphotree::buildMaxTree;
  using NodePtr = MorphologicalTree<uint8>::NodePtr;
  using morphotree::AreaComputer;
  using morphotree::UltimateAttributeOpening;

  // check number of arguments from the command call
  if (argc < 4) {
    std::cerr << "usage error!\n";
    std::cerr << "usage: ultimate_alongation_open <image> <out_img> <maxCriterion>\n";
    return -1;
  }
  // read image
  int width, height, nchannels;
  uint8 *data = stbi_load(argv[1], &width, &height, &nchannels, 1);

  // convert to morphotree image format
  Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f(data, data + domain.numberOfPoints());

  // build max-tree
  std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
  MorphologicalTree<uint8> maxtree = buildMaxTree(f, adj);
  std::cout << "Number of nodes: " << maxtree.numberOfNodes() << std::endl;

  /*int tarea = 30;
  std::vector<uint32> area = AreaComputer<uint8>().computeAttribute(maxtree);
  maxtree.idirectFilter([&area, tarea](NodePtr node){return area[node->id()] >= tarea;});
  std::cout << "Number of nodes: " << maxtree.numberOfNodes() << std::endl;
  */

  // attributes
  std::vector<uint32> maxDist = computeMaxDistanceAttribute(domain, f, maxtree);
  int maxCriterion = 0;
  for (int id=0; id < maxtree.numberOfNodes(); id++) {
    if(maxDist[id] > maxCriterion)
      maxCriterion = maxDist[id];
  }
  std::cout << "computeMaxDistanceAttribute [OK] " << maxCriterion << std::endl;

  maxCriterion = std::stoi(argv[3]);
  UltimateAttributeOpening uao(maxtree, maxCriterion, maxDist);
  std::vector<uint8> imgMaxConstrast = uao.getMaxConstrastImage();
  std::vector<uint8> imgRGBAssociated = uao.getAssociatedColorImage();
	std::cout << "computeUAO [OK]" << std::endl;	
  
  std::string s(argv[2]);
  s = s.substr(s.find_last_of("/")+1);
  std::string name = s.substr(0, s.length()-4); 
  std::string name_maxConstrast = name + "_maxConstrast.png";
  std::string name_associated = name +  "_associated.png";         

  // record filtered image into the disk
  stbi_write_png(name_maxConstrast.c_str(), domain.width(), domain.height(), 1, imgMaxConstrast.data(), 0);
  stbi_write_png(name_associated.c_str(), domain.width(), domain.height(), 3, imgRGBAssociated.data(), 0);
  stbi_image_free(data);
  
  return 0;
}


