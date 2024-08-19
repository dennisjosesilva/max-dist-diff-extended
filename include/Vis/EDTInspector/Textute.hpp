#pragma once 

#include <glad/glad.h>
#include <memory>


class Texture
{
public:
  Texture(const std::string &filename);
  Texture(std::unique_ptr<unsigned char> data, int width, int height);

  inline int id() const { return id_; }
  inline int width() const { return width_; }
  inline int height() const { return height_; }

  inline void bind() const { glBindTexture(GL_TEXTURE_2D, id_); }
  void unBind() const { glBindTexture(GL_TEXTURE_2D, 0); };

  void load(const std::string &filename);
  void load(std::unique_ptr<unsigned char> data, int width, int height);

  void destroy();

  ~Texture();

private:
  void loadImageToGPU(std::unique_ptr<unsigned char> data);

private:
  unsigned int id_;
  int width_;
  int height_;
};