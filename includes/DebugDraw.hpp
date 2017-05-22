#ifndef DEBUGDRAW_HPP
# define DEBUGDRAW_HPP

# include <GL/glew.h>
# include <memory>
# include "Vertex.hpp"

class Shader;
class Transformable;
class Camera;

class DebugDraw
{
public:
	DebugDraw(void);
	virtual ~DebugDraw(void);

	static DebugDraw & getInstance(void);

	void drawLine(Vector3 const & start, Vector3 const & end, Color const & color = Color::White);
	void drawTransform(Transformable const & transformable);
	void drawOrthogonalProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void drawFrustum(Camera const & camera, float xn, float yn, float xf, float yf, float near, float far);

private:
	static std::unique_ptr<DebugDraw>	m_instance;
	static const std::size_t MaxLine = 100;

	std::shared_ptr<Shader>		m_shader;
	GLuint						m_vertexArrayObject;
	GLuint						m_vertexBufferObject;
	Vertex						m_vertices[MaxLine * 2u];
	std::size_t					m_size;

	void draw(void);

};

#endif
