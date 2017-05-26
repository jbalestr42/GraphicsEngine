#include "Matrix.hpp"
#include "Math.hpp"
#include "Vector4.hpp"
#include <algorithm>
#include <iostream>

Matrix::Matrix(void) :
	Matrix(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			0.f, 0.f, 0.f, 1.f)
{}

Matrix::Matrix(float f00, float f01, float f02, float f03,
				float f10, float f11, float f12, float f13,
				float f20, float f21, float f22, float f23,
				float f30, float f31, float f32, float f33)
{
	m_matrix[0] = f00; m_matrix[1] = f01; m_matrix[2] = f02; m_matrix[3] = f03;
	m_matrix[4] = f10; m_matrix[5] = f11; m_matrix[6] = f12; m_matrix[7] = f13;
	m_matrix[8] = f20; m_matrix[9] = f21; m_matrix[10] = f22; m_matrix[11] = f23;
	m_matrix[12] = f30; m_matrix[13] = f31; m_matrix[14] = f32; m_matrix[15] = f33;
}

Matrix::Matrix(Matrix const & matrix)
{
	*this = matrix;
}

Matrix::Matrix(Matrix && matrix)
{
	*this = std::move(matrix);
}

Matrix & Matrix::operator=(Matrix const & matrix)
{
	std::copy(std::begin(matrix.m_matrix), std::end(matrix.m_matrix), std::begin(m_matrix));
	return (*this);
}

Matrix & Matrix::operator=(Matrix && matrix)
{
	std::move(std::begin(matrix.m_matrix), std::end(matrix.m_matrix), std::begin(m_matrix));
	return (*this);
}

float & Matrix::operator[](int index)
{
	return (m_matrix[index]);
}

Matrix Matrix::operator*(Matrix const & matrix) const
{
	Matrix	result;
	for (int i = 0, offset = 0; i < 4; i++, offset = i * 4)
	{
		for (int j = 0; j < 4; j++)
		{
			result[offset + j] = m_matrix[offset + 0] * matrix.m_matrix[j + 0];
			result[offset + j] += m_matrix[offset + 1] * matrix.m_matrix[j + 4];
			result[offset + j] += m_matrix[offset + 2] * matrix.m_matrix[j + 8];
			result[offset + j] += m_matrix[offset + 3] * matrix.m_matrix[j + 12];
		}
	}
	return (result);
}

Vector4 Matrix::operator*(Vector4 const & vector) const
{
	Vector4 res;

	res.x += m_matrix[0 * 4 + 0] * vector.x;
	res.x += m_matrix[1 * 4 + 0] * vector.y;
	res.x += m_matrix[2 * 4 + 0] * vector.z;
	res.x += m_matrix[3 * 4 + 0] * vector.w;

	res.y += m_matrix[0 * 4 + 1] * vector.x;
	res.y += m_matrix[1 * 4 + 1] * vector.y;
	res.y += m_matrix[2 * 4 + 1] * vector.z;
	res.y += m_matrix[3 * 4 + 1] * vector.w;

	res.z += m_matrix[0 * 4 + 2] * vector.x;
	res.z += m_matrix[1 * 4 + 2] * vector.y;
	res.z += m_matrix[2 * 4 + 2] * vector.z;
	res.z += m_matrix[3 * 4 + 2] * vector.w;

	res.w += m_matrix[0 * 4 + 3] * vector.x;
	res.w += m_matrix[1 * 4 + 3] * vector.y;
	res.w += m_matrix[2 * 4 + 3] * vector.z;
	res.w += m_matrix[3 * 4 + 3] * vector.w;

	return (res);
}


Matrix & Matrix::multiply(Matrix const & matrix)
{
	Matrix tmp(std::move(*this));

	for (int i = 0, offset = 0; i < 4; i++, offset = i * 4)
	{
		for (int j = 0; j < 4; j++)
		{
			m_matrix[offset + j] =  tmp.m_matrix[offset + 0] * matrix.m_matrix[j + 0];
			m_matrix[offset + j] += tmp.m_matrix[offset + 1] * matrix.m_matrix[j + 4];
			m_matrix[offset + j] += tmp.m_matrix[offset + 2] * matrix.m_matrix[j + 8];
			m_matrix[offset + j] += tmp.m_matrix[offset + 3] * matrix.m_matrix[j + 12];
		}
	}
	return (*this);
}

