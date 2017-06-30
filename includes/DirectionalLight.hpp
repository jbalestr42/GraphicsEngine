#ifndef DIRECTIONNALLIGHT_HPP
# define DIRECTIONNALLIGHT_HPP

# include "Light.hpp"
# include "ShadowMap.hpp"

class DirectionalLight : public Light
{
public:
	static const std::size_t	MaxLight = 10u;

	DirectionalLight(void);
	DirectionalLight(Color const & color);
	DirectionalLight(DirectionalLight const & directionnalLight);
	virtual ~DirectionalLight(void) = default;

	DirectionalLight & operator=(DirectionalLight const & directionnalLight);

	Vector3 getRotatedDirection(void);
	Vector3 const & getDirection(void) const;

	//TODO rename for writing/reading
	virtual void computeShadowMap(Camera const & camera);
	virtual void bindShadowMap(Shader & shader);

private:
	Matrix		m_viewProj;
	ShadowMap	m_shadowMap;
	Vector3		m_direction;

};

#endif
