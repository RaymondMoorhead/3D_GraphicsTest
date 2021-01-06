/*! \file Viewport.cpp
    \date 1/17/2020
    \author Raymond Moorhead
    \brief Contains implementation of ViewportManager struct, a helper struct to reduce file size of GraphicsController.
*/

#include "Viewport.h"

#include <SDL_opengl.h>
#include <math.h>

#include "../../DefineLists/DebugOptions.inl"

#ifdef _DEBUG
  #include "../../Core/Core.h"
#endif

// initialize the static variables
int Viewport::win_width = 0;
int Viewport::win_height = 0;
float Viewport::win_ratio = 0;

Viewport::Viewport(void)
{
  clear_color[0] = clear_color[1] = clear_color[2] = .5f;
}

Viewport::Viewport(int btf_order_) : btf_order(btf_order_)
{
  clear_color[0] = clear_color[1] = clear_color[2] = .5f;
}

Viewport::Viewport(int btf_order_, int x0, int y0, int x1, int y1, float r, float g, float b) : btf_order(btf_order_)
{
  clear_color[0] = r;
  clear_color[1] = g;
  clear_color[2] = b;
  SetDimensions(x0, y0, x1, y1);
}

Viewport::Viewport(const Viewport& rhs)
{
  clear_color[0] = clear_color[1] = clear_color[2] = .5f;
  operator=(rhs);
}

void Viewport::SetDimensions(int x0, int y0, int x1, int y1)
{
  position[0] = x0;
  position[1] = y0;
  size[0] = x1 - x0;
  size[1] = y1 - y0;
  ratio = float(size[0]) / float(size[1]);
}

void Viewport::Set() const
{
  glViewport(position[0], position[1], size[0], size[1]);
}

bool Viewport::IsInside(int x, int y) const
{
  return ((x >= position[0]) &&
          (y >= position[1]) &&
          (x <= (position[0] + size[0])) &&
          (y <= (position[1] + size[1])));
}

void Viewport::GetRelativePosition(int x_in, int y_in, float* x_out, float* y_out) const
{
  float ratio = float(size[0]) / float(size[1]);
  *x_out = (float(x_in - position[0]) / float(size[0])) * ratio;
  *y_out =  float(y_in - position[1]) / float(size[1]);
}

bool Viewport::operator<(const Viewport& rhs) const
{
  return btf_order < rhs.btf_order;
}

Viewport& Viewport::operator=(const Viewport& rhs)
{
  position[0] = rhs.position[0];
  position[1] = rhs.position[1];
  
  size[0] = rhs.size[0];
  size[1] = rhs.size[1];
  
  clear_color[0] = rhs.clear_color[0];
  clear_color[1] = rhs.clear_color[1];
  clear_color[2] = rhs.clear_color[2];
  
  ratio = rhs.ratio;
  btf_order = rhs.btf_order;
  return *this;
}
