/*! \file Shader.cpp
    \date 3/12/2019
    \author Raymond Moorhead
    \brief Contains Shader struct implementation.
*/

#include "GL/glew.h"
#include "Shader.h"
#include "../..//Debug/DebugLog.h"

#include <stdio.h>
#include <sstream>
#include <string>
#include <fstream>

static const char* SHADER_ASSET_PATH = "../Assets/Shaders/";

// HELPER FUNCTIONS START
static bool ReadInShader(GLuint shader, const char* filename)
{
  std::ifstream file;
  
  file.open(filename, std::ios::in);
  if(!file.is_open())
  {
    LOG_MARKED("Could not find shader file " << filename, '!');
    return false;
  }
  
  if(!file.good())
  {
    file.close();
    return false;
  }
  
  std::stringstream shader_stream;
  std::string shader_string;
  const char* shader_chars;
  
  shader_stream << file.rdbuf();
  shader_string = shader_stream.str();
  shader_chars = shader_string.c_str();
  
  glShaderSource(shader, 1, &(shader_chars), nullptr);
  glCompileShader(shader);
  
  file.close();
  return true;
}

static void PrintShaderResults(GLuint shader, const char* name, bool compile = true)
{
  int infologLength = 0;
  int charsWritten = 0;
  char *infoLog;
  GLint success = 0;
  
  if(compile)
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  else
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
  
  LOG_MARKED_IF(name << " shader failed to " << (compile ? "compile" : "link"), success == GL_FALSE, '!');
  
  if(compile)
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
  else
    glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infologLength);

  // should always have '\0' character, so really we're looking
  // for data beyond the expected character
  if (infologLength > 1)
  {
    infoLog = (char *)malloc(infologLength);
    if(compile)
      glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
    else
      glGetProgramInfoLog(shader, infologLength, &charsWritten, infoLog);
    LOG(infoLog);
    std::ofstream file;
    file.open("../Logs/" + std::string(name) + "_log.log", std::ofstream::out | std::ofstream::trunc);
    file << infoLog;
    file.close();
    free(infoLog);
  }
}

// HELPER FUNCTIONS END
Shader::Shader(void) : name("BadShader"), program(-1)
{

}

Shader::Shader(const char* name_) : program(-1)
{
  Compile(name_);
}

Shader::Shader(const Shader& rhs) : name(rhs.name), program(rhs.program)
{

}

void Shader::Compile(const char* name_)
{
  // create
  GLuint vert, frag;
  name = name_;
  vert = glCreateShader(GL_VERTEX_SHADER);
  frag = glCreateShader(GL_FRAGMENT_SHADER);

  // read in code
  ReadInShader(vert, (SHADER_ASSET_PATH + std::string(name_) + ".vert").c_str());
  ReadInShader(frag, (SHADER_ASSET_PATH + std::string(name_) + ".frag").c_str());

  // print out compilation data
  PrintShaderResults(vert, (std::string(name_) + "_vert").c_str());
  PrintShaderResults(frag, (std::string(name_) + "_frag").c_str());

  // create the program
  if(program == GLuint(-1))
    program = glCreateProgram();
  glAttachShader(program, vert);
  glAttachShader(program, frag);

  glLinkProgram(program);
  PrintShaderResults(program, name_, false);
  
  glDetachShader(program, vert);
  glDetachShader(program, frag);
  
  glDeleteShader(vert);
  glDeleteShader(frag);
}

void Shader::Recompile()
{
  Compile(name);
  GLint link_result;
  glGetProgramiv(program, GL_LINK_STATUS, &link_result);

  if (link_result == GL_TRUE)
  {
    LOG_MARKED("Recompilation of " << name << " successful", '+');
  }
  else
  {
    LOG_MARKED("Recompilation of " << name << " failed, please check Logs folder for details", '!');
  }
}

void Shader::Use()
{
  glUseProgram(program);
}
