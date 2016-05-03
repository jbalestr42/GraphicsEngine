#include "Light.hpp"

Light::Light(void) :
	Light(Color())
{}

Light::Light(Color const & color) :
	Transformable(),
	m_color(color)
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
	return (*this);
}

Color const & Light::getColor(void) const
{
	return (m_color);
}
