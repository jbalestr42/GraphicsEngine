#include "Light.hpp"

Light::Light(void) :
	Light(Color())
{}

Light::Light(Color const & color) :
	Transformable(),
	m_color(color),
	m_ambientIntensity(0.2f),
	m_diffuseIntensity(1.f)
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
	m_diffuseIntensity = light.m_diffuseIntensity;
	return (*this);
}

Color const & Light::getColor(void) const
{
	return (m_color);
}

float Light::getAmbientIntensity(void) const
{
	return (m_ambientIntensity);
}

float Light::getDiffuseIntensity(void) const
{
	return (m_diffuseIntensity);
}
