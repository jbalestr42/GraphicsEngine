#include "Matrix.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Math.hpp"
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>

int main()
{
	glm::vec3 eye(2.f, -41.f, 12.f);
	glm::vec3 target(2.f, 7.f, -5.f);
	glm::vec3 up(0.f, 1.f, 0.f);
	glm::vec4 v(23.4f, -2.03f, 12.6f, 0.5f);
	glm::mat4 look = glm::lookAt(eye, target, up);

	Vector3 veye(2.f, -41.f, 12.f);
	Vector3 vtarget(2.f, 7.f, -5.f);
	Vector3 vup(0.f, 1.f, 0.f);
	Vector4 vv(23.4f, -2.03f, 12.6f, 0.5f);
	Matrix vlook = Matrix::lookAt(veye, vtarget, vup);

	/* TEST OK matrix * vector4
	std::cout << vv * vlook << std::endl;
	std::cout << glm::to_string(v * look) << std::endl;
	std::cout << vlook * vv << std::endl;
	std::cout << glm::to_string(look * v) << std::endl;
	*/

	/* TEST OK inverse
	std::cout << vlook.inverse() << std::endl;
	std::cout << glm::to_string(glm::inverse(look)) << std::endl;
	*/

	/* TEST OK perspective projection
	 * fov as radian in glm
	std::cout << Matrix::perspectiveProjection(60.f, 1920.f / 1080.f, 0.1f, 100.f) << std::endl;
	std::cout << glm::to_string(glm::perspective(60.f, 1920.f / 1080.f, 0.1f, 100.f)) << std::endl;
	*/

	/* TEST OK lookAt
	std::cout << vlook << std::endl;
	std::cout << glm::to_string(look) << std::endl;
	*/

	/* TEST OK
	std::cout << veye.cross(vtarget) << std::endl;
	std::cout << glm::to_string(glm::cross(eye, target)) << std::endl;
	*/

	/* TEST OK matrix mul
	glm::mat4 mm(2.f, 65.f, 21.f, -12.f,
			-5.f, 12.f, 323.f, 45.f,
			65.f, 21.f, -5.f, 12.f,
			2.f, 323.f, 65.f, 21.f);

	Matrix vmm(2.f, 65.f, 21.f, -12.f,
			-5.f, 12.f, 323.f, 45.f,
			65.f, 21.f, -5.f, 12.f,
			2.f, 323.f, 65.f, 21.f);
	std::cout << glm::to_string(mm * mm) << std::endl;
	std::cout << vmm * vmm << std::endl;
	*/

	//glm::mat4 m1 = glm::eulerAngleYXZ(2.f, 6.f, 20.f);
	//std::cout << glm::to_string(m1) << std::endl;

	//Matrix vm1 = vlook.eulerAnglesToMatrix({6.f, 2.f, 20.f}, Matrix::EulerOrder::YXZ);
	//std::cout << vm1 << std::endl;
	return 0;
}
