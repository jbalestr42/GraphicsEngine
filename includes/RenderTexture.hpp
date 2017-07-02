#ifndef FRAMEBUFFER_HPP
# define FRAMEBUFFER_HPP

# include <GL/glew.h>
# include "IDrawable.hpp"
# include "RenderTarget.hpp"

class Shader;

class RenderTexture : public RenderTarget, public IDrawable
{
public:
	RenderTexture(void);
	RenderTexture(std::size_t width, std::size_t height);
	virtual ~RenderTexture(void);

	virtual void bind(void);
	void init(void);
	virtual void draw(Shader & shader) const;

	void bindTexture(GLenum textureUnit);

protected:
	void createRenderTexture(GLenum attachment, GLenum texTarget, GLint mipMapLevel = 0u);
	virtual void initTextureParam(void);

private:
	GLuint			m_vertexArrayObject;
	GLuint			m_vertexBufferObject;
	GLuint			m_frameBufferObject;
	GLuint			m_texture;

};

#endif