Matrix & Matrix::rotateX(float angle)
{
	float rad = angle * Deg2Rad;
	float cos = std::cos(rad);
	float sin = std::sin(rad);

	Matrix rotation(1.f, 0.f, 0.f, 0.f,
			0.f, cos, -sin, 0.f,
			0.f, sin, cos, 0.f,
			0.f, 0.f, 0.f, 1.f);
	return multiply(rotation);
}

Matrix & Matrix::rotateY(float angle)
{
	float rad = angle * Deg2Rad;
	float cos = std::cos(rad);
	float sin = std::sin(rad);

	Matrix rotation(cos, 0.f, -sin, 0.f,
			0.f, 1.f, 0.f, 0.f,
			sin, 0.f, cos, 0.f,
			0.f, 0.f, 0.f, 1.f);
	return multiply(rotation);
}

Matrix & Matrix::rotateZ(float angle)
{
	float rad = angle * Deg2Rad;
	float cos = std::cos(rad);
	float sin = std::sin(rad);

	Matrix rotation(cos, -sin, 0.f, 0.f,
			sin,  cos, 0.f, 0.f,
			0.f,  0.f, 1.f, 0.f,
			0.f,  0.f, 0.f, 1.f);
	return multiply(rotation);
}

Matrix & Matrix::rotate(Vector3 const & vector, EulerOrder order)
{
	Matrix rotation = eulerAnglesToMatrix(vector, order);
	return multiply(rotation);
}

Matrix Matrix::eulerAnglesToMatrix(Vector3 const & angles, EulerOrder order)
{
	Matrix rotation;
	float sinX = std::sin(Deg2Rad * angles.x);
	float sinY = std::sin(Deg2Rad * angles.y);
	float sinZ = std::sin(Deg2Rad * angles.z);
	float cosX = std::cos(Deg2Rad * angles.x);
	float cosY = std::cos(Deg2Rad * angles.y);
	float cosZ = std::cos(Deg2Rad * angles.z);

	switch (order)
	{
		case XYZ:
			rotation[0] = cosY * cosZ;
			rotation[1] = -cosY * sinZ;
			rotation[2] = sinY;
			rotation[4] = cosZ * sinX * sinY + cosX * sinZ;
			rotation[5] = cosX * cosZ - sinX * sinY * sinZ;
			rotation[6] = -cosY * sinX;
			rotation[8] = -cosX * cosZ * sinY + sinX * sinZ;
			rotation[9] = cosZ * sinX + cosX * sinY * sinZ;
			rotation[10] = cosX * cosY;
			break;
		case YZX:
			rotation[0] = cosY * cosZ;
			rotation[1] = sinX * sinY - cosX * cosY * sinZ;
			rotation[2] = cosX * sinY + cosY * sinX * sinZ;
			rotation[4] = sinZ;
			rotation[5] = cosX * cosZ;
			rotation[6] = -cosZ * sinX;
			rotation[8] = -cosZ * sinY;
			rotation[9] = cosY * sinX + cosX * sinY * sinZ;
			rotation[10] = cosX * cosY - sinX * sinY * sinZ;
			break;
		case ZXY:
			rotation[0] = cosY * cosZ - sinX * sinY * sinZ;
			rotation[1] = -cosX * sinZ;
			rotation[2] = cosZ * sinY + cosY * sinX * sinZ;
			rotation[4] = cosZ * sinX * sinY + cosY * sinZ;
			rotation[5] = cosX * cosZ;
			rotation[6] = -cosY * cosZ * sinX + sinY * sinZ;
			rotation[8] = -cosX * sinY;
			rotation[9] = sinX;
			rotation[10] = cosX * cosY;
			break;
		case ZYX:
			rotation[0] = cosY * cosZ;
			rotation[1] = cosZ * sinX * sinY - cosX * sinZ;
			rotation[2] = cosX * cosZ * sinY + sinX * sinZ;
			rotation[4] = cosY * sinZ;
			rotation[5] = cosX * cosZ + sinX * sinY * sinZ;
			rotation[6] = -cosZ * sinX + cosX * sinY * sinZ;
			rotation[8] = -sinY;
			rotation[9] = cosY * sinX;
			rotation[10] = cosX * cosY;
			break;
		case YXZ:
			rotation[0] = cosY * cosZ + sinX * sinY * sinZ;
			rotation[1] = cosZ * sinX * sinY - cosY * sinZ;
			rotation[2] = cosX * sinY;
			rotation[4] = cosX * sinZ;
			rotation[5] = cosX * cosZ;
			rotation[6] = -sinX;
			rotation[8] = -cosZ * sinY + cosY * sinX * sinZ;
			rotation[9] = cosY * cosZ * sinX + sinY * sinZ;
			rotation[10] = cosX * cosY;
			break;
		case XZY:
			rotation[0] = cosY * cosZ;
			rotation[1] = -sinZ;
			rotation[2] = cosZ * sinY;
			rotation[4] = sinX * sinY + cosX * cosY * sinZ;
			rotation[5] = cosX * cosZ;
			rotation[6] = -cosY * sinX + cosX * sinY * sinZ;
			rotation[8] = -cosX * sinY + cosY * sinX * sinZ;
			rotation[9] = cosZ * sinX;
			rotation[10] = cosX * cosY + sinX * sinY * sinZ;
			break;
	}
	return (rotation);
}

