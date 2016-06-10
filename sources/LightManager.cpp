#include "LightManager.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include <cassert>

DirectionalLight & LightManager::createDirectionalLight(Color const & color)
{
	m_directionalLight.emplace_back(color);
	return (m_directionalLight.back());
}

PointLight & LightManager::createPointLight(Color const & color, Vector3 const & position)
{
	m_pointLight.emplace_back(color, position);
	return (m_pointLight.back());
}

DirectionalLight & LightManager::getDirectionalLight(std::size_t index)
{
	assert(index < m_directionalLight.size());
	return (m_directionalLight[index]);
}

DirectionalLight const & LightManager::getDirectionalLight(std::size_t index) const
{
	return (getDirectionalLight(index));
}

PointLight & LightManager::getPointLight(std::size_t index)
{
	assert(index < m_pointLight.size());
	return (m_pointLight[index]);
}

PointLight const & LightManager::getPointLight(std::size_t index) const
{
	return (getPointLight(index));
}

std::vector<DirectionalLight> & LightManager::getDirectionalLight(void)
{
	return (m_directionalLight);
}

std::vector<DirectionalLight> const & LightManager::getDirectionalLight(void) const
{
	return (m_directionalLight);
}

std::vector<PointLight> & LightManager::getPointLight(void)
{
	return (m_pointLight);
}

std::vector<PointLight> const & LightManager::getPointLight(void) const
{
	return (m_pointLight);
}

std::size_t LightManager::getDirectionalLightCount(void) const
{
	return (m_directionalLight.size());
}

std::size_t LightManager::getPointLightCount(void) const
{
	return (m_pointLight.size());
}
