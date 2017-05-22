#include "DebugDraw.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include "Transformable.hpp"
#include "Camera.hpp"
#include "Vector4.hpp"
#include <cassert>

std::unique_ptr<DebugDraw> DebugDraw::m_instance = nullptr;

DebugDraw::DebugDraw(void) :
	m_shader(ResourceManager::getInstance().getShader(-1)),
	m_size(0u)
{
	// Creates vertices to draw the texture
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);
	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * MaxLine * 2, &m_vertices[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3)));

	glBindVertexArray(0);
}

DebugDraw::~DebugDraw(void)
{
	glDeleteBuffers(1, &m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

DebugDraw & DebugDraw::getInstance(void)
{
	if (m_instance == nullptr)
	{
		m_instance.reset(new DebugDraw());
	}
	return *m_instance;
}

void DebugDraw::drawLine(Vector3 const & start, Vector3 const & end, Color const & color)
{
	assert(m_size < MaxLine);
	m_vertices[m_size].position = start;
	m_vertices[m_size].color = color;
	m_vertices[m_size + 1].position = end;
	m_vertices[m_size + 1].color = color;
	m_size += 2;
}

void DebugDraw::drawTransform(Transformable const & transformable)
{
	//TODO remove scale from matrix
	m_shader->setParameter("ModelMatrix", transformable.getMatrix());
	drawLine(Vector3(0.f, 0.f, 0.f), Vector3(1.f, 0.f, 0.f), Color::Red);
	drawLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 1.f, 0.f), Color::Green);
	drawLine(Vector3(0.f, 0.f, 0.f), Vector3(0.f, 0.f, 1.f), Color::Blue);
	draw();
}

void DebugDraw::drawOrthogonalProjection(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	drawLine(Vector3(left, bottom, nearPlane), Vector3(left, top, nearPlane), Color::Red);
	drawLine(Vector3(left, top, nearPlane), Vector3(right, top, nearPlane), Color::Red);
	drawLine(Vector3(right, top, nearPlane), Vector3(right, bottom, nearPlane), Color::Red);
	drawLine(Vector3(right, bottom, nearPlane), Vector3(left, bottom, nearPlane), Color::Red);

	drawLine(Vector3(left, bottom, nearPlane), Vector3(left, bottom, farPlane), Color::Red);
	drawLine(Vector3(right, bottom, nearPlane), Vector3(right, bottom, farPlane), Color::Red);
	drawLine(Vector3(left, top, nearPlane), Vector3(left, top, farPlane), Color::Red);
	drawLine(Vector3(right, top, nearPlane), Vector3(right, top, farPlane), Color::Red);

	drawLine(Vector3(left, bottom, farPlane), Vector3(left, top, farPlane), Color::Red);
	drawLine(Vector3(left, top, farPlane), Vector3(right, top, farPlane), Color::Red);
	drawLine(Vector3(right, top, farPlane), Vector3(right, bottom, farPlane), Color::Red);
	drawLine(Vector3(right, bottom, farPlane), Vector3(left, bottom, farPlane), Color::Red);
}

void DebugDraw::drawFrustum(Camera const & camera, float xn, float yn, float xf, float yf, float near, float far)
{
	drawTransform(camera);
	m_shader->setParameter("ModelMatrix", camera.getMatrix());
	drawLine(Vector3(xn, yn, near), Vector3(-xn, yn, near), Color::White);
	drawLine(Vector3(-xn, yn, near), Vector3(-xn, -yn, near), Color::White);
	drawLine(Vector3(-xn, -yn, near), Vector3(xn, -yn, near), Color::White);
	drawLine(Vector3(xn, -yn, near), Vector3(xn, yn, near), Color::White);

	drawLine(Vector3(xf, yf, far), Vector3(-xf, yf, far), Color::White);
	drawLine(Vector3(-xf, yf, far), Vector3(-xf, -yf, far), Color::White);
	drawLine(Vector3(-xf, -yf, far), Vector3(xf, -yf, far), Color::White);
	drawLine(Vector3(xf, -yf, far), Vector3(xf, yf, far), Color::White);

	drawLine(Vector3(xn, yn, near), Vector3(xf, yf, far), Color::White);
	drawLine(Vector3(-xn, yn, near), Vector3(-xf, yf, far), Color::White);
	drawLine(Vector3(xn, -yn, near), Vector3(xf, -yf, far), Color::White);
	drawLine(Vector3(-xn, -yn, near), Vector3(-xf, -yf, far), Color::White);
	draw();
}

void DebugDraw::draw(void)
{
	m_shader->bind();

	glBindVertexArray(m_vertexArrayObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_size * 2, &m_vertices);
	glDrawArrays(GL_LINES, 0, m_size);
	glBindVertexArray(0);
	m_size = 0u;
}
