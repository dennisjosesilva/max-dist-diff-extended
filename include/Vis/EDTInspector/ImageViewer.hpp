#pragma once 

#include <glad/glad.h>
#include <imgui.h>
#include <Textute.hpp>

void ImageViewer(const char *label, const Texture &texture)
{
  // Begin ImGui window
  ImGui::Begin(label);

  // Get the size of the ImGui window to fit the OpenGL Rendering
  ImVec2 size = ImGui::GetContentRegionAvail();
  ImVec2 pos = ImGui::GetCursorPos();

  // Create a framebuffer to render to the ImGui texture
  unsigned int framebuffer;
  glGenFramebuffers(1, &framebuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

  // Create a texture to render the cube onto
  unsigned int texColorBuffer;
  glGenTextures(1, &texColorBuffer);
  glBindTexture(GL_TEXTURE_2D, texColorBuffer);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, (int)size.x, (int)size.y, 0, 
    GL_RED, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

  glViewport(0, 0, (int)size.x, (int)size.y);
  
}