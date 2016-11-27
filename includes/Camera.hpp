#ifndef CAMERA_HPP
# define CAMERA_HPP

# include "IView.hpp"
# include "Vector2.hpp"
# include "Matrix.hpp"

class Camera : public IView
{
public:
	Camera(void);
	Camera(Camera const & camera);
	virtual ~Camera(void) = default;

	Camera & operator=(Camera const & camera);

	Vector3 const & getRotation(void) const;
	Vector3 const & getOriginUp(void) const;
	Vector3 const & getOriginDirection(void) const;
	Vector3 const & getDirection(void) const;
	Vector3 const & getRight(void) const;
	Vector3 const & getUp(void) const;

	void setPosition(Vector3 const & position);
	void setRotation(Vector3 const & rotation);
	void setOriginUp(Vector3 const & originUp);
	void setOriginDirection(Vector3 const & originDirection);

	virtual Matrix const & getProjectionMatrix(void) const;
	virtual Matrix const & getViewMatrix(void) const;
	virtual Vector3 const & getPosition(void) const;

	void update(float frametime);

	void lookAt(Vector3 const & position, Vector3 const & center, Vector3 const & up);

private:
	Vector3		m_originUp;
	Vector3		m_originDirection;
	Vector3		m_position;
	Vector3		m_rotation;
	Vector3		m_direction;
	Vector3		m_up;
	Vector3		m_right;
	Vector2		m_mousePosition;
	Matrix		m_view;
	Matrix		m_projection;
};

#endif
