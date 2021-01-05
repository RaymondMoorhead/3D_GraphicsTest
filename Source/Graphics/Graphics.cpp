#include "Graphics.h"
#include "../Debug/DebugLog.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_glfw.h"
#include "ImGui/imgui_impl_opengl3.h"

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <string>

Graphics::Graphics() : imgui_callback(nullptr), window(nullptr)
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
  window = glfwCreateWindow(1280, 720, "Basic Bounce Engine", NULL, NULL);
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

bool Graphics::Draw(float& dt, std::vector<BasicObject*>* objects)
{
  glfwPollEvents();

  // Start the Dear ImGui frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  // draw objects
  for (auto it = objects->begin(); it != objects->end(); ++it)
  {
    (*it)->Draw();
  }

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
      for (size_t i = 0; i < objects->size(); ++i)
      {
        ImGui::PushID(int(i));
        if (ImGui::BeginMenu((*objects)[i]->name))
        {
          (*objects)[i]->PrintImGui();
          ImGui::EndMenu();
        }
        ImGui::PopID();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }

  if (imgui_callback)
    imgui_callback();

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

  return glfwWindowShouldClose(window);
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