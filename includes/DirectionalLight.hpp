#ifndef DIRECTIONNALLIGHT_HPP
# define DIRECTIONNALLIGHT_HPP

# include "Light.hpp"

class DirectionalLight : public Light
{
public:
	DirectionalLight(void);
	DirectionalLight(Color const & color, float ambientIntensity);
	DirectionalLight(DirectionalLight const & directionnalLight);
	virtual ~DirectionalLight(void) = default;

	DirectionalLight & operator=(DirectionalLight const & directionnalLight);

	float getAmbientIntensity(void) const;

private:
	float	m_ambientIntensity;

};

#endif
