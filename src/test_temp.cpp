#include <morphology/Adjacency.hpp>
#include <morphology/ErosionSerial.hpp>
#include <morphotree/core/box.hpp>
#include <morphotree/core/io.hpp>

#include <iostream>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

int main(int argc, char *argv[])
{
  using morphotree::Box;
  using morphotree::uint8;
  using morphotree::uint32;
  using morphotree::printImageIntoConsoleWithCast;

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

  
  std::vector<int> f_eroded = erodeSerial(domain, f, Adj8{domain});

  if (argc < 3) {
    printImageIntoConsoleWithCast<int>(f_eroded, domain);
  }
  else {
    std::vector<uint8> f_converted(domain.numberOfPoints());
    for (uint32 pidx = 0; pidx < domain.numberOfPoints(); pidx++) {
      // clamp
      if (f_eroded[pidx] < 0)
        f_converted[pidx] = 0;
      else if (f_eroded[pidx] > 255)
        f_converted[pidx] = 255;
      else 
        f_converted[pidx] = static_cast<uint32>(f_eroded[pidx]);      
    }

    stbi_write_png(argv[2], domain.width(), domain.height(), 1, f_converted.data(), 0);
  }

  return 0;
}