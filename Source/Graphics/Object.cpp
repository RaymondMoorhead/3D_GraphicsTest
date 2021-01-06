#include "Object.h"

#include <string>

#include "ImGui/imgui.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

Object::Object() : ImGuiDraw(nullptr)
{
}

Object::Object(unsigned id) : ImGuiDraw(("Object " + std::to_string(id)).c_str()), position(), rotation(), scale(1, 1, 1)
{

}

Object::~Object()
{

}

void Object::Draw()
{
  
}

void Object::DrawImGui()
{
  ImGui::InputFloat3("Position", &position.x);
  ImGui::InputFloat3("Rotation", &rotation.x);
  ImGui::InputFloat3("Scale", &scale.x);
}