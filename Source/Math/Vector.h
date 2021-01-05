/*! \file Vector.h
    \date 12/18/2019
    \author Raymond Moorhead
    \brief Contains declaration of the Vector struct.
*/

#pragma once

//!  Structure which handles point and vector math
/*!
   NOTE: It would be vastly preferable to call most of these functions with
         references to the rhs data, rather than by copy- but much of the
         math involved requires "velocity += delta_time * gravity" which
         can only be passed in by copy, unless we create a middleman 'temp'
         which will likely carry with it just as many inefficiencies, and
         overcomplicates the code on review.
*/
struct Vector
{
  public:
    //!  \brief Default Constructor which initializes Vector for 2D math.
    Vector();
    /*!
      \brief Copy Constructor, pulls all data (including flags) from source.
      \param rhs The source Vector to copy from.
    */
    Vector(const Vector& rhs);
    /*!
      \brief Constructor which forms a 2D Vector from the given x and y.
      \param x The x value.
      \param y The y value.
    */
    Vector(float x, float y);
    /*!
      \brief Constructor which forms a 3D Vector from the given x, y, and z.
      \param x The x value.
      \param y The y value.
      \param z The z value.
    */
    Vector(float x, float y, float z);
    //!  \brief Destructor for Vector, performs no significant operations.
    ~Vector();
    
    //!  X value of Vector.
    float x;
    //!  Y value of Vector.
    float y;
    //!  Z value of Vector.
    float z;
    
    /*!
      \brief Reconstructs Vector as 2D from the given x and y.
      \param x The x value.
      \param y The y value.
      \return This Vector.
    */
    Vector& operator()(float x, float y);
    /*!
      \brief Reconstructs Vector as 3D from the given x, y, and z.
      \param x The x value.
      \param y The y value.
      \param z The z value.
      \return This Vector.
    */
    Vector& operator()(float x, float y, float z);
    
    /*!
      \brief Assigns the given Vector to this Vector.
      \param rhs The right-hand vector input.
      \return This Vector.
    */
    Vector& operator=(Vector rhs);
  
    /*!
      \brief Adds the given Vector to this Vector.
      \param rhs The right-hand vector input.
      \return This Vector.
    */
    Vector& operator+=(Vector rhs);
    /*!
      \brief Subtracts the given Vector to this Vector.
      \param rhs The right-hand vector input.
      \return This Vector.
    */
    Vector& operator-=(Vector rhs);
    /*!
      \brief Scales this Vector with the given float.
      \param scalar The right-hand float input.
      \return This Vector.
    */
    Vector& operator*=(float scalar);
    /*!
      \brief Scales each dimension of this Vector with each Dimension of the provided one.
      \param rhs The right-hand vector input.
      \return This Vector.
    */
    Vector& operator*=(Vector rhs); // vector scalar (x *= rhs.x, y *= rhs.y, z *= rhs.z)
  
    /*!
      \brief Returns the addition of this Vector and the input Vector.
      \param rhs The right-hand vector input.
      \return The addition of this and rhs.
    */
    Vector operator+(Vector rhs) const;
    /*!
      \brief Returns the subtraction of this Vector and the input Vector.
      \param rhs The right-hand vector input.
      \return The subtraction of this and rhs.
    */
    Vector operator-(Vector rhs) const;
    /*!
      \brief Returns the scale of this Vector with the given float.
      \param scalar The right-hand float input.
      \return This scale of this and rhs.
    */
    Vector operator*(float scalar) const;
    
    /*!
      \brief Calculates the dot product of this and rhs.
      \param rhs The right-hand Vector input.
      \return The dot product of this and rhs.
    */
    float operator*(Vector rhs) const; // dot product
    
    /*!
      \brief Rotates this Vector by the given radians along the given axis.
      \param axis The axis of rotation used.
      \param radians The rotation in radians to be performed.
      \return This Vector.
    */
    Vector& RotateRad(Vector axis, float radians);
    /*!
      \brief Rotates this Vector by the given degrees along the given axis.
      \param axis The axis of rotation used.
      \param degrees The rotation in degrees to be performed.
      \return This Vector.
    */
    Vector& RotateDeg(Vector axis, float degrees);
    
    /*!
      \brief Rotates this Vector by the given radians along the three primary axis.
      \param rotation The X, Y, and Z rotation amounts in radians.
      \return This Vector.
    */
    Vector& RotateEulerRad(Vector& rotation);
    /*!
      \brief Rotates this Vector by the given radians along the three primary axis.
      \param x_rad The X axis rotation in radians.
      \param x_rad The Y axis rotation in radians.
      \param x_rad The Z axis rotation in radians.
      \return This Vector.
    */
    Vector& RotateEulerRad(float x_rad, float y_rad, float z_rad);
    /*!
      \brief Rotates this Vector by the given degrees along the three primary axis.
      \param x_rad The X axis rotation in degrees.
      \param x_rad The Y axis rotation in degrees.
      \param x_rad The Z axis rotation in degrees.
      \return This Vector.
    */
    Vector& RotateEulerDeg(float x_deg, float y_deg, float z_deg);
    
    /*!
      \brief Normalizes this Vector.
      \return This Vector.
    */
    Vector& Normalize(void);
    /*!
      \brief Checks if this vector is normalized.
      \return True if Normalized, false otherwise.
    */
    inline bool IsNormalized(void) const;
    /*!
      \brief Returns the Normalized version of this Vector.
      \return A normalized version of this Vector.
    */
    Vector GetNormalized(void);
    
    /*!
      \brief Returns the cross product of this Vector and the provided one.
      \param rhs The input Vector to perform the cross product with.
      \return The cross product of this Vector and rhs.
    */
    Vector Cross(Vector rhs) const;
    
    /*!
      \brief Returns the length of this Vector.
      \return The float length of this Vector.
    */
    float Length(void);
    /*!
      \brief Returns the squared length of this Vector.
      \return The float squared length of this Vector.
      
      The usefulness of this function lies in it being a more efficient
      way to check which of two Vectors is longer without the expensive
      square root forumla being used.
    */
    float LengthSq(void) const; // squared length
    
    /*!
      \brief Returns the midpoint of this Vector and a second Vector, treated as Points.
      \param rhs The second Vector to be treated as a Point.
      \return A Vector which, when treated as a point, lies exactly between this and rhs.
    */
    Vector Midpoint(Vector rhs) const;

    /*!
      \brief Returns true if this Vector is a length of zero.
      \param epsilon The degree of exactness desired in the check.
      \return True if zero, false otherwise.
    */
    bool IsZero(float epsilon = 0.0001f) const;
    
    
  private:
    //!  The flags used by this Vector for optimizations and error checking.
    unsigned char flags_ = 0;
    //!  The result of the last Length call.
    float length_ = 0;
    
    // FLAG VALUES
    static const char flag_is_2d_ = 0x01;              /*!< Only x and y are in use. */
    static const char flag_is_normalized_ = 0x02;      /*!< Has not been changed since Normalize was called. */
    static const char flag_length_is_accurate_ = 0x04; /*!< Has not been changed since Length was called. */
    
    /*!
      \brief Returns the state of given flag(s), returns true if any are set.
      \param input_flags The flag(s) to check.
      \return True if any flags are set, false otherwise.
    */
    bool FlagCheck_(char input_flags) const;
    /*!
      \brief Sets the given flag(s).
      \param input_flags The flag(s) to set.
    */
    void FlagSet_(char input_flags);
    /*!
      \brief Clears the given flag(s).
      \param input_flags The flag(s) to clear.
    */
    void FlagClear_(char input_flags);
};
