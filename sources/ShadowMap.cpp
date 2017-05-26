#include "ShadowMap.hpp"
#include "Color.hpp"

ShadowMap::ShadowMap(void) :
		RenderTexture(1024u, 1024u)
{
	RenderTarget::setClearColor(Color(0.1f, 0.1f, 0.1f, 1.f));
	RenderTarget::setClearMask(GL_DEPTH_BUFFER_BIT);
}

void ShadowMap::initTextureParam(void)
{
	// TODO Replace by a virtual method for each part of this method
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, getWidth(), getHeight(), 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	RenderTexture::createRenderTexture(GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
}