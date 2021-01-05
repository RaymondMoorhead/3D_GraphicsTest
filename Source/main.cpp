#include "Core/BasicCore.h"

#include "Exercises/01_VisualizedArrayList.h"
#include "Exercises/00_VisualizedLinkedList.h"

#include "GLFW/glfw3.h"
#include "ImGui/imgui.h"

// put in the global scope so as to be accessible to everything
BasicCore core;

static void IMGUI_CALLBACK()
{
  // The best resources for what you can do in imgui
  // are 'Dependencies/ImGui/imgui.h' and google
  // (imgui.h lines 440-580 will be the most useful)

  // begin an ImGui window
  if (ImGui::Begin("Window Name"))
  {
    ImGui::Text("Hey There Friend!");
  }

  // every ImGui item with the keywords 'Begin' or
  // 'Push' in it must have an associated 'End' or
  // 'Pop' when you're done with it
  ImGui::End();
}

int main(int argc, char* argv[])
{
  core.Initialize();
  core.graphics.imgui_callback = IMGUI_CALLBACK;

  // add objects
  BasicObject test_obj;
  core.objects.push_back(&test_obj);

  // used to demonstrate how Java's ArrayLists and C++'s vectors work
  // full squares are the 'apparent' size (to the user), and empty
  // squares are the capacity- the actual size of the container
  core.objects.push_back(new VisualizedArrayList<int>());

  // used to demonstrate how Linked Lists work
  // places boxes with random sizes and positions (simulating
  // random positions in memory), and links them with lines to
  // simulate 'next' pointers
  core.objects.push_back(new VisualizedLinkedList<int>());

  // run it
  while (core.IsRunning())
  {
    core.Update();

    // add your code in this loop, it will be
    // executed once per frame. Feel free to
    // use GLFW to get the keypresses, or add
    // custom imgui calls to IMGUI_CALLBACK
    // above main

    // such as:

    // jump
    if (glfwGetKey(core.graphics.window, GLFW_KEY_SPACE) == GLFW_PRESS)
      test_obj.velocity.y = 2.0f;

    // move left/right
    // the screen goes from (-1, -1) to (1, 1)
    // X += Y * delta_time;
    // means increase X by Y amount every second
    if (glfwGetKey(core.graphics.window, GLFW_KEY_A) == GLFW_PRESS)
      test_obj.position.x -= 1.0f * core.GetDeltaTime();
    if (glfwGetKey(core.graphics.window, GLFW_KEY_D) == GLFW_PRESS)
      test_obj.position.x += 1.0f * core.GetDeltaTime();
  }


  return 0;
}

// Some additions you may consider
//
// 1- Add a function that allocates a BasicObject with the 'new' keyword, and adds it to core.objects
// 2- Add an 'ImGui::Button' in 'IMGUI_CALLBACK' which calls the object creation function in 1
//   * ImGui operates almost entirely on if-checks and bool return values, the line 'if(ImGui::Button("Button")) {}'
//     will execute the code in the curly braces if the button called 'Button' is clicked.
// 3- When the jumping object collides with another object, show this by changing their color or printing it in imgui
// 4- Have the jumping object shoot out smaller boxes when left mouse is clicked (google is your friend in finding glfw functions)
// 5- Make it so that if a shotout box collides with another box, other than the jumping box, it gets removed from core.objects and destroyed with the 'delete' keyword
