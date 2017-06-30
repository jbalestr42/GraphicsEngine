#include "RenderManager.hpp"
#include "Shader.hpp"
#include "RenderTarget.hpp"
#include "Camera.hpp"
#include <cassert>

void RenderManager::draw(Model & model, Shader & shader)
{
	m_drawables[&shader].push_back(&model);
}

void RenderManager::display(RenderTarget & target, Camera const & camera)
{
	std::shared_ptr<Shader> depth = ResourceManager::getInstance().getShader(ShaderId::Depth);
	for (auto & light : m_directionalLight)
	{
		if (light.isShadowEnabled())
		{
			// Compute and bind the shadow map
			light.computeShadowMap(camera);
			for (auto & list : m_drawables)
			{
				for (auto & model : list.second)
				{
					// TODO check AABB collision between ligth view and model
					// TODO if (drawable.castShadow)
					model->draw(*depth);
				}
			}
		}
	}

	target.setViewport();
	target.bind();
	target.clear();

	for (auto & list : m_drawables)
	{
		list.first->setParameter("ProjectionMatrix", camera.getProjectionMatrix());
		list.first->setParameter("ViewMatrix", camera.getViewMatrix());
		list.first->setParameter("view_position", camera.getPosition());

		list.first->setParameter("directional_light_count", getDirectionalLightCount());
		list.first->setParameter("directional_lights", getDirectionalLight());
		list.first->setParameter("point_light_count", getPointLightCount());
		list.first->setParameter("point_lights", getPointLight());
		list.first->setParameter("spot_light_count", getSpotLightCount());
		list.first->setParameter("spot_lights", getSpotLight());

		for (auto & model : list.second)
		{
			model->draw(*list.first);
		}
	}

	m_drawables.clear();
}

DirectionalLight & RenderManager::createDirectionalLight(Color const & color)
{
	m_directionalLight.emplace_back(color);
	return m_directionalLight.back();
}

PointLight & RenderManager::createPointLight(Color const & color, Vector3 const & position)
{
	m_pointLight.emplace_back(color, position);
	return m_pointLight.back();
}

SpotLight & RenderManager::createSpotLight(Color const & color, Vector3 const & position)
{
	m_spotLight.emplace_back(color, position);
	return m_spotLight.back();
}

DirectionalLight & RenderManager::getDirectionalLight(std::size_t index)
{
	return const_cast<DirectionalLight &>(static_cast<const RenderManager &>(*this).getDirectionalLight(index));
}

DirectionalLight const & RenderManager::getDirectionalLight(std::size_t index) const
{
	assert(index < m_directionalLight.size());
	return m_directionalLight[index];
}

PointLight & RenderManager::getPointLight(std::size_t index)
{
	return const_cast<PointLight &>(static_cast<const RenderManager &>(*this).getPointLight(index));
}

PointLight const & RenderManager::getPointLight(std::size_t index) const
{
	assert(index < m_pointLight.size());
	return m_pointLight[index];
}

SpotLight & RenderManager::getSpotLight(std::size_t index)
{
	return const_cast<SpotLight &>(static_cast<const RenderManager &>(*this).getSpotLight(index));
}

SpotLight const & RenderManager::getSpotLight(std::size_t index) const
{
	assert(index < m_spotLight.size());
	return m_spotLight[index];
}

std::vector<DirectionalLight> & RenderManager::getDirectionalLight(void)
{
	return m_directionalLight;
}

std::vector<DirectionalLight> const & RenderManager::getDirectionalLight(void) const
{
	return m_directionalLight;
}

std::vector<PointLight> & RenderManager::getPointLight(void)
{
	return m_pointLight;
}

std::vector<PointLight> const & RenderManager::getPointLight(void) const
{
	return m_pointLight;
}

std::vector<SpotLight> & RenderManager::getSpotLight(void)
{
	return m_spotLight;
}

std::vector<SpotLight> const & RenderManager::getSpotLight(void) const
{
	return m_spotLight;
}

std::size_t RenderManager::getDirectionalLightCount(void) const
{
	return m_directionalLight.size();
}

std::size_t RenderManager::getPointLightCount(void) const
{
	return m_pointLight.size();
}

std::size_t RenderManager::getSpotLightCount(void) const
{
	return m_spotLight.size();
}
