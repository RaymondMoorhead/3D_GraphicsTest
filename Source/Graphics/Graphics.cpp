#include "Graphics.h"
#include "../Debug/DebugLog.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

static Graphics GRAPHICS;

Graphics::Graphics() : window(nullptr)
{

}

static void glfw_error_callback(int error, const char* description)
{
  LOG("Glfw Error " << error << ": " << description);
}

void Graphics::Initialize()
{
  // Setup window
  glfwSetErrorCallback(glfw_error_callback);
  int result = glfwInit();
  LOG_MARKED_IF("glfwInit failed", !result, '!');

  // Pick GL and GLSL versions
  const char* glsl_version = "#version 130";
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

  // Create window with graphics context
  window = glfwCreateWindow(1280, 720, "3D Graphics Test", NULL, NULL);
  LOG_MARKED_IF("glfwCreateWindow failed", !window, '!');

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1); // Enable vsync

  // Initialize OpenGL loader
  bool error = glewInit() != GLEW_OK;
  LOG_MARKED_IF("glewInit failed", error, '!');

  // enable alpha
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // imgui
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

bool Graphics::Update(float& dt)
{
  // create objects
  while (!obj_to_create_.empty())
    CreateNextObject_();

  // delete objects
  while (!obj_to_delete_.empty())
    DeleteNextObject_();

  Draw_(dt);
  return glfwWindowShouldClose(window);
}

void Graphics::Draw_(float& dt)
{
  glfwPollEvents();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // draw objects
  for (auto it = objects_.begin(); it != objects_.end(); ++it)
    it->second->Draw();

  // draw imgui
  if (ImGui::BeginMainMenuBar())
  {
    if (ImGui::BeginMenu("Info"))
    {
      ImGui::Value("FPS", 1.0f / dt);
      ImGui::Value("Delta Time", dt);
      ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("Objects"))
    {
      for (auto it = imgui_draw_.begin(); it != imgui_draw_.end(); ++it)
      {
        ImGui::PushID(*it); // use the address as a unique id
        if ((*it)->create_window_ && ImGui::BeginMenu((*it)->name_))
        {
          (*it)->DrawImGui();
          ImGui::EndMenu();
        }
        ImGui::PopID();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  for (auto it = imgui_draw_.begin(); it != imgui_draw_.end(); ++it)
    (*it)->DrawImGui();

  // Rendering
  ImGui::Render();
  int display_w, display_h;
  glfwGetFramebufferSize(window, &display_w, &display_h);
  glViewport(0, 0, display_w, display_h);
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

  glfwSwapBuffers(window);
  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);// black
  glClear(GL_COLOR_BUFFER_BIT);

  GLenum err = glGetError();
  LOG_MARKED_IF("GraphicsController::Update caught glError " << err << ", you should add checks to your code to find the exact point of failure", err != 0, '!');
}

void Graphics::Exit()
{
  // Cleanup
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplGlfw_Shutdown();
  ImGui::DestroyContext();

  glfwDestroyWindow(window);
  glfwTerminate();
}

void Graphics::CreateObject(const char* obj_file)
{
  obj_to_create_.push(obj_file);
}

void Graphics::DeleteObject(unsigned id)
{
  obj_to_delete_.push(id);
}

void Graphics::CreateNextObject_()
{
  const char* file = obj_to_create_.top();
  Object* obj = new Object(next_id_++);
  // object serialization here
  obj_to_create_.pop();
}

void Graphics::DeleteNextObject_()
{
  objects_.erase(objects_.find(obj_to_delete_.top()));
  obj_to_delete_.pop();
}