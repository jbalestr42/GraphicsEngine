#ifndef DIRECTIONNALLIGHT_HPP
# define DIRECTIONNALLIGHT_HPP

# include "Light.hpp"

class DirectionalLight : public Light
{
public:
	static const std::size_t	MaxLight = 10u;

	DirectionalLight(void);
	DirectionalLight(Color const & color, float ambientIntensity = 1.f, float diffuseIntensity = 1.f);
	DirectionalLight(DirectionalLight const & directionnalLight);
	virtual ~DirectionalLight(void) = default;

	DirectionalLight & operator=(DirectionalLight const & directionnalLight);

	Vector3 getRotatedDirection(void);
	Vector3 const & getDirection(void) const;
	float getAmbientIntensity(void) const;
	float getDiffuseIntensity(void) const;

private:
	//TODO move in Light
	Vector3	m_direction;
	float	m_ambientIntensity;
	float	m_diffuseIntensity;

};

#endif
