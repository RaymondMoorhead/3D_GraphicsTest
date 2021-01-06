/*! \file Viewport.h
    \date 1/17/2020
    \author Raymond Moorhead
    \brief Contains ViewportManager struct declaration, a helper struct to reduce file size of GraphicsController.
*/

#pragma once

//! Recommended Viewport back-to-front positions only, you can make your own.
enum ViewportOrder
{
  #define PremadeViewport(Name, Order, Unused1, Unused2, Unused3, Unused4, Unused5, Unused6, Unused7) VO_ ## Name = Order,
  #include "../../DefineLists/PremadeViewports.inl"
    NumViewportOrders
  #undef PremadeViewport
};

struct Viewport
{
  public:
    //! \brief Default constructor for Viewport, performs no significant operations.
    Viewport(void);
    
    /*! \brief Non-default Viewport constructor, used mainly for indexing into GraphicsController::view_list.
        \param btf_order The back-to-front order of this Viewport.
    */
    Viewport(int btf_order);
    
    /*! \brief Non-default Viewport constructor, used mainly emplacement in GraphicsSystem::Initialize.
        \param btf_order The back-to-front order of this Viewport.
        \param x0 The lower left x coordinate of the Viewport.
        \param y0 The lower left y coordinate of the Viewport.
        \param x1 The upper right x coordinate of the Viewport.
        \param y1 The upper right y coordinate of the Viewport.
        \param r The red coloration of the clear color.
        \param g The green coloration of the clear color.
        \param b The blue coloration of the clear color.
    */
    Viewport(int btf_order, int x0, int y0, int x1, int y1, float r, float g, float b);
      
    /*! \brief Viewport copy constructor, copies all data verbatim.
        \param rhs The Viewport to copy from.
    */
    Viewport(const Viewport& rhs);
    
    /*! \brief Sets the position, size, and ratio of the Viewport
        \param x0 The lower left x coordinate of the Viewport.
        \param y0 The lower left y coordinate of the Viewport.
        \param x1 The upper right x coordinate of the Viewport.
        \param y1 The upper right y coordinate of the Viewport.
    */
    void SetDimensions(int x0, int y0, int x1, int y1);
      
    //! \brief Sets glViewport with this Viewport's data.
    void Set() const;
    
    /*! \brief Sets out variables with the relative position of a point from the lower left of the window.
        \param x The window x position of the point in question.
        \param y The window y position of the point in question.
    */
      bool IsInside(int x, int y) const;
    
    /*! \brief Sets out variables with the relative position of a point from the lower left of the window.
        \param x_in The window x position of the point in question.
        \param y_in The window y position of the point in question.
        \param x_out The relative x position from the bottom left of the viewport it's in.
        \param y_out The relative y position from the bottom left of the viewport it's in.
    */
    void GetRelativePosition(int x_in, int y_in, float* x_out, float* y_out) const;
    
    /*! \brief Returns true if this Viewport is further back in the back-to-front ordering.
        \param rhs The Viewport to compare back-to-front ordering against.
        \return True if this Viewport is further back than rhs.
    */
    bool operator<(const Viewport& rhs) const;
    
    /*! \brief Assigns the given Viewport to this Viewport.
        \param rhs The Viewport to assign to this.
        \return A reference to this Viewport.
    */
    Viewport& operator=(const Viewport& rhs);
    
    //! The lower left of the viewport, which the other 3 corners are generated from.
    int position[2] = {0, 0};
    //! The size of the viewport.
    int size[2] = {0, 0};
    //! Aspect ration of the Viewport
    float ratio = 1.0f;
    //! The back-to-front order placement, lower is further back
    int btf_order = 0;
	  //! The background color.
	  float clear_color[3];
      
    static int win_width;  //! Width of whole window.
    static int win_height; //! Height of whole window.
    static float win_ratio;//! Width/Height ratio of whole window.
};
