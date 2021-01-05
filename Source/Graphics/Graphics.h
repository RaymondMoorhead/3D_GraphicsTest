#pragma once

#include "../Core/BasicObject.h"

#include <vector>

struct GLFWwindow;

class Graphics
{
  public:
    Graphics();
    
    void Initialize();
    bool Draw(float& dt);
    void Exit();

    // set this to point at whatever function
    // you have with imgui calls
    void (*imgui_callback)();

    GLFWwindow* window;
};