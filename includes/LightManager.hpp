#ifndef LIGHTMANAGER_HPP
# define LIGHTMANAGER_HPP

# include <vector>
# include <memory>
# include "DirectionalLight.hpp"
# include "PointLight.hpp"

class LightManager
{
public:
	enum LightType
	{
		Directional,
		Point,
		Spot
	};

	LightManager(void) = default;
	virtual ~LightManager(void) = default;

	DirectionalLight & createDirectionalLight(Color const & color);
	PointLight & createPointLight(Color const & color, Vector3 const & position);

	DirectionalLight & getDirectionalLight(std::size_t index);
	PointLight & getPointLight(std::size_t index);

	std::size_t getDirectionalLightCount(void) const;
	std::size_t getPointLightCount(void) const;

	void update(void);

private:
	std::vector<DirectionalLight> m_directionalLight;
	std::vector<PointLight> m_pointLight;

	LightManager(LightManager const & lightManager);
	LightManager & operator=(LightManager const & lightManager);

};

#endif
