#include "ImGuiDraw.h"
#include "Graphics.h"

ImGuiDraw::ImGuiDraw() : name_(nullptr), create_window_(false)
{
  // should never be called due to private nature
}

ImGuiDraw::ImGuiDraw(const char* name, bool create_window) : name_(name), create_window_(create_window)
{
  GRAPHICS.imgui_draw_.emplace(this);
}

ImGuiDraw::~ImGuiDraw()
{
  GRAPHICS.imgui_draw_.erase(GRAPHICS.imgui_draw_.find(this));
}