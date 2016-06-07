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

PointLight & LightManager::getPointLight(std::size_t index)
{
	assert(index < m_pointLight.size());
	return (m_pointLight[index]);
}

std::size_t LightManager::getDirectionalLightCount(void) const
{
	return (m_directionalLight.size());
}

std::size_t LightManager::getPointLightCount(void) const
{
	return (m_pointLight.size());
}

void LightManager::update(void)
{
	ResourceManager::ShaderMap shaders = ResourceManager::getInstance().getAllShaders();
	for (auto shader : shaders)
	{
		if (shader.second->isActive())
		{
			shader.second->setParameter("directional_light_count", m_directionalLight.size());
			shader.second->setParameter("directional_lights", m_directionalLight);
			shader.second->setParameter("point_light_count", m_pointLight.size());
			shader.second->setParameter("point_lights", m_pointLight);
		}
	}
}
