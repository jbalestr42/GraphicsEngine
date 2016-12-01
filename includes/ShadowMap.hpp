#ifndef SHADOWMAP_HPP
# define SHADOWMAP_HPP

# include "FrameBuffer.hpp"

class ShadowMap : public FrameBuffer
{
public:
	ShadowMap(void);
	virtual ~ShadowMap(void) = default;

	virtual void initTextureParam(void);
};


#endif