Matrix & Matrix::translate(Vector3 const & vector)
{
	Matrix translation(1.f, 0.f, 0.f, 0.f,
			0.f, 1.f, 0.f, 0.f,
			0.f, 0.f, 1.f, 0.f,
			vector.x, vector.y, vector.z, 1.f);
	return multiply(translation);
}

Matrix & Matrix::scale(Vector3 const & vector)
{
	Matrix scale(vector.x, 0.f, 0.f, 0.f,
			0.f, vector.y, 0.f, 0.f,
			0.f, 0.f, vector.z, 0.f,
			0.f, 0.f, 0.f, 1.f);
	return multiply(scale);
}

float Matrix::determinant(void) const
{
	float const * m = m_matrix;
	float det =
		(m[0] * (m[5] * (m[10] * m[15] - m[11] * m[14])
				 - m[6] * (m[9] * m[15] - m[11] * m[13])
				 + m[7] * (m[9] * m[14] - m[10] * m[13])))
		-
		(m[1] * (m[4] * (m[10] * m[15] - m[11] * m[14])
				 - m[6] * (m[8] * m[15] - m[11] * m[12])
				 + m[7] * (m[8] * m[14] - m[10] * m[12])))
		+
		(m[2] * (m[4] * (m[9] * m[15] - m[11] * m[13])
				 - m[5] * (m[8] * m[15] - m[11] * m[12])
				 + m[7] * (m[8] * m[13] - m[9] * m[12])))
		-
		(m[3] * (m[4] * (m[9] * m[14] - m[10] * m[13])
				 - m[5] * (m[8] * m[14] - m[10] * m[12])
				 + m[6] * (m[8] * m[13] - m[9] * m[12])));
	return (det);
}

Matrix Matrix::inverse(void) const
{
	Matrix inverse;
	float det = 1.f / determinant();

	Matrix minor;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			// get the co-factor (matrix) of A(j,i)
			getMinor(*this, minor, j, i);
			inverse[i * 4 + j] = det * determinant3(minor);
			if ((i + j) % 2 == 1)
				inverse.m_matrix[i * 4 + j] = -inverse.m_matrix[i * 4 + j];
		}
	}
	return (inverse);
}

float const * Matrix::getMatrix(void) const
{
	return (m_matrix);
}

