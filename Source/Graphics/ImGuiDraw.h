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