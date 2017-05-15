#include "Texture.hpp"
#include "Shader.hpp"
#include <iostream>

Texture::Texture(std::string const & filename) :
	m_textureID(0u),
	m_width(0u),
	m_height(0u),
	m_id(0u)
{
	glGenTextures(1, &m_textureID);
	glBindTexture(GL_TEXTURE_2D, m_textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // No blurry texture
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	ilGenImages(1, &m_id);
	ilBindImage(m_id);
	ilLoadImage(filename.c_str());
	m_width = ilGetInteger(IL_IMAGE_WIDTH);
	m_height = ilGetInteger(IL_IMAGE_HEIGHT);

	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_FALSE);
	glPixelStorei(GL_UNPACK_ROW_LENGTH, 0); // rows are tightly packed
	glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
	glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // pixels are tightly packep

	// TODO use exception
	int err = ilGetError();
	if (err)
		std::cout << err << " " << filename << std::endl;

	if (!ilGetData())
		return ;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, ilGetInteger(IL_IMAGE_FORMAT), ilGetInteger(IL_IMAGE_TYPE), ilGetData());
	ilBindImage(0);
}

Texture::~Texture(void)
{
	glDeleteTextures(1, &m_textureID);
	ilDeleteImage(m_id);
	ilBindImage(0u);
}

void Texture::bind(GLenum textureUnit, GLenum textureTarget)
{
	glActiveTexture(textureUnit);
	glBindTexture(textureTarget, m_textureID);
}

bool Texture::load(void)
{
	return (true);
}

std::size_t Texture::getHeight(void) const
{
	return m_height;
}

std::size_t Texture::getWidth(void) const
{
	return m_width;
}