void Matrix::identity(void)
{
	m_matrix[0] = 1.f; m_matrix[1] = 0.f; m_matrix[2] = 0.f; m_matrix[3] = 0.f;
	m_matrix[4] = 0.f; m_matrix[5] = 1.f; m_matrix[6] = 0.f; m_matrix[7] = 0.f;
	m_matrix[8] = 0.f; m_matrix[9] = 0.f; m_matrix[10] = 1.f; m_matrix[11] = 0.f;
	m_matrix[12] = 0.f; m_matrix[13] = 0.f; m_matrix[14] = 0.f; m_matrix[15] = 1.f;
}

Matrix Matrix::perspectiveProjection(float fov, float aspectRatio, float nearPlane, float farPlane)
{
	float		tanHalfFovY = std::tan(Deg2Rad * fov / 2.f);
	float		scaleX = 1.f / (aspectRatio * tanHalfFovY);
	float		scaleY = 1.f / tanHalfFovY;

	Matrix result(scaleX, 0.f, 0.f, 0.f,
			0.f, scaleY, 0.f, 0.f,
			0.f, 0.f, -(farPlane + nearPlane) / (farPlane - nearPlane), -1.f,
			0.f, 0.f, -(2.f * nearPlane * farPlane) / (farPlane - nearPlane), 0.f);
	return (result);
}

Matrix Matrix::orthographicProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	Matrix result(2.f / (right - left), 0.f, 0.f, 0.f,
				0.f, 2.f / (top - bottom), 0.f, 0.f,
				0.f, 0.f, -2.f / (farPlane - nearPlane), 0.f,
				-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(farPlane + nearPlane) / (farPlane - nearPlane), 1.f);
	return (result);
}

Matrix Matrix::lookAt(Vector3 const & eye, Vector3 const & center, Vector3 const & up)
{
	Vector3 f = (center - eye).normalize();
	Vector3 u = up;
	u.normalize();
	Vector3 s = f.cross(u).normalize();
	u = s.cross(f);

	Matrix result(s.x, u.x, -f.x, 0.f,
				s.y, u.y, -f.y, 0.f,
				s.z, u.z, -f.z, 0.f,
				-s.dotProduct(eye), -u.dotProduct(eye), f.dotProduct(eye), 1.f);
	return (result);
}

int Matrix::getMinor(Matrix const & src, Matrix & dest, int row, int col) const
{
	int colCount;
	int rowCount = 0;

	for (int i = 0; i < 4; i++)
	{
		if (i != row)
		{
			colCount = 0;
			for (int j = 0; j < 4; j++)
			{
				if (j != col)
				{
					dest[rowCount * 3 + colCount] = src.m_matrix[i * 4 + j];
					colCount++;
				}
			}
			rowCount++;
		}
	}
	return 1;
}

float Matrix::determinant3(Matrix const & matrix) const
{
	float const * m = matrix.m_matrix;
	float det =   m[0] * (m[4] * m[8] - m[5] * m[7])
				- m[1] * (m[3] * m[8] - m[5] * m[6])
				+ m[2] * (m[3] * m[7] - m[4] * m[6]);
	return (det);
}

std::ostream & operator<<(std::ostream & out, Matrix const & matrix)
{
	const float * m = matrix.getMatrix();
	out << "Matrix(";
	out << m[0 * 4 + 0] << ", " << m[0 * 4 + 1] << ", " << m[0 * 4 + 2] << ", " << m[0 * 4 + 3] << ", " << std::endl;
	out << m[1 * 4 + 0] << ", " << m[1 * 4 + 1] << ", " << m[1 * 4 + 2] << ", " << m[1 * 4 + 3] << ", " << std::endl;
	out << m[2 * 4 + 0] << ", " << m[2 * 4 + 1] << ", " << m[2 * 4 + 2] << ", " << m[2 * 4 + 3] << ", " << std::endl;
	out << m[3 * 4 + 0] << ", " << m[3 * 4 + 1] << ", " << m[3 * 4 + 2] << ", " << m[3 * 4 + 3] << ")" << std::endl;
	return (out);
}
