#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Quaternion.hpp"

Camera::Camera(void) :
	m_originUp(0.f, 1.f, 0.f),
	m_originDirection(0.f, 0.f, 1.f)
{ }

Camera::Camera(Camera const & camera)
{
	*this = camera;
}

Camera::~Camera(void) { }

Camera & Camera::operator=(Camera const & camera)
{
	m_originUp = camera.m_originUp;
	m_originDirection = camera.m_originDirection;
	m_position = camera.m_position;
	m_rotation = camera.m_rotation;
	m_mousePosition = camera.m_mousePosition;
	m_view = camera.m_view;
	return (*this);
}

void Camera::lookAt(Vector3 const & position, Vector3 const & center, Vector3 const & up)
{
	Vector3 f = (center - position).normalize();
	Vector3 u = up;
	u.normalize();
	Vector3 s = f.cross(u).normalize();
	u = s.cross(f);
	u.normalize();

	m_view[0] = s.x;
	m_view[4] = s.y;
	m_view[8] = s.z;
	m_view[3] = 0.f;
	m_view[1] = u.x;
	m_view[5] = u.y;
	m_view[9] = u.z;
	m_view[7] = 0.f;
	m_view[2] = f.x;
	m_view[6] = f.y;
	m_view[10] = f.z;
	m_view[11] = 0.f;
	m_view[12] = s.dotProduct(center);
	m_view[13] = u.dotProduct(center);
	m_view[14] = f.dotProduct(center);
	m_view[15] = 1.f;
}

Matrix const & Camera::getView(void) const
{
	return (m_view);
}

void Camera::update(float frametime)
{
	static float speed = frametime * 10000.f;
	Vector2 delta = m_mousePosition - Mouse::getPosition();
	m_mousePosition = Mouse::getPosition();

	Quaternion q;
	m_rotation.x += delta.y * frametime * 100.f;
	m_rotation.y -= delta.x * frametime * 100.f;
	q.fromEuler(m_rotation);
	Vector3 direction = m_originDirection.rotate(q).normalize();
	Vector3 right = direction.cross(m_originUp).normalize();
	Vector3 up = right.cross(direction).normalize();

	if (Keyboard::isKeyPress(GLFW_KEY_A))
		m_position += right * frametime * speed;
	else if (Keyboard::isKeyPress(GLFW_KEY_D))
		m_position -= right * frametime * speed;
	if (Keyboard::isKeyPress(GLFW_KEY_W))
		m_position += direction * frametime * speed;
	else if (Keyboard::isKeyPress(GLFW_KEY_S))
		m_position -= (direction * frametime * speed);
	if (Keyboard::isKeyPress(GLFW_KEY_Q))
		m_position += up * frametime * speed;
	else if (Keyboard::isKeyPress(GLFW_KEY_E))
		m_position -= up * frametime * speed;

	lookAt(m_position, m_position + direction, up);
}
