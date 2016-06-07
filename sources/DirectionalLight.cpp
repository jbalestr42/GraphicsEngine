#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(void) :
	DirectionalLight(Color())
{ }

DirectionalLight::DirectionalLight(Color const & color) :
	Light(color),
	m_direction({0.f, 0.f, 1.f})
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
