#include <morphotree/tree/mtree.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>
#include <BasicAttributeComputer.hpp>

#include <iostream>
#include <string>
#include <functional>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

void help()
{
  std::cout << "usage: \n"            
            << "direct_filter <in_img> <out_img> <attr> <op> <value>\n\n"
            << "\tApply a director filter in <in_img> by keep only the nodes that\n"
            << "\thave value of <attr> less than or greater than (<op>) a <value>.\n"
            << "\nparams\n--------------------------------------------------------\n"
            << "\t<in_img>: (str) path for the input image\n"
            << "\t<out_img>: (str) path for the output (filtered) image in png\n"
            << "\t<attr>: (str) name of the attribute:\n"
            << "\t\tarea,\n"
            << "\t\tvolume,\n"
            << "\t\tlevel,\n"
            << "\t\tmean_level,\n"
            << "\t\tlevel_variance,\n"
            << "\t\tbox_width,\n"
            << "\t\tbox_height,\n"
            << "\t\trectangularity,\n"
            << "\t\tratioWH,\n"
            << "\t\tmoment20,\n"
            << "\t\tmoment02,\n"
            << "\t\tmoment11,\n"
            << "\t\tinertia,\n"
            << "\t\torientation,\n"
            << "\t\tlength_major_axis,\n"
            << "\t\tlength_minor_axis,\n"
            << "\t\teccentricity,\n"
            << "\t\tcompactness.\n"
            << "\t<op>: (char) Operation - use 'g' for greater than, 'l' for lower than, or '=' to equals to\n"
            << "\t<value>: (float) threshold value \n";
}

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

  if (argc == 2 && std::string(argv[1]) == "-h") {
    help();
    return 0;
  }
  if (argc != 6) {
    std::cerr << "Usage Error!";
    help();
    return -1;
  }
  
  // parse cmd parameters
  std::string inImg{argv[1]};
  std::string outImg{argv[2]};
  std::string attr{argv[3]};
  std::function<bool (float, float)> op;
  float thresholdValue = atof(argv[5]);

  if (argv[4][0] == 'g') 
    op = [](float lvalue, float rvalue) { return lvalue > rvalue; };
  else if (argv[4][0] == 'l')
    op = [](float lvalue, float rvalue) { return lvalue < rvalue; };
  else if (argv[4][0] == '=')
    op = [](float lvalue, float rvalue) { return std::abs(lvalue - rvalue) < 0.0001; };
  else {
    std::cerr << "ERROR: Invalid operation: " << argv[4] << "\n";
  }

  
  // read input image 
  int width, height, nchannels;
  uint8 *data = stbi_load(inImg.c_str(), &width, &height, &nchannels, 1);
    
  Box domain = Box::fromSize({static_cast<uint32>(width), static_cast<uint32>(height)});
  std::vector<uint8> f{data, data + domain.numberOfPoints()};

  // create max-tree
  std::shared_ptr<Adjacency> adj = std::make_shared<Adjacency8C>(domain);
  MTree tree = buildMaxTree(f, adj);

  std::cout << "number of nodes: " << tree.numberOfNodes() << "\n";
  std::cout << "===== performing filter =======" << "\n";

  // compute attributes
  std::vector<BasicAttributes> attrs = computeBasicAttributes(domain, f, tree);

  // perform filter
  if (attr == "area") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].area(), thresholdValue);
    });  
  }
  else if (attr == "volume") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].volume(), thresholdValue);
    });  
  }
  else if (attr == "level") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].level(), thresholdValue);
    });  
  }
  else if (attr == "mean_level") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].meanLevel(), thresholdValue);
    });  
  }
  else if (attr == "level_variance") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].varianceLevel(), thresholdValue);
    });  
  }
  else if (attr == "box_width") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].width(), thresholdValue);
    });  
  }
  else if (attr == "box_height") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].height(), thresholdValue);
    });  
  }
  else if (attr == "rectangularity") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].rectangularity(), thresholdValue);
    });  
  }
  else if (attr == "ratioWH") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].ratioWH(), thresholdValue);
    });  
  }
  else if (attr == "moment20") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].moment20(), thresholdValue);
    });  
  }
  else if (attr == "moment02") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].moment02(), thresholdValue);
    });  
  }
  else if (attr == "moment11") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].moment11(), thresholdValue);
    });  
  }
  else if (attr == "inertia") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].inertia(), thresholdValue);
    });  
  }
  else if (attr == "orientation") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].orientation(), thresholdValue);
    });  
  }
  else if (attr == "length_major_axis") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].lenMajorAxis(), thresholdValue);
    });  
  }
  else if (attr == "length_minor_axis") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].lenMinorAxis(), thresholdValue);
    });  
  }
  else if (attr == "eccentricity") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].eccentricity(), thresholdValue);
    });  
  }
  else if (attr == "compactness") {
    tree.idirectFilter([&op, &attrs, thresholdValue](NodePtr node) { 
      return op(attrs[node->id()].compactness(), thresholdValue);
    });  
  }
  else {
    std::cerr << "ERROR. invalid attribute: " << attr << "\n";
    return -1;
  }

  std::cout << "number of nodes after filter: " << tree.numberOfNodes() << "\n";

  std::vector<uint8> out = tree.reconstructImage();
  stbi_write_png(outImg.c_str(), domain.width(), domain.height(), 1, out.data(), 0);
  stbi_image_free(data);

  std::cout << "DONE.\n";
  return 0;
}