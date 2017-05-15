#ifndef SPOTLIGHT_HPP
# define SPOTLIGHT_HPP

# include "DirectionalLight.hpp"

class SpotLight : public DirectionalLight
{
public:
	static const std::size_t	MaxLight = 10u;

	SpotLight(void);
	SpotLight(Color const & color, Vector3 const & position, float constantAttenuation = 1.f, float linearAttenuation = 0.09f, float quadraticAttenuation = 0.032f);
	SpotLight(SpotLight const & spotLight);
	virtual ~SpotLight(void) = default;

	SpotLight & operator=(SpotLight const & spotLight);

	void setAngles(float innerAngle, float outerAngle);
	void setAttenuation(float constantAttenuation, float linearAttenuation, float quadraticAttenuation);

	float getInnerAngle(void) const;
	float getOuterAngle(void) const;
	float getConstantAttenuation(void) const;
	float getLinearAttenuation(void) const;
	float getQuadraticAttenuation(void) const;

private:
	float		m_innerAngle;
	float		m_outerAngle;
	float		m_constantAttenuation;
	float		m_linearAttenuation;
	float		m_quadraticAttenuation;

};

#endif
