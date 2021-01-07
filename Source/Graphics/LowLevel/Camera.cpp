/*! \file Camera.cpp
    \date 1/17/2020
    \author Raymond Moorhead
    \brief Implementation of Camera.
*/

#include "Camera.h"
#include "Shader.h"
#include "../Object.h"
#include "../Graphics.h"

#include <ImGui/imgui.h>
#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
//#include <glm/vec3.hpp>

Camera::Camera() : look_at(0.0f, 0.0f, -1.0f), up(0.0f, 1.0f, 0.0f), right(1.0f, 0.0f, 0.0f)
{
}

Camera::~Camera()
{
  
}

void Camera::SetProjection(Shader* shader, const float& view_ratio)
{
  float fov = 90.f * (1.0f / zoom);
  glm::mat4 proj = glm::perspective(fov, view_ratio, near_plane, far_plane);
  shader->Use();
  glUniformMatrix4fv(glGetUniformLocation(shader->program, "projection"), 1, GL_FALSE, glm::value_ptr(proj));
  
  glm::vec3 position_vec3(position.x, position.y, position.z);
  glm::vec3 target_vec3(position_vec3 + glm::vec3(look_at.x, look_at.y, look_at.z));
  glm::vec3 up_vec3(up.x, up.y, up.z);
  glm::mat4 view = glm::lookAt(position_vec3, target_vec3, up_vec3);
  shader->Use();
  glUniformMatrix4fv(glGetUniformLocation(shader->program, "view"), 1, GL_FALSE, glm::value_ptr(view));
}

void Camera::SetBillboard(Shader* shader)
{
  glUniform3f(glGetUniformLocation(shader->program, "cam_up"), up.x, up.y, up.z);
  glUniform3f(glGetUniformLocation(shader->program, "cam_right"), right.x, right.y, right.z);
}

void Camera::SetFromObject(Object* obj)
{
  // set base values
  up(0.0f, 1.0f, 0.0f);
  look_at(0.0f, 0.0f, -1.0f);
  
  // rotate based on object
  up.RotateEulerRad(obj->rotation);
  look_at.RotateEulerRad(obj->rotation);
  
  position = obj->position;
  right = up.Cross(look_at);
}

void Camera::Zoom(float zoom_)
{
  if(zoom_ > 0.0f)
    zoom *= zoom_;
}
