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
	m_shader->setParameter("ModelMatrix", Matrix());
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
	draw();
}

#include "Math.hpp"
void DebugDraw::drawFrustum(Camera const & camera)
{
	float ar = static_cast<float>(800.f / 600.f);
	float fov = 60.f;
	float near = 0.1f;
	float far = 100.f;
	float halfHeight = tanf(Deg2Rad * (fov / 2.f));
	float halfWidth = halfHeight * ar;

	//drawTransform(camera);
	m_shader->setParameter("ModelMatrix", Matrix());
	//--------------------------------------

	//Vector3 r = camera.m_right * halfWidth * far;
	//Vector3 t = camera.m_up * halfHeight * far;
	//Vector3 c = camera.getPosition() + camera.m_direction * far;

	//Vector3 nr = camera.m_right * halfWidth * near;
	//Vector3 nt = camera.m_up * halfHeight * near;
	//Vector3 nc = camera.getPosition() + camera.m_direction * near;

	//drawLine(c + t + r, c + t - r, Color::White);
	//drawLine(c + t + r, c - t + r, Color::White);
	//drawLine(c - t - r, c - t + r, Color::White);
	//drawLine(c - t - r, c + t - r, Color::White);

	//drawLine(nc + nt + nr, nc + nt - nr, Color::White);
	//drawLine(nc + nt + nr, nc - nt + nr, Color::White);
	//drawLine(nc - nt - nr, nc - nt + nr, Color::White);
	//drawLine(nc - nt - nr, nc + nt - nr, Color::White);

	// -------------------------------- 

#if 1
	Matrix inv = (camera.getViewMatrix() * camera.getProjectionMatrix()).inverse();
	Vector4 f[8u] =
	{
		// near face
		{1, 1, -1, 1.f},
		{-1, 1, -1, 1.f},
		{1, -1, -1, 1.f},
		{-1, -1, -1 , 1.f},

		// 1 face
		{1, 1, 1, 1.f},
		{-1, 1, 1 , 1.f},
		{1, -1, 1 , 1.f},
		{-1, -1,1, 1.f},
	};

	Vector3 v[8];
	for (int i = 0; i < 8; i++)
	{
		Vector4 ff = inv * f[i];
		v[i].x = ff.x / ff.w;
		v[i].y = ff.y / ff.w;
		v[i].z = ff.z / ff.w;
	}

	drawLine(v[0], v[1], Color::White);
	drawLine(v[0], v[2], Color::White);
	drawLine(v[3], v[1], Color::White);
	drawLine(v[3], v[2], Color::White);

	drawLine(v[4], v[5], Color::White);
	drawLine(v[4], v[6], Color::White);
	drawLine(v[7], v[5], Color::White);
	drawLine(v[7], v[6], Color::White);

	drawLine(v[0], v[4], Color::White);
	drawLine(v[1], v[5], Color::White);
	drawLine(v[3], v[7], Color::White);
	drawLine(v[2], v[6], Color::White);
#else
	Matrix inv = camera.getViewMatrix().inverse();

	float xn = halfWidth * near;
	float xf = halfWidth * far;
	float yn = halfHeight * near;
	float yf = halfHeight * far;

	Vector4 f[8u] =
	{
		// near face
		{xn, yn, near, 1.f},
		{-xn, yn, near, 1.f},
		{xn, -yn, near, 1.f},
		{-xn, -yn,near , 1.f},

		// far face
		{xf, yf, far, 1.f},
		{-xf, yf,far , 1.f},
		{xf, -yf,far , 1.f},
		{-xf, -yf,far, 1.f},
	};

	Vector3 v[8];
	for (int i = 0; i < 8; i++)
	{
		Vector4 ff = inv * f[i];
		v[i].x = ff.x / ff.w;
		v[i].y = ff.y / ff.w;
		v[i].z = ff.z / ff.w;
	}

	drawLine(v[0], v[1], Color::White);
	drawLine(v[0], v[2], Color::White);
	drawLine(v[3], v[1], Color::White);
	drawLine(v[3], v[2], Color::White);

	drawLine(v[4], v[5], Color::White);
	drawLine(v[4], v[6], Color::White);
	drawLine(v[7], v[5], Color::White);
	drawLine(v[7], v[6], Color::White);

	drawLine(v[0], v[4], Color::White);
	drawLine(v[1], v[5], Color::White);
	drawLine(v[3], v[7], Color::White);
	drawLine(v[2], v[6], Color::White);
#endif
	draw();
}

void DebugDraw::draw(void)
{
	// TODO draw debug in front of everything
	m_shader->bind();

	glBindVertexArray(m_vertexArrayObject);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * m_size * 2, &m_vertices);
	glDrawArrays(GL_LINES, 0, m_size);
	glBindVertexArray(0);
	m_size = 0u;
}
