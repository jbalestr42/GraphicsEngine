#include "FrameBuffer.hpp"
#include <iostream>

FrameBuffer::FrameBuffer(void) :
	FrameBuffer(1024u, 1024u)
{}

FrameBuffer::FrameBuffer(std::size_t width, std::size_t height) :
	m_frameBufferObject(0u),
	m_texture(0u),
	m_width(width),
	m_height(height)
{
	glGenFramebuffers(1, &m_frameBufferObject);
	glGenTextures(1, &m_texture);
	//TODO use Texture class

	// Creates vertices to draw the texture
	GLfloat quad[] =
	{
		// Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
		// Positions   // TexCoords
		-1.0f, 1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f,
		1.0f,  -1.0f, 1.0f, 0.0f,
		-1.0f, 1.0f,  0.0f, 1.0f,
		1.0f,  -1.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};
	glGenVertexArrays(1, &m_vertexArrayObject);
	glGenBuffers(1, &m_vertexBufferObject);
	glBindVertexArray(m_vertexArrayObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad), &quad, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));
	glBindVertexArray(0);
}

FrameBuffer::~FrameBuffer(void)
{
	glDeleteFramebuffers(1, &m_frameBufferObject);
	glDeleteTextures(1, &m_texture);
	//TODO delete vao vbo
}

void FrameBuffer::bindFrameBuffer(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
}

void FrameBuffer::bindTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void FrameBuffer::init(void)
{
	bindTexture();

	initTextureParam();

// TODO use exception
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::initTextureParam(void)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(), getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	createFrameBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);

	//TODO use to do renderTexture
	//glGenRenderbuffers(1, &m_renderBufferObject);
	//glBindRenderbuffer(GL_RENDERBUFFER, m_renderBufferObject);
	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_width, m_height); // Use a single renderbuffer object for both a depth AND stencil buffer.
	//glBindRenderbuffer(GL_RENDERBUFFER, 0);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderBufferObject); // Now actually attach it
}

void FrameBuffer::createFrameBuffer(GLenum attachment, GLenum texTarget, GLint mipMapLevel)
{
	bindFrameBuffer();
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, texTarget, m_texture, mipMapLevel);
}

std::size_t FrameBuffer::getWidth(void) const
{
	return (m_width);
}

std::size_t FrameBuffer::getHeight(void) const
{
	return (m_height);
}

void FrameBuffer::draw(void)
{
	glBindVertexArray(m_vertexArrayObject);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}