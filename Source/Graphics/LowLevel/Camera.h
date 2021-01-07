/*! \file Camera.h
    \date 1/17/2020
    \author Raymond Moorhead
    \brief Declaration of Camera.
*/

#pragma once

#include "../../Math/Vector.h"

// forwards declarations so we need not include the files
struct Shader;
class Object;

//! A component that manages data used in screen generation for 2D.
struct Camera
{
  public:
    //! \brief Initializes variables for 2D drawing.
    Camera();
    //! \brief Calls Stop.
    ~Camera();
   
    /*! \brief Sets the projection and view uniforms for 3D perspective projection.
        \param shader The shader to set uniforms for.
        \param view_ratio The width/height ratio of the Viewport being rendered to.
    */
    void SetProjection(Shader* shader, const float& view_ratio);
    
    /*! \brief Sets cam_up and cam_right uniforms for billboarding in 3D.
        \param shader The shader to set uniforms for.
    */
    void SetBillboard(Shader* shader);
    
    /*! \brief Sets the variables for 3D based on an Object's variables
        \param obj The object to base data on.
    */
    void SetFromObject(Object* obj);
    
    /*!
      \brief Multiplies zoom_ by the given amount.
      \param zoom The multiplier for zoom_, must be greater than 0.
    */
    void Zoom(float zoom);
    
    //! Position of camera in 3D.
    Vector position;
    //! LookAt vector of the camera in 3D.
    Vector look_at;
    //! Up vector of the camera in 3D.
    Vector up;
    //! Right vector of the camera in 3D.
    Vector right;
    
    //! Near plane of the camera, the closest point to the camera that will be rendered.
    float near_plane = -10.f;
    //! Far plane of the camera, the farthest point from the camera that will be rendered.
    float far_plane = 100.f;
    //! Zoom level of camera, in 3D acts as fov of (90 * 1/zoom).
    float zoom = 1.f;
};
