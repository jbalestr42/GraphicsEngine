#include "Matrix.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include <iostream>

int main()
{
	Vector3 v1 = Vector3(0.057f, 0.057f, 0.1f);
	Vector4 v2 = Vector4(0.057f, 0.057f, 0.1f, 1.0f);
	Matrix m = Matrix(-0.507f, 0.0000000037f, 0.86f, -0.104f,
						-0.104f, 0.992f, -0.061f, 0.855f,
						0.855f, 0.12f, 0.503f, 22.252f,
						22.252f, 7.29f, 24.85f, 0.f);

	std::cout << (v1 * m) << std::endl;
	std::cout << (v2 * m) << std::endl;
	std::cout << (m * v2) << std::endl;
	return 0;
}
