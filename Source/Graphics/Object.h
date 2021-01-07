#pragma once

#include "../Math/Vector.h"
#include "ImGuiDraw.h"

class Object : public ImGuiDraw
{
  Object();
  public:
    Object(unsigned id);
    ~Object();
    void Draw();
    void DrawImGui() override;
  
    Vector position;
    Vector rotation;
    Vector scale;
};