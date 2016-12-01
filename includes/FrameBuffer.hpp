#ifndef FRAMEBUFFER_HPP
# define FRAMEBUFFER_HPP

# include <cstddef>
# include <GL/glew.h>
# include "IDrawable.hpp"
# include "RenderTarget.hpp"

class Shader;

class FrameBuffer : public RenderTarget, public IDrawable
{
public:
	FrameBuffer(void);
	FrameBuffer(std::size_t width, std::size_t height);
	virtual ~FrameBuffer(void);

	virtual void bind(void);
	void init(void);
	void draw(Shader & shader);

	void bindTexture(void);

protected:
	void createFrameBuffer(GLenum attachment, GLenum texTarget, GLint mipMapLevel = 0u);
	virtual void initTextureParam(void);

private:
	GLuint			m_vertexArrayObject;
	GLuint			m_vertexBufferObject;
	GLuint			m_frameBufferObject;
	GLuint			m_texture;

	FrameBuffer(FrameBuffer const & frameBuffer);
	FrameBuffer & operator=(FrameBuffer const & frameBuffer);

};

#endif
