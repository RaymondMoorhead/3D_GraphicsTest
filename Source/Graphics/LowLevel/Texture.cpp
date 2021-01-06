/*! \file Texture.cpp
    \date 12/31/2019
    \author Raymond Moorhead
    \brief Implementation of Texture struct.
*/

#include "texture.h"
#include <STB/stb_image.h>
#include <string>
#include <vector>
#include <fstream>
#include <ImGui/imgui.h>

const char* TEXTURE_FOLDER_PATH = "../Assets/Textures/";

Texture::Texture() : ImGuiDraw("Texture"),
    internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_CLAMP_TO_BORDER), wrap_t(GL_CLAMP_TO_BORDER), filter_min(GL_NEAREST), filter_max(GL_NEAREST)
{
}

Texture::Texture(const char* file) : ImGuiDraw(file),
    internal_format(GL_RGBA), image_format(GL_RGBA), wrap_s(GL_CLAMP_TO_BORDER), wrap_t(GL_CLAMP_TO_BORDER), filter_min(GL_NEAREST), filter_max(GL_NEAREST)
{
  Load(file);
}

Texture::~Texture()
{
  if(TexIsValid())
  {
    glDeleteTextures(id.size(), &id[0]);
  }
}

  void Texture::DrawImGui()
  {
    ImGui::InputFloat("Suggested Frame Time", &anim_frame_dur);
    ImGui::Value("Number of textures/frames", unsigned(id.size()));
    if(ImGui::InputInt("Texture in use", &tex_in_use_) && ((GLuint(tex_in_use_) >= id.size()) || (tex_in_use_ < 0)))
      tex_in_use_ = int(id.size() - 1);
    ImGui::Text("size = %u x %u", width[tex_in_use_], height[tex_in_use_]);
    ImGui::Text("id = %u", id[tex_in_use_]);

    ImGui::Image((void*)(intptr_t)(id[tex_in_use_]), ImVec2(width[tex_in_use_], height[tex_in_use_]), ImVec2(0,1), ImVec2(1,0));
  }

bool Texture::Load(const char* file)
{
  GLint width_, height_, nrChannels;
  stbi_set_flip_vertically_on_load(true);  
  unsigned char *data = stbi_load((std::string(TEXTURE_FOLDER_PATH) + file).c_str(), &width_, &height_, &nrChannels, 0);
  if(data == nullptr)
  {
    CORE->Log(("Texture " + (std::string(TEXTURE_FOLDER_PATH) + file) + " failed to load with error: " + std::string(stbi_failure_reason())).c_str(), LI_Error);
    return false;
  }
  width.push_back(GLuint(width_));
  height.push_back(local_max_height = max_height = GLuint(height_));
  wh_ratio.push_back(float(width_) / float(height_));
  id.resize(1);
  glGenTextures(1, &id[0]);
  Generate_(data);
  stbi_image_free(data);
  #ifdef VERBOSE_LOGGING
    CORE->Log(("Texture " + std::string(file) + " loaded").c_str());
  #endif
  return true;
}

bool Texture::LoadAnimation(const char* folder_and_name, const char* ext)
{
  std::string full_path = std::string(TEXTURE_FOLDER_PATH) + folder_and_name;
  local_max_height = max_height = 0;
  
  // check for metadata
  std::ifstream metadata((full_path + "metadata").c_str());
  std::string line;
  size_t data_start;
  if(metadata.is_open())
  {
    // suggested animation frame duration
    std::getline(metadata, line);
    data_start = line.find_first_of("0123456789.");
    if(data_start != std::string::npos)
      anim_frame_dur = atof(line.c_str());
    
    metadata.close();
  }
  
  // load as many textures as possible
  GLint width_, height_, nrChannels;
  std::vector<unsigned char*> data;
  stbi_set_flip_vertically_on_load(true);  
  for(size_t i = 1; true; ++i)
  {
    data.push_back(stbi_load((full_path + std::to_string(i) + ext).c_str(), &width_, &height_, &nrChannels, 0));
    if(data.back() == nullptr)
    {
      data.pop_back();
      break;
    }
    width.push_back(GLuint(width_));
    height.push_back(GLuint(height_));
    if(GLuint(height_) > local_max_height)
      local_max_height = height_;
    wh_ratio.push_back(float(width_) / float(height_));
  }
  
  // account for what we found
  if(data.empty())
  {
    CORE->Log(("Textures " + (std::string(TEXTURE_FOLDER_PATH) + folder_and_name + std::string("") + ext) + " failed to load with error: " + std::string(stbi_failure_reason())).c_str(), LI_Error);
    return false;
  }
  id.resize(data.size());
  
  // generate textures
  glGenTextures(id.size(), &id[0]);
  for(size_t i = 0; i < data.size(); ++i)
  {
    Generate_(data[i], i);
    stbi_image_free(data[i]);
  }
  
  #ifdef VERBOSE_LOGGING
    CORE->Log(("Textures " + (folder_and_name + std::string("#") + ext) + " loaded").c_str());
  #endif
  return true;
}

bool Texture::TexIsValid()
{
  return !id.empty();
}

void Texture::Bind(GLuint tex) const
{
  #ifdef SAFETIES
    if(tex >= id.size())
    {
      CORE->Log("Attempted to Bind a texture at an invalid index", LI_Error);
      return;
    }
  #endif
  glBindTexture(GL_TEXTURE_2D, id[tex]);
}

void Texture::Generate_(unsigned char* data, size_t index)
{
  // Create Texture
  glBindTexture(GL_TEXTURE_2D, id[index]);
  glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width[index], height[index], 0, image_format, GL_UNSIGNED_BYTE, data);
    
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
