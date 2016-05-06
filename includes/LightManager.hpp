#ifndef LIGHTMANAGER_HPP
# define LIGHTMANAGER_HPP

# include <vector>
# include <memory>
# include "DirectionalLight.hpp"

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

	DirectionalLight & createDirectionalLight(Color const & color, float ambientIntensity = 1.f, float diffuseIntensity = 1.f);
	DirectionalLight & getDirectionalLight(std::size_t index);
	std::size_t getDirectionalLightCount(void) const;

	void update(void);

private:
	std::vector<DirectionalLight> m_directionalLight;

	LightManager(LightManager const & lightManager);
	LightManager & operator=(LightManager const & lightManager);

};

#endif
