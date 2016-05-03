#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(void) :
	Light(),
	m_ambientIntensity(1.f)
{ }

DirectionalLight::DirectionalLight(Color const & color, float ambientIntensity) :
	Light(color),
	m_ambientIntensity(ambientIntensity)
{ }

DirectionalLight::DirectionalLight(DirectionalLight const & directionnalLight) :
	Light(directionnalLight)
{
	*this = directionnalLight;
}

DirectionalLight & DirectionalLight::operator=(DirectionalLight const & directionnalLight)
{
	Light::operator=(directionnalLight);
	m_ambientIntensity = directionnalLight.m_ambientIntensity;
	return (*this);
}

float DirectionalLight::getAmbientIntensity(void) const
{
	return (m_ambientIntensity);
}
