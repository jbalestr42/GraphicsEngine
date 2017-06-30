#ifndef POINTLIGHT_HPP
# define POINTLIGHT_HPP

# include "Light.hpp"
# include <cstddef>

class PointLight : public Light
{
public:
	static const std::size_t	MaxLight = 10u;

	PointLight(void);
	PointLight(Color const & color, Vector3 const & position, float constantAttenuation = 1.f, float linearAttenuation = 0.09f, float quadraticAttenuation = 0.032f);
	PointLight(PointLight const & pointLight);
	virtual ~PointLight(void) = default;

	PointLight & operator=(PointLight const & pointLight);

	void setAttenuation(float constantAttenuation, float linearAttenuation, float quadraticAttenuation);

	float getConstantAttenuation(void) const;
	float getLinearAttenuation(void) const;
	float getQuadraticAttenuation(void) const;

	virtual void computeShadowMap(Camera const & camera);

private:
	float		m_constantAttenuation;
	float		m_linearAttenuation;
	float		m_quadraticAttenuation;

};

#endif
