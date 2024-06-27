#include <morphology/Adjacency.hpp>
#include <morphology/ErosionSerial.hpp>
#include <morphotree/core/box.hpp>
#include <morphotree/core/io.hpp>
#include <morphotree/tree/ct_builder.hpp>
#include <morphotree/adjacency/adjacency8c.hpp>

#include <contour/IncrementalContourSerial.hpp>
#include <iostream>

#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

std::vector<bool> naiveContour(const morphotree::Box &domain,
  IncrementalContourSerial::NodePtr node);

std::vector<bool> reconstructContours(const morphotree::Box &domain,
  const IncrementalContourSerial::Contours &contours);

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::printImageIntoConsoleWithCast;
  using MTree = IncrementalContourSerial::MTree;
  using NodePtr = IncrementalContourSerial::NodePtr;
  using morphotree::buildMaxTree;
  using morphotree::Adjacency8C;
  using Contours = IncrementalContourSerial::Contours;

  std::vector<uint8> f;
  Box domain;

  // check number of arguments from the command call
  if (argc < 3) {
    f = {
      0, 0 ,0, 0, 0, 0, 0,
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

  std::vector<Contours> contours =
    extractContourSerial(domain, f, Adj4{domain}, tree);  

  if (argc < 3) {
    tree.tranverse([&contours, &domain](NodePtr node){
      std::vector<bool> cimg(domain.numberOfPoints(), false);
      for (uint32 pidx : contours[node->id()])
        cimg[pidx] = true;

      printImageIntoConsoleWithCast<int>(cimg, domain);
      std::cout << std::endl;
    });
  }
  else 
  {
    if (std::string(argv[2]) == "-c") {
      bool checked = true;
      tree.tranverse([&checked, &domain, &contours](NodePtr node){
        std::vector<bool> ncontour = naiveContour(domain, node);
        std::vector<bool> incrContour = reconstructContours(domain, contours[node->id()]);

        for (uint32 pidx = 0; pidx < domain.numberOfPoints(); pidx++)
        {
          if (ncontour[pidx] != incrContour[pidx]) {
            std::cout << "ERROR: node.id= " << node->id() << "\n";
            checked = false;
          }
        }
      });

      if (checked)
        std::cout << "\nCHECKING DONE SUCCESSFUL\n";
      else 
        std::cout << "\nCHECKING DONE FAILED\n";

    }
    else {
      tree.tranverse([&contours, &domain, &argv](NodePtr node){
        std::stringstream imgpath;

        imgpath << argv[2] << node->id() << ".png";

        std::vector<uint8> cimg(domain.numberOfPoints()*3, 255);
        for (uint32 pidx : node->reconstruct()) {
          cimg[pidx*3] = cimg[pidx*3+1] = cimg[pidx*3+2] = 0;
        }
        
        for (uint32 pidx : contours[node->id()]) {
          cimg[pidx*3] = 255;
          cimg[pidx*3+1] = cimg[pidx*3+2] = 0;
        }

        
        stbi_write_png(imgpath.str().c_str(), domain.width(), domain.height(),
          3, cimg.data(), 0);        
        std::cout << imgpath.str() << "written in disk\n";
      });
    }
  }

  // std::vector<int> f_eroded = erodeSerial(domain, f, Adj8{domain});

  // if (argc < 3) {
  //   printImageIntoConsoleWithCast<int>(f_eroded, domain);
  // }
  // else {
  //   std::vector<uint8> f_converted(domain.numberOfPoints());
  //   for (uint32 pidx = 0; pidx < domain.numberOfPoints(); pidx++) {
  //     // clamp
  //     if (f_eroded[pidx] < 0)
  //       f_converted[pidx] = 0;
  //     else if (f_eroded[pidx] > 255)
  //       f_converted[pidx] = 255;
  //     else 
  //       f_converted[pidx] = static_cast<uint32>(f_eroded[pidx]);      
  //   }

  //   stbi_write_png(argv[2], domain.width(), domain.height(), 1, f_converted.data(), 0);
  // }

  return 0;
}

std::vector<bool> naiveContour(const morphotree::Box &domain,
  IncrementalContourSerial::NodePtr node)
{
  using morphotree::uint32;
  using morphotree::Box;

  std::vector<bool> cimg(domain.numberOfPoints(), false);
  std::vector<bool> bimg = node->reconstruct(domain);
  Adj4 adj{domain};
  for (uint32 pidx : node->reconstruct()) {
    for (uint32 qidx : adj(pidx)) {
      if (qidx == Box::UndefinedIndex || !bimg[qidx]) {
        cimg[pidx] = true;
        break;
      }
    }
  }
  
  return cimg;
}

std::vector<bool> reconstructContours(const morphotree::Box &domain,
  const IncrementalContourSerial::Contours &contours)
{
  using morphotree::uint32;

  std::vector<bool> cimg(domain.numberOfPoints(), false);
  for (uint32 pidx : contours)
    cimg[pidx] = true;
  
  return cimg;
}
