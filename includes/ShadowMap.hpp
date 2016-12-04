#ifndef SHADOWMAP_HPP
# define SHADOWMAP_HPP

# include "RenderTexture.hpp"

class ShadowMap : public RenderTexture
{
public:
	ShadowMap(void);
	virtual ~ShadowMap(void) = default;

	virtual void initTextureParam(void);
};


#endif
