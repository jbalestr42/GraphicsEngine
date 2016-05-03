#ifndef VECTOR3_HPP
# define VECTOR3_HPP

# include <iostream>

class Matrix;
class Quaternion;

class Vector3
{
public:
	Vector3(void);
	Vector3(float x, float y, float z);
	Vector3(Vector3 const & vector3);
	Vector3(Vector3 && vector3);
	~Vector3(void) = default;

	float length(void) const;
	float dotProduct(Vector3 const & vector) const;
	Vector3 & normalize(void);
	Vector3 cross(Vector3 const & vector) const;

	Vector3 rotate(Quaternion const & quaternion) const;

	Vector3 & operator=(Vector3 const & vector3);
	Vector3 & operator=(Vector3 && vector3);
	Vector3 & operator+=(Vector3 const & vector3);
	Vector3 & operator*=(Vector3 const & vector3);
	Vector3 & operator-=(Vector3 const & vector3);
	Vector3 operator+(Vector3 const & vector3) const;
	Vector3 operator*(float factor) const;
	Vector3 operator*(Vector3 const & vector3) const;
	Vector3 operator*(Matrix const & matrix) const;
	Vector3 operator-(Vector3 const & vector3) const;
	Vector3 operator-(void) const;

	float	x;
	float	y;
	float	z;

};

std::ostream & operator<<(std::ostream & out, Vector3 const & vector);
Vector3 operator/(float value, Vector3 const & vector3);

#endif
