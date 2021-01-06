/*! \file Texture.h
    \date 12/31/2019
    \author Raymond Moorhead
    \brief Declaration of Texture struct.
*/

#pragma once

#include "../ImGuiDraw.h"
#include <GL/glew.h>
#include <vector>

//! Used to identify Texture source folder.
extern const char* TEXTURE_FOLDER_PATH;

//! Stores texture information.
struct Texture : public ImGuiDraw
{
  public:
    //! Constructor, sets default texture modes.
    Texture();
    
    /*!
      \brief Constructor, sets default texture modes and loads file.
      \param file The file to load, full file path expected.
      \param path The TextureType, which hints at the path.
    */
    Texture(const char* file);
    
    /*!
      \brief Constructor, sets default texture modes and attempts to load multiple files.
      \param folder_and_name The first part of a file name and path, such as "Hero\Hero-Idle"
      \param ext The extension (with period) of the file, such as ".png"
      \param path The TextureType, which hints at the path.
    */
    Texture(const char* folder_and_name, const char* ext);
    
    //! Desstructor, sets default texture modes and deletes texture.
    ~Texture();
    

    //!  \brief Displays full texture.
    void DrawImGui();
    
    /*!
      \brief Attempts to load a file into a texture.
      \param file The file to load, full file path expected.
      \param path The TextureType, which hints at the path.
    */
    bool Load(const char* file);
    
    /*!
      \brief Returns if a texture is valid, if it loaded correctly.
      \return True if the texture was loaded properly.
    */
    bool TexIsValid();
    
    /*! \brief Binds the texture as the current active GL_TEXTURE_2D texture object.
        \param tex The index of the texture to be used.
    */
    void Bind(GLuint tex = 0) const;
    
    //! The ID of the first texture, used by OpenGL to reference this texture.
    GLuint id;
    
    //! The width of the texture.
    GLuint width;
    //! The height of the texture.
    GLuint height;
    //! The width/height ratio of the texture.
    float wh_ratio;
    
    //! The object format.
    GLuint internal_format;
    //! The texture format.
    GLuint image_format;
    
    //! Wrapping mode on S axis.
    GLuint wrap_s;
    //! Wrapping mode on T axis.
    GLuint wrap_t;
    //! Filtering mode if image is smaller than covered area.
    GLuint filter_min;
    //! Filtering mode if texture is larger than covered area.
    GLuint filter_max;
    
  private:
    /*!
      \brief Creates a TexImage2D in OpenGL.
      \param data The image information.
      \param index The index for the width, height, and id information.
    */
    void Generate_(unsigned char* data, size_t index = 0);
};
