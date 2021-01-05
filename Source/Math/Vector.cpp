/*! \file Vector.cpp
    \date 12/18/2019
    \author Raymond Moorhead
    \brief Contains declaration of the Vector struct.
*/

#include "Vector.h"
#include <cmath>

Vector::Vector() : x(0), y(0), z(0), flags_(flag_is_2d_)
{

}

Vector::Vector(const Vector& rhs) : x(rhs.x), y(rhs.y), z(rhs.z)
{

}

Vector::Vector(float x_, float y_) : x(x_), y(y_), z(0), flags_(flag_is_2d_)
{

}

Vector::Vector(float x_, float y_, float z_) : x(x_), y(y_), z(z_)
{

}

Vector::~Vector()
{

}

// post-constructor ways to set Vector to 2D or 3D

Vector& Vector::operator()(float x_, float y_)
{
  FlagClear_(0xFF);
  FlagSet_(flag_is_2d_);
  x = x_;
  y = y_;
  z = 0;
  return *this;
}

Vector& Vector::operator()(float x_, float y_, float z_)
{
  FlagClear_(0xFF);
  FlagClear_(flag_is_2d_);
  x = x_;
  y = y_;
  z = z_;
  return *this;
}

// NON-CONST OPERATORS

Vector& Vector::operator=(Vector rhs)
{
  flags_ = rhs.flags_;
  length_ = rhs.length_;
  x = rhs.x;
  y = rhs.y;
  z = rhs.z;
  return *this;
}

Vector& Vector::operator+=(Vector rhs)
{
  FlagClear_(flag_length_is_accurate_ | flag_is_normalized_);
  x += rhs.x;
  y += rhs.y;
  z += rhs.z;
  return *this;
}

Vector& Vector::operator-=(Vector rhs)
{
  FlagClear_(flag_length_is_accurate_ | flag_is_normalized_);
  x -= rhs.x;
  y -= rhs.y;
  z -= rhs.z;
  return *this;
}

Vector& Vector::operator*=(float scalar)
{
  FlagClear_(flag_length_is_accurate_ | flag_is_normalized_);
  x *= scalar;
  y *= scalar;
  z *= scalar;
  return *this;
}

Vector& Vector::operator*=(Vector rhs)
{
  FlagClear_(flag_length_is_accurate_ | flag_is_normalized_);
  x *= rhs.x;
  y *= rhs.y;
  z *= rhs.z;
  return *this;
}

// CONST OPERATORS

Vector Vector::operator+(Vector rhs) const
{
  return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector Vector::operator-(Vector rhs) const
{
  return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector Vector::operator*(float scalar) const
{
  return Vector(x * scalar, y * scalar, z * scalar);
}

float Vector::operator*(Vector rhs) const
{
  // dot product
  return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

// ROTATION

Vector& Vector::RotateRad(Vector axis, float radians)
{
  float c = cos(radians);
  float n1C = 1.0f - c;
  float s = sin(radians);
  float m[9]; // matrix

  // set matrix rows
  m[0] = axis.x * axis.x * n1C + c;
  m[1] = axis.x * axis.y * n1C - axis.z * s;
  m[2] = axis.x * axis.z * n1C + axis.y * s;

  m[3] = axis.x * axis.y * n1C + axis.z * s;
  m[4] = axis.y * axis.y * n1C + c;
  m[5] = axis.y * axis.z * n1C - axis.x * s;

  m[6] = axis.x * axis.z * n1C - axis.y * s;
  m[7] = axis.y * axis.z * n1C + axis.x * s;
  m[8] = axis.z * axis.z * n1C + c;

  // calculate result
  Vector intermediate;
  intermediate.x = (x * m[0]) + (y * m[1]) + (z * m[2]);
  intermediate.y = (x * m[3]) + (y * m[4]) + (z * m[5]);
  intermediate.z = (x * m[6]) + (y * m[7]) + (z * m[8]);
  return *this = intermediate;
}

#define PI_DIVIDED_BY_180 0.01745329252f
Vector& Vector::RotateDeg(Vector axis, float degrees)
{
  return RotateRad(axis, degrees * PI_DIVIDED_BY_180);
}

Vector& Vector::RotateEulerRad(Vector& rotation)
{
  return RotateEulerRad(rotation.x, rotation.y, rotation.z);
}

Vector& Vector::RotateEulerRad(float x_rad, float y_rad, float z_rad)
{
  RotateRad(Vector(0.f, 1.f, 0.f), y_rad);// y
  RotateRad(Vector(0.f, 0.f, 1.f), z_rad);// z
  RotateRad(Vector(1.f, 0.f, 0.f), x_rad);// x
  return *this;
}

Vector& Vector::RotateEulerDeg(float x_deg, float y_deg, float z_deg)
{
  return RotateEulerRad(x_deg * PI_DIVIDED_BY_180, y_deg * PI_DIVIDED_BY_180, z_deg * PI_DIVIDED_BY_180);
}
#undef PI_DIVIDED_BY_180

// NORMALIZATION

Vector& Vector::Normalize(void)
{
  // efficiency check
  if(IsNormalized())
    return *this;
  FlagSet_(flag_length_is_accurate_ | flag_is_normalized_);
  
  float length = Length();
  x /= length;
  y /= length;
  z /= length;
  length_ = 1.0000f;
  return *this;
}

inline bool Vector::IsNormalized(void) const
{
  return FlagCheck_(flag_is_normalized_);
}

Vector Vector::GetNormalized(void)
{
  return Vector(*this).Normalize();
}

// MANIPULATION

Vector Vector::Cross(Vector rhs) const
{
  return Vector((y * rhs.z) - (z * rhs.y), (z * rhs.x) - (x * rhs.z), (x * rhs.y) - (y * rhs.x));
}

float Vector::Length(void)
{
  if(FlagCheck_(flag_length_is_accurate_))
    return length_;
  FlagSet_(flag_length_is_accurate_);
  return length_ = sqrt(LengthSq());
}

float Vector::LengthSq(void) const
{
  return *this * *this;
}

Vector Vector::Midpoint(Vector rhs) const
{
  return (*this + rhs) * .5f;
}

bool Vector::IsZero(float epsilon) const
{
  if(FlagCheck_(flag_length_is_accurate_))
    return length_ < epsilon;
  return (LengthSq() < epsilon);
}

// FLAG FUNCTIONS

inline bool Vector::FlagCheck_(char input_flags) const
{
  return flags_ & input_flags;
}

inline void Vector::FlagSet_(char input_flags)
{
  flags_ |= input_flags;
}

inline void Vector::FlagClear_(char input_flags)
{
  flags_ &= ~input_flags;
}
