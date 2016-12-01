#include "RenderTarget.hpp"
#include "Color.hpp"
#include <GL/glew.h>

RenderTarget::RenderTarget(std::size_t width, std::size_t height) :
	m_width(width),
	m_height(height),
	m_clearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
{ }

void RenderTarget::setClearColor(Color const & color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderTarget::setViewport(void)
{
	glViewport(0, 0, m_width, m_height);
}

void RenderTarget::clear(void) const
{
	glClear(m_clearMask);
}

std::size_t RenderTarget::getWidth(void) const
{
	return (m_width);
}

std::size_t RenderTarget::getHeight(void) const
{
	return (m_height);
}

void RenderTarget::setWidth(std::size_t width)
{
	m_width = width;
}

void RenderTarget::setHeight(std::size_t height)
{
	m_height = height;
}

void RenderTarget::setClearMask(std::size_t clearMask)
{
	m_clearMask = clearMask;
}
