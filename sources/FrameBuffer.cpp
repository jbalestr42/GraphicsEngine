#include "FrameBuffer.hpp"

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
}

FrameBuffer::~FrameBuffer(void)
{
	glDeleteFramebuffers(1, &m_frameBufferObject);
	glDeleteTextures(1, &m_texture);
}

void FrameBuffer::bindFrameBuffer(void)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_frameBufferObject);
}

void FrameBuffer::bindTexture(void)
{
	glBindTexture(GL_TEXTURE_2D, m_texture);
}

void FrameBuffer::initTextureParam(void)
{
	bindTexture();
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, getWidth(), getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	createFrameBuffer(GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
