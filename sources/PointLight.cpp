#include "PointLight.hpp"

PointLight::PointLight(void) :
	PointLight(Color::White, Vector3(0.f, 0.f, 0.f))
{ }

PointLight::PointLight(Color const & color, Vector3 const & position, float constantAttenuation, float linearAttenuation, float quadraticAttenuation) :
	Light(color), //TODO
	m_constantAttenuation(constantAttenuation),
	m_linearAttenuation(linearAttenuation),
	m_quadraticAttenuation(quadraticAttenuation)
{
	setPosition(position);
}

PointLight::PointLight(PointLight const & pointLight) :
	Light(pointLight)
{
	*this = pointLight;
}

PointLight & PointLight::operator=(PointLight const & pointLight)
{
	Light::operator=(pointLight);
	m_constantAttenuation = pointLight.m_constantAttenuation;
	m_linearAttenuation = pointLight.m_linearAttenuation;
	m_quadraticAttenuation = pointLight.m_quadraticAttenuation;
	return (*this);
}

void PointLight::setAttenuation(float constantAttenuation, float linearAttenuation, float quadraticAttenuation)
{
	m_constantAttenuation = constantAttenuation;
	m_linearAttenuation = linearAttenuation;
	m_quadraticAttenuation = quadraticAttenuation;
}

float PointLight::getConstantAttenuation(void) const
{
	return (m_constantAttenuation);
}

float PointLight::getLinearAttenuation(void) const
{
	return (m_linearAttenuation);
}

float PointLight::getQuadraticAttenuation(void) const
{
	return (m_quadraticAttenuation);
}

void PointLight::computeShadowMap(Camera const &)
{
	//TODO std::cout << "shadow map for pointlight not implemented" << std::endl;
}
