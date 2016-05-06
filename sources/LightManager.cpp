#include "LightManager.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"
#include <cassert>

DirectionalLight & LightManager::createDirectionalLight(Color const & color, float ambientIntensity, float diffuseIntensity)
{
	m_directionalLight.emplace_back(color, ambientIntensity, diffuseIntensity);
	return (m_directionalLight.back());
}

DirectionalLight & LightManager::getDirectionalLight(std::size_t index)
{
	assert(index < m_directionalLight.size());
	return (m_directionalLight[index]);
}

std::size_t LightManager::getDirectionalLightCount(void) const
{
	return (m_directionalLight.size());
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
		}
	}
}
