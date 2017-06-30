#include "DirectionalLight.hpp"
#include "Vector4.hpp"
#include "Math.hpp"
#include "Camera.hpp"

DirectionalLight::DirectionalLight(void) :
	DirectionalLight(Color())
{}

#include <iostream>
DirectionalLight::DirectionalLight(Color const & color) :
	Light(color),
	m_direction({0.f, 0.f, 1.f})
{
	m_shadowMap.init();
}

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

void DirectionalLight::computeShadowMap(Camera const & camera)
{
	Matrix camInv = camera.getViewMatrix().inverse();
		// TODO change the directionnal light direction and the relative stuff in shaders
	Matrix lightView = Matrix::lookAt({0.f, 0.f, 0.f}, -getRotatedDirection().normalize(), {0.f, 1.f, 0.f});

	// TODO clean
	// Retrieve data from IView
	float ar = static_cast<float>(camera.getWidth()) / static_cast<float>(camera.getHeight());
	float fov = 60.f;
	float near = 0.1f;
	float far = 100.f;
	float cascadeEnd[2] = { near, far };
	std::size_t cascadeCount = 1;

	float halfHeight = std::tan(Deg2Rad * (fov / 2.f));
	float halfWidth = halfHeight * ar;

	for (std::size_t i = 0u; i < cascadeCount; i++)
	{
		float xf = halfWidth * cascadeEnd[i + 1];
		float yf = halfHeight * cascadeEnd[i + 1];

		Vector4 farthestCorner = Vector4(xf, yf, -far, 1.f);

		// Frustum center in camera space
		Vector4 fc = Vector4(0.f, 0.f, -(near + far) / 2.f, 1.f);
		// Frustum center in world space
		Vector4 fcW = camInv * fc;

		// Distance from center to farthest corner used to compute the ortho projection
		float dist = (farthestCorner - fc).length();
		Matrix lightProj = Matrix::orthographicProjection(fcW.x - dist, fcW.x + dist, fcW.y - dist, fcW.y + dist, -fcW.z - dist, -fcW.z + dist);
		m_viewProj = lightProj * lightView;
	}

	// Draw the shadowmap in a new texture
	m_shadowMap.setViewport();
	m_shadowMap.bind();
	m_shadowMap.clear();

	ResourceManager::getInstance().getShader(ShaderId::Depth)->setParameter("LightViewProjMatrix", m_viewProj);
}

void DirectionalLight::bindShadowMap(Shader & shader)
{
	// TODO tableau de viewProj et de shadow map dans le shader pour gérer le CSM
	// for (int i = 0; i < light.getCSMCount(); i++)
	// Need global value for multiple light with shadows
	shader.setParameter("LightViewProjMatrix", m_viewProj);
	shader.setParameter("shadow_map", ShadowMapIndex);
	m_shadowMap.bindTexture(/*TODO use the good index*/);
}
