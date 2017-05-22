#ifndef MATRIX_HPP
# define MATRIX_HPP

# include "Vector3.hpp"
# include <iosfwd>

class Vector4;

class Matrix
{
public:
	enum EulerOrder
	{
		XYZ,
		YZX,
		ZXY,
		ZYX,
		YXZ,
		XZY
	};

	Matrix(void);
	Matrix(float f00, float f01, float f02, float f03,
			float f10, float f11, float f12, float f13,
			float f20, float f21, float f22, float f23,
			float f30, float f31, float f32, float f33);
	Matrix(Matrix const & matrix);
	Matrix(Matrix && matrix);
	virtual ~Matrix(void) = default;

	Matrix & operator=(Matrix const & matrix);
	Matrix & operator=(Matrix && matrix);
	float & operator[](int index);
	Matrix operator*(Matrix const & matrix) const;
	Vector4 operator*(Vector4 const & vector) const;

	Matrix & multiply(Matrix const & matrix);
	Matrix & rotateX(float angle);
	Matrix & rotateY(float angle);
	Matrix & rotateZ(float angle);
	Matrix & rotate(Vector3 const & vector, EulerOrder order = XYZ);
	Matrix & translate(Vector3 const & vector);
	Matrix & scale(Vector3 const & vector);
	Matrix eulerAnglesToMatrix(Vector3 const & angles, EulerOrder order = ZYX);

	Matrix inverse(void) const;
	float determinant(void) const;
	Matrix coFactor(void);
	float const * getMatrix(void) const;
	void identity(void);
	static Matrix perspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane);
	static Matrix orthographicProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	static Matrix lookAt(Vector3 const & position, Vector3 const & center, Vector3 const & up);

private:
	float	m_matrix[16];

	int getMinor(Matrix const & src, Matrix & dest, int row, int col) const;
	float determinant3(Matrix const & matrix) const;
};

std::ostream & operator<<(std::ostream & out, Matrix const & matrix);

#endif
