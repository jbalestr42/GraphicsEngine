#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# define ILUT_USE_OPENGL

# if defined(__APPLE__) || defined(MACOSX)
#  include <OpenGL/gl3.h>
# else
#  include <GL/glew.h>
# endif
# include <cstddef>
# include <string>
# include <IL/il.h>

class Shader;

class Texture
{
public:
	Texture(std::string const & filename);
	virtual ~Texture(void);

	std::size_t getHeight(void) const;
	std::size_t getWidth(void) const;

	void bind(GLenum textureUnit, GLenum textureTarget);
	bool load(void);

private:
	GLuint			m_textureID;
	std::size_t		m_width;
	std::size_t		m_height;
	ILuint			m_id;

	Texture(void) = delete;
	Texture(Texture const & texture);
	Texture & operator=(Texture const & texture);

};

#endif
