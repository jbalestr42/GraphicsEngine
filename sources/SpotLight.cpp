#include "SpotLight.hpp"

SpotLight::SpotLight(void) :
	SpotLight(Color::White, Vector3(0.f, 0.f, 0.f))
{ }

SpotLight::SpotLight(Color const & color, Vector3 const & position, float constantAttenuation, float linearAttenuation, float quadraticAttenuation) :
	DirectionalLight(color),
	m_innerAngle(0.f),
	m_outerAngle(0.f),
	m_constantAttenuation(constantAttenuation),
	m_linearAttenuation(linearAttenuation),
	m_quadraticAttenuation(quadraticAttenuation)
{
	setPosition(position);
}

SpotLight::SpotLight(SpotLight const & spotLight) :
	DirectionalLight(spotLight)
{
	*this = spotLight;
}

SpotLight & SpotLight::operator=(SpotLight const & spotLight)
{
	DirectionalLight::operator=(spotLight);
	setAngles(spotLight.m_innerAngle, spotLight.m_outerAngle);
	setAttenuation(spotLight.m_constantAttenuation, spotLight.m_linearAttenuation, spotLight.m_quadraticAttenuation);
	return (*this);
}

void SpotLight::setAngles(float innerAngle, float outerAngle)
{
	m_innerAngle = innerAngle;
	m_outerAngle = outerAngle;
}

void SpotLight::setAttenuation(float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
	m_constantAttenuation = constantAttenuation;
	m_linearAttenuation = linearAttenuation;
	m_quadraticAttenuation = quadraticAttenuation;
}

float SpotLight::getInnerAngle(void) const
{
	return (m_innerAngle);
}

float SpotLight::getOuterAngle(void) const
{
	return (m_outerAngle);
}

float SpotLight::getConstantAttenuation(void) const
{
	return (m_constantAttenuation);
}

float SpotLight::getLinearAttenuation(void) const
{
	return (m_linearAttenuation);
}

float SpotLight::getQuadraticAttenuation(void) const
{
	return (m_quadraticAttenuation);
}
