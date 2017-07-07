#include "DirectionalLight.hpp"
#include "Vector4.hpp"
#include "Math.hpp"
#include "Camera.hpp"
#include "DebugDraw.hpp"
#include <vector>

DirectionalLight::DirectionalLight(void) :
	DirectionalLight(Color())
{}

DirectionalLight::DirectionalLight(Color const & color) :
	Light(color),
	m_direction({0.f, 0.f, -1.f})
{
	float cascadeCount = 3u;
	m_shadowData.resize(cascadeCount);
	for (std::size_t i = 0u; i < cascadeCount; i++)
		m_shadowData[i].shadowMap.init();
}

DirectionalLight::DirectionalLight(DirectionalLight const & directionnalLight) :
	Light(directionnalLight)
{
	*this = directionnalLight;
}

DirectionalLight & DirectionalLight::operator=(DirectionalLight const & directionnalLight)
{
	Light::operator=(directionnalLight);
	//TODO copy the shadow data
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

std::vector<DirectionalLight::ShadowData> & DirectionalLight::getShadowData(void)
{
	return m_shadowData;
}

// TODO: to improve we should consider what is between the camera frustum and the light
// mesh behind the camera are not used
void DirectionalLight::computeShadowMap(Camera const & camera)
{
	Matrix camInv = camera.getViewMatrix().inverse();
	Matrix lightView = Matrix::lookAt({0.f, 0.f, 0.f}, getRotatedDirection().normalize(), {0.f, 1.f, 0.f});

	float ar = static_cast<float>(camera.getWidth()) / static_cast<float>(camera.getHeight());
	float near = camera.getNearPlane();
	float far = camera.getFarPlane();
	float halfHeight = std::tan(Deg2Rad * (camera.getFov() / 2.f));
	float halfWidth = halfHeight * ar;

	m_shadowData[0].endClipSpace = near + (far - near) * 0.25f;
	m_shadowData[1].endClipSpace = near + (far - near) * 0.75f;
	m_shadowData[2].endClipSpace = near + (far - near) * 1.0f;

	for (std::size_t i = 0u; i < m_shadowData.size(); i++)
	{
		float xf = halfWidth * m_shadowData[i].endClipSpace;
		float yf = halfHeight * m_shadowData[i].endClipSpace;

		Vector4 farthestCorner = Vector4(xf, yf, -m_shadowData[i].endClipSpace, 1.f);

		float n = i == 0 ? near : m_shadowData[i - 1u].endClipSpace;
		// Frustum center in camera space
		Vector4 fc = Vector4(0.f, 0.f, -(n + (m_shadowData[i].endClipSpace - n) / 2.f), 1.f);
		// Frustum center in world space
		Vector4 fcW = camInv * fc;

		// Distance from center to farthest corner used to compute the ortho projection
		float dist = (farthestCorner - fc).length();
		Matrix lightProj = Matrix::orthographicProjection(fcW.x - dist, fcW.x + dist, fcW.y - dist, fcW.y + dist, -fcW.z - dist, -fcW.z + dist);
		m_shadowData[i].viewProj = lightProj * lightView;

		//DebugDraw::getInstance().drawOrthogonalProjection(m_shadowData[i].viewProj);
	}
}
