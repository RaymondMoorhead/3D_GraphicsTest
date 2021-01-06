/*! \file Shader.h
    \date 3/12/2019
    \author Raymond Moorhead
    \brief Contains Shader struct declaration.
*/

#pragma once

typedef unsigned int	GLuint;

struct Shader
{
  Shader(void);
  Shader(const char*);
  Shader(const Shader&);
  
  void Compile(const char* name);
  void Recompile();
  
  //! Calls glUseProgram
  void Use();

  // file data
  const char* name;

  // compiled program
  GLuint program;
};
