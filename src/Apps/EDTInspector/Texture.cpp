#include <Vis/EDTInspector/Textute.hpp>

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture(const std::string &filename)
  : id_{0}
{
  load(filename);
}

Texture::Texture(std::unique_ptr<unsigned char> data, int width, int height)
  :id_{0}
{
  load(std::move(data), width, height);
}

Texture::~Texture()
{  
  destroy();
}

void Texture::load(const std::string &filename)
{
  if (id_) {
    destroy();
  }

  int nchannels;
  unsigned char *data = stbi_load(filename.c_str(), &width_, &height_, &nchannels, 0);

  loadImageToGPU(std::unique_ptr<unsigned char>(data));
}

void Texture::load(std::unique_ptr<unsigned char> data, int width, int height)
{
  if (id_) {
    destroy();
  }

  width_ = width;
  height_ = height;
  loadImageToGPU(std::move(data));
}

void Texture::destroy()
{  
  glDeleteTextures(1, &id_);
  id_ = 0;
}

void Texture::loadImageToGPU(std::unique_ptr<unsigned char> data)
{
  // Initialise GPU Texture
  glGenTextures(1, &id_);
  glBindTexture(GL_TEXTURE_2D, id_);

  // Setup filtering parameters for display
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  // making ImGui::Image drawing image in grayscale
  int swizzle[4] = {
    GL_RED,   // Shader Red channel source = Textuere Red
    GL_GREEN,   // Shader Green channel source = Textuere Red
    GL_BLUE,   // Shader Blue channel source = Textuere Red
    GL_ONE    // Shader alpha channel source = One
  };
  glTexParameterIiv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzle);


    std::cout << width_ << " " << height_ << "\n";

  // Upload pixels into texture
  glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width_, height_, 0, GL_RGBA, GL_UNSIGNED_BYTE, data.get());
  glGenerateMipmap(GL_TEXTURE_2D);

  
}


