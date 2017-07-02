#ifndef TEXTURE_HPP
# define TEXTURE_HPP

# define ILUT_USE_OPENGL

# include <string>
# include <IL/il.h>
# include "OpenGL.hpp"
# include "NonCopyable.hpp"

class Texture : public NonCopyable
{
public:
	Texture(std::string const & filename);
	virtual ~Texture(void);

	std::size_t getHeight(void) const;
	std::size_t getWidth(void) const;

	int getTextureID(void) const { return m_textureID; }

	void bind(GLenum textureUnit, GLenum textureTarget);
	bool load(void);

private:
	GLuint			m_textureID;
	std::size_t		m_width;
	std::size_t		m_height;
	ILuint			m_id;

	Texture(void) = delete;

};

#endif
