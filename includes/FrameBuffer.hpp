#ifndef FRAMEBUFFER_HPP
# define FRAMEBUFFER_HPP

# include <cstddef>
# include <GL/glew.h>

class FrameBuffer
{
public:
	FrameBuffer(void);
	FrameBuffer(std::size_t width, std::size_t height);
	virtual ~FrameBuffer(void);

	void bindFrameBuffer(void);
	virtual void initTextureParam(void);

	std::size_t getWidth(void) const;
	std::size_t getHeight(void) const;

protected:
	void bindTexture(void);
	void createFrameBuffer(GLenum attachment, GLenum texTarget, GLint mipMapLevel = 0u);

private:
	GLuint			m_frameBufferObject;
	GLuint			m_texture;
	std::size_t		m_width;
	std::size_t		m_height;

	FrameBuffer(FrameBuffer const & frameBuffer);
	FrameBuffer & operator=(FrameBuffer const & frameBuffer);

};

#endif
