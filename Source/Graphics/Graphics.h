/*! \file Graphics.h
    \date 1/7/2021
    \author Raymond Moorhead
    \brief Contains Graphics class, which manages graphics, including the objects
*/
#pragma once

#include "Object.h"
#include "ImGuiDraw.h"
#include "LowLevel/Viewport.h"

#include <unordered_map>
#include <unordered_set>
#include <stack>

struct GLFWwindow;

class Graphics
{
  friend class ImGuiDraw;

  public:
    Graphics();
    
    //! \brief Initializes OpenGL and ImGui.
    void Initialize();

    /*! \brief Handles object changes and calls Draw_.
        \param dt The delta time of the current frame, the fraction of a second it will take to run.
        \return True if the program should keep running
    */
    bool Update(float& dt);

    //! \brief Shuts down OpenGL and ImGui.
    void Exit();
    
    /*! \brief Requests an Object be created next frame
        \param obj_file The file with the model data to be used.
    */
    void CreateObject(const char* obj_file);
    /*! \brief Requests an object be deleted next frame
        \param id The unique id of the Object to be deleted.
    */

    void DeleteObject(unsigned id);

    GLFWwindow* window;
    Viewport viewport;

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