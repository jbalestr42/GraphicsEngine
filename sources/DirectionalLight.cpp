#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(void) :
	DirectionalLight(Color(), 1.f, 1.f)
{ }

DirectionalLight::DirectionalLight(Color const & color, float ambientIntensity, float diffuseIntensity) :
	Light(color),
	m_direction({0.f, 0.f, 1.f}),
	m_ambientIntensity(ambientIntensity),
	m_diffuseIntensity(diffuseIntensity)
{}

DirectionalLight::DirectionalLight(DirectionalLight const & directionnalLight) :
	Light(directionnalLight)
{
	*this = directionnalLight;
}

DirectionalLight & DirectionalLight::operator=(DirectionalLight const & directionnalLight)
{
	Light::operator=(directionnalLight);
	m_direction = directionnalLight.m_direction;
	m_ambientIntensity = directionnalLight.m_ambientIntensity;
	m_diffuseIntensity = directionnalLight.m_diffuseIntensity;
	return (*this);
}

Vector3 DirectionalLight::getRotatedDirection(void)
{
	return (m_direction * getMatrix());
}

Vector3 const & DirectionalLight::getDirection(void) const
{
	return (m_direction);
}

float DirectionalLight::getAmbientIntensity(void) const
{
	return (m_ambientIntensity);
}

float DirectionalLight::getDiffuseIntensity(void) const
{
	return (m_diffuseIntensity);
}
