/*! \file ImGuiDraw.h
    \date 1/7/2021
    \author Raymond Moorhead
    \brief Contains ImGuiDraw abstract class declaration, used to display debugging data.
*/
#pragma once

class ImGuiDraw
{
  friend class Graphics;
  ImGuiDraw();

  const char* name_;
  const bool create_window_;

  public:
    ImGuiDraw(const char* name, bool create_window = true);
    ~ImGuiDraw();
    virtual void DrawImGui(void) = 0;
};