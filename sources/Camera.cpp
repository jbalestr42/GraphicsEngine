#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Quaternion.hpp"

Camera::Camera(void) :
	m_originUp(0.f, 1.f, 0.f),
	m_originDirection(0.f, 0.f, 1.f)
{
	//TODO get the right size
	m_projection = Matrix::perspectiveProjection(60.f, 800.f / 600.f, 0.1f, 100.f);
}

Camera::Camera(Camera const & camera)
{
	*this = camera;
}

Camera & Camera::operator=(Camera const & camera)
{
	m_originUp = camera.m_originUp;
	m_originDirection = camera.m_originDirection;
	m_position = camera.m_position;
	m_rotation = camera.m_rotation;
	m_mousePosition = camera.m_mousePosition;
	m_view = camera.m_view;
	m_projection = camera.m_projection;
	return (*this);
}

Vector3 const & Camera::getPosition(void) const
{
	return (m_position);
}

Vector3 const & Camera::getRotation(void) const
{
	return (m_rotation);
}

Vector3 const & Camera::getOriginUp(void) const
{
	return (m_originUp);
}

Vector3 const & Camera::getOriginDirection(void) const
{
	return (m_originDirection);
}

void Camera::setPosition(Vector3 const & position)
{
	m_position = position;
}

void Camera::setRotation(Vector3 const & rotation)
{
	m_rotation = rotation;
}

void Camera::setOriginUp(Vector3 const & originUp)
{
	m_originUp = originUp;
}

void Camera::setOriginDirection(Vector3 const & originDirection)
{
	m_originDirection = originDirection;
}

Matrix const & Camera::getProjectionMatrix(void) const
{
	return (m_projection);
}

Matrix const & Camera::getViewMatrix(void) const
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

	m_view = Matrix::lookAt(m_position, m_position + direction, up);
}
