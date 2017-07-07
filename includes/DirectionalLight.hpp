#ifndef DIRECTIONNALLIGHT_HPP
# define DIRECTIONNALLIGHT_HPP

# include <vector>
# include "Light.hpp"
# include "ShadowMap.hpp"

class DirectionalLight : public Light
{
public:
	struct ShadowData
	{
		ShadowMap	shadowMap;
		Matrix		viewProj;
		float		endClipSpace;
	};

	static const std::size_t	MaxLight = 10u;

	DirectionalLight(void);
	DirectionalLight(Color const & color);
	DirectionalLight(DirectionalLight const & directionnalLight);
	virtual ~DirectionalLight(void) = default;

	DirectionalLight & operator=(DirectionalLight const & directionnalLight);

	Vector3 getRotatedDirection(void);
	Vector3 const & getDirection(void) const;

	std::vector<ShadowData> & getShadowData(void);

	virtual void computeShadowMap(Camera const & camera);

private:
	std::vector<ShadowData>	m_shadowData;
	Vector3					m_direction;

};

#endif
