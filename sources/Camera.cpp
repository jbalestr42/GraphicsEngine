#include "Camera.hpp"
#include "Keyboard.hpp"
#include "Mouse.hpp"
#include "Quaternion.hpp"

Camera::Camera(void) :
	Camera(800u, 600u)
{ }

Camera::Camera(std::size_t width, std::size_t height) :
	Transformable(),
	m_originUp(0.f, 1.f, 0.f),
	m_originDirection(0.f, 0.f, -1.f),
	m_width(width),
	m_height(height),
	m_fov(60.f),
	m_near(0.1f),
	m_far(100.f)
{
	m_projection = Matrix::perspectiveProjection(m_fov, static_cast<float>(m_width) / static_cast<float>(m_height), m_near, m_far);
}

Camera::Camera(Camera const & camera) :
	Transformable(camera)
{
	*this = camera;
}

Camera & Camera::operator=(Camera const & camera)
{
	Transformable::operator=(camera);
	m_originUp = camera.m_originUp;
	m_originDirection = camera.m_originDirection;
	m_direction = camera.m_direction;
	m_up = camera.m_up;
	m_right = camera.m_right;
	m_mousePosition = camera.m_mousePosition;
	m_view = camera.m_view;
	m_projection = camera.m_projection;
	m_width = camera.m_width;
	m_height = camera.m_height;
	m_fov = camera.m_fov;
	m_near = camera.m_near;
	m_far = camera.m_far;
	return (*this);
}

Vector3 const & Camera::getPosition(void) const
{
	return (Transformable::getPosition());
}

Vector3 const & Camera::getOriginUp(void) const
{
	return (m_originUp);
}

Vector3 const & Camera::getOriginDirection(void) const
{
	return (m_originDirection);
}

Vector3 const & Camera::getDirection(void) const
{
	return (m_direction);
}

Vector3 const & Camera::getRight(void) const
{
	return (m_right);
}

Vector3 const & Camera::getUp(void) const
{
	return (m_up);
}

std::size_t Camera::getWidth(void) const
{
	return (m_width);
}

std::size_t Camera::getHeight(void) const
{
	return (m_height);
}

float Camera::getFov(void) const
{
	return (m_fov);
}

float Camera::getNearPlane(void) const
{
	return (m_near);
}

float Camera::getFarPlane(void) const
{
	return (m_far);
}

Matrix const & Camera::getViewMatrix(void) const
{
	return (m_view);
}

Matrix const & Camera::getProjectionMatrix(void) const
{
	return (m_projection);
}

void Camera::setOriginUp(Vector3 const & originUp)
{
	m_originUp = originUp;
}

void Camera::setOriginDirection(Vector3 const & originDirection)
{
	m_originDirection = originDirection;
}

void Camera::setFov(float fov)
{
	m_fov = fov;
}

void Camera::setNearPlane(float nearPlane)
{
	m_near = nearPlane;
}

void Camera::setFarPlane(float farPlane)
{
	m_far = farPlane;
}

void Camera::update(float frametime)
{
	static const float speed = frametime * 10000.f; // TODO add speed variable
	Vector2 delta = m_mousePosition - Mouse::getPosition();
	m_mousePosition = Mouse::getPosition();

	Quaternion q;
	Transformable::rotateX(-delta.y * frametime * 100.f);
	Transformable::rotateY(delta.x * frametime * 100.f);
	q.fromEuler(Transformable::getRotation());
	m_direction = m_originDirection.rotate(q).normalize();
	m_right = m_originUp.cross(m_direction).normalize();
	m_up = m_direction.cross(m_right).normalize();

	if (Keyboard::isKeyPress(GLFW_KEY_A))
		Transformable::translate(m_right * frametime * speed);
	else if (Keyboard::isKeyPress(GLFW_KEY_D))
		Transformable::translate(-m_right * frametime * speed);
	if (Keyboard::isKeyPress(GLFW_KEY_W))
		Transformable::translate(m_direction * frametime * speed);
	else if (Keyboard::isKeyPress(GLFW_KEY_S))
		Transformable::translate(-m_direction * frametime * speed);
	if (Keyboard::isKeyPress(GLFW_KEY_Q))
		Transformable::translate(-m_up * frametime * speed);
	else if (Keyboard::isKeyPress(GLFW_KEY_E))
		Transformable::translate(m_up * frametime * speed);

	m_view = Matrix::lookAt(Transformable::getPosition(), Transformable::getPosition() + m_direction, m_up);
}
