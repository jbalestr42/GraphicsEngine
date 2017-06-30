#ifndef LIGHTMANAGER_HPP
# define LIGHTMANAGER_HPP

# include <vector>
# include "DirectionalLight.hpp"
# include "PointLight.hpp"
# include "SpotLight.hpp"
# include "NonCopyable.hpp"

class RenderTarget;
class Camera;
#include "Shader.hpp"
#include "Model.hpp"
#include <map>
#include <list>

class RenderManager : public NonCopyable
{
public:
	RenderManager(void) = default;
	virtual ~RenderManager(void) = default;

	void draw(Model & model, Shader & shader);
	void display(RenderTarget & target, Camera const & camera);

	std::map<Shader *, std::list<Model const *>> m_drawables;

	DirectionalLight & createDirectionalLight(Color const & color);
	PointLight & createPointLight(Color const & color, Vector3 const & position);
	SpotLight & createSpotLight(Color const & color, Vector3 const & position);

	DirectionalLight & getDirectionalLight(std::size_t index);
	DirectionalLight const & getDirectionalLight(std::size_t index) const;
	PointLight & getPointLight(std::size_t index);
	PointLight const & getPointLight(std::size_t index) const;
	SpotLight & getSpotLight(std::size_t index);
	SpotLight const & getSpotLight(std::size_t index) const;

	std::vector<DirectionalLight> & getDirectionalLight(void);
	std::vector<DirectionalLight> const & getDirectionalLight(void) const;
	std::vector<PointLight> & getPointLight(void);
	std::vector<PointLight> const & getPointLight(void) const;
	std::vector<SpotLight> & getSpotLight(void);
	std::vector<SpotLight> const & getSpotLight(void) const;

	std::size_t getDirectionalLightCount(void) const;
	std::size_t getPointLightCount(void) const;
	std::size_t getSpotLightCount(void) const;

private:
	std::vector<DirectionalLight>	m_directionalLight;
	std::vector<PointLight>			m_pointLight;
	std::vector<SpotLight>			m_spotLight;

};

#endif
