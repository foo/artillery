#include <math.h>

#pragma once

class MDVector
{
public:
	float x,y;

	MDVector(float a = 0, float b = 0) : x(a), y(b) {}

	const bool operator==(const MDVector &vec) const
     {
          return ((x == vec.x) && (y == vec.y));
     }
     const bool operator!=(const MDVector &vec) const
     {
          return !(*this == vec);
     }
     const MDVector operator+(const MDVector &vec) const
     {
          return MDVector(x + vec.x, y + vec.y);
     }
     const MDVector operator+() const
     {    
          return MDVector(*this);
     }
     const MDVector& operator+=(const MDVector& vec)
     {    x += vec.x;
          y += vec.y;
          return *this;
     }
     const MDVector operator-(const MDVector& vec) const
     {    
          return MDVector(x - vec.x, y - vec.y);
     }
     const MDVector operator-() const
     {    
          return MDVector(-x, -y);
     }
     const MDVector &operator-=(const MDVector& vec)
     {
          x -= vec.x;
          y -= vec.y;

          return *this;
     }
     const MDVector &operator*=(const float &s)
     {
          x *= s;
          y *= s;
          
          return *this;
     }
     const MDVector &operator/=(const float &s)
     {
          const float recip = 1/s;
          x *= recip;
          y *= recip;
          return *this;
     }
     const MDVector operator*(const float &s) const
     {
          return MDVector(x*s, y*s);
     }
     friend inline const MDVector operator*(const float &s, const MDVector &vec)
     {
          return vec*s;
     }
     const MDVector operator/(float s) const
     {
          s = 1/s;

          return MDVector(s*x, s*y);
     }

     const float DotProduct(const MDVector &vec) const
     {
          return x*vec.x + y*vec.x;
     }
     const float operator%(const MDVector &vec) const
     {
          return x*vec.x + y*vec.x;
     }
     const float Length() const
     {
          return sqrtf(x*x + y*y);
     }
     const MDVector UnitVector() const
     {
          return (*this) / Length();
     }
     void Normalize()
     {
          (*this) /= Length();
     }
     const float operator!() const
     {
          return sqrtf(x*x + y*y);
	 }
     const MDVector operator | (const float length) const
     {
          return *this * (length / !(*this));
     }
     const MDVector& operator |= (const float length)
     {
          return *this = *this | length;
     }
     const float inline Angle(const MDVector& normal) const
     {
          return acosf(*this % normal);
     }
     const MDVector inline Reflection(const MDVector& normal) const
     {    
          const MDVector vec(*this | 1);     // normalizuje wektor
          return (vec - normal * 2.0 * (vec % normal)) * !*this;
     }
	 bool isInPoly(
		const MDVector p1,
		const MDVector p2,
		const MDVector p3,
		const MDVector p4
		) const
	{
		const float twoPI = 6.28f;
		const float tolerance = 0.1f;
		MDVector s1,s2,s3,s4;
		s1 = (*this) - p1;
		s2 = (*this) - p2;
		s3 = (*this) - p3;
		s4 = (*this) - p4;

		float sum = 0;
		s1.Normalize();
		s2.Normalize();
		s3.Normalize();
		s4.Normalize();
		s1 |= 1;
		s2 |= 1;
		s3 |= 1;
		s4 |= 1;
		sum += (s1).Angle(s2);
		sum += (s2).Angle(s3);
		sum += (s3).Angle(s4);
		sum += (s4).Angle(s1);

		if (sum <= (twoPI + tolerance) && sum >= (twoPI - tolerance))
			return true;
		return false;
	}
};