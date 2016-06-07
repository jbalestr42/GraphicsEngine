#include "Light.hpp"

Light::Light(void) :
	Light(Color())
{}

Light::Light(Color const & color) :
	Transformable(),
	m_color(color),
	m_ambientIntensity(0.2f)
{}

Light::Light(Light const & light) :
	Transformable(light)
{
	*this = light;
}

Light & Light::operator=(Light const & light)
{
	Transformable::operator=(light);
	m_color = light.m_color;
	m_ambientIntensity = light.m_ambientIntensity;
	return (*this);
}

Color const & Light::getColor(void) const
{
	return (m_color);
}

void Light::setAmbientIntensity(float ambientIntensity)
{
	m_ambientIntensity = ambientIntensity;
}

float Light::getAmbientIntensity(void) const
{
	return (m_ambientIntensity);
}
