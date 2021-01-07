/*! \file Texture.cpp
    \date 12/31/2019
    \author Raymond Moorhead
    \brief Implementation of Texture struct.
*/

#include "texture.h"
#include "../..//Debug/DebugLog.h"
#include "STB/stb_image.h"
#include <string>
#include <vector>
#include <fstream>
#include <ImGui/imgui.h>

const char* TEXTURE_FOLDER_PATH = "../Assets/Textures/";

Texture::Texture() : ImGuiDraw("Texture"), id(GLuint(-1)), width(GLuint(-1)), height(GLuint(-1)), wh_ratio(0.0f),
    internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_CLAMP_TO_BORDER), wrap_t(GL_CLAMP_TO_BORDER), filter_min(GL_NEAREST), filter_max(GL_NEAREST)
{
}

Texture::Texture(const char* file) : ImGuiDraw(file), id(GLuint(-1)), width(GLuint(-1)), height(GLuint(-1)), wh_ratio(0.0f),
    internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_CLAMP_TO_BORDER), wrap_t(GL_CLAMP_TO_BORDER), filter_min(GL_NEAREST), filter_max(GL_NEAREST)
{
  Load(file);
}

Texture::~Texture()
{
  if(TexIsValid())
    glDeleteTextures(1, &id);
}

void Texture::DrawImGui()
{
  if (TexIsValid())
  {
    ImGui::Text("size = %u x %u", width, height);
    ImGui::Text("id = %u", id);

    ImGui::Image((void*)(intptr_t)(id), ImVec2(float(width), float(height)), ImVec2(0, 1), ImVec2(1, 0));
  }
  else
    ImGui::Text("Not a valid texture");
}

bool Texture::Load(const char* file)
{
  GLint width_, height_, nrChannels; 
  unsigned char *data = stbi_load((std::string(TEXTURE_FOLDER_PATH) + file).c_str(), &width_, &height_, &nrChannels, 0);
  if(data == nullptr)
  {
    LOG_MARKED("Texture " << TEXTURE_FOLDER_PATH << file << " failed to load with error: " << stbi_failure_reason(), '!');
    return false;
  }
  width = width_;
  height = height_;
  wh_ratio = float(width) / float(height);
  glGenTextures(1, &id);
  Generate_(data);
  stbi_image_free(data);
  LOG(("Texture " + std::string(file) + " loaded").c_str());
  return true;
}

bool Texture::TexIsValid()
{
  return id != GLuint(-1);
}

void Texture::Bind() const
{
  glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::Generate_(unsigned char* data)
{
  // Create Texture
  glBindTexture(GL_TEXTURE_2D, id);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, image_format, GL_UNSIGNED_BYTE, data);
    
  // Set Texture wrap and filter modes
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_s);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_t);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter_min);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter_max);
  if((wrap_s == GL_CLAMP_TO_BORDER) || (wrap_t == GL_CLAMP_TO_BORDER))
  {
    static float clamped_border_color[] = {1.0f, 1.0f, 1.0f, 0.0f};
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clamped_border_color);
  }
    
  // Unbind texture
  glBindTexture(GL_TEXTURE_2D, 0);
}
