#pragma once

#include "Object.h"
#include "ImGuiDraw.h"

#include <unordered_map>
#include <unordered_set>
#include <stack>

struct GLFWwindow;

class Graphics
{
  friend class ImGuiDraw;

  public:
    Graphics();
    
    void Initialize();
    bool Update(float& dt);
    void Exit();
    
    void CreateObject(const char* obj_file);
    void DeleteObject(unsigned id);

    GLFWwindow* window;

  private:
    unsigned next_id_ = 0;
    std::unordered_set<ImGuiDraw*> imgui_draw_;
    std::unordered_map<unsigned, Object*> objects_;
    std::stack<const char*> obj_to_create_;
    std::stack<unsigned> obj_to_delete_;

    void Draw_(float& dt);

    void CreateNextObject_();
    void DeleteNextObject_();
};

extern Graphics GRAPHICS;