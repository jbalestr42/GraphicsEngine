#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "Transformable.hpp"
# include "IView.hpp"
# include "Vector2.hpp"
# include "Matrix.hpp"

class Camera : public IView, public Transformable
{
public:
	Camera(void);
	Camera(std::size_t width, std::size_t height);
	Camera(Camera const & camera);
	virtual ~Camera(void) = default;

	Camera & operator=(Camera const & camera);

	Vector3 const & getOriginUp(void) const;
	Vector3 const & getOriginDirection(void) const;
	Vector3 const & getDirection(void) const;
	Vector3 const & getRight(void) const;
	Vector3 const & getUp(void) const;
	std::size_t getWidth(void) const;
	std::size_t getHeight(void) const;
	float getFov(void) const;
	float getNearPlane(void) const;
	float getFarPlane(void) const;

	virtual Vector3 const & getPosition(void) const;
	virtual Matrix const & getProjectionMatrix(void) const;
	virtual Matrix const & getViewMatrix(void) const;

	void setOriginUp(Vector3 const & originUp);
	void setOriginDirection(Vector3 const & originDirection);
	void setFov(float fov);
	void setNearPlane(float nearPlane);
	void setFarPlane(float farPlane);

	void update(float frametime);

private:
	Vector3		m_originUp;
	Vector3		m_originDirection;
	Vector3		m_direction;
	Vector3		m_up;
	Vector3		m_right;
	Vector2		m_mousePosition;
	Matrix		m_view;
	Matrix		m_projection;

	std::size_t	m_width;
	std::size_t	m_height;
	float		m_fov;
	float		m_near;
	float		m_far;

};

#endif
