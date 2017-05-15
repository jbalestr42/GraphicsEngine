#ifndef DIRECTIONNALLIGHT_HPP
# define DIRECTIONNALLIGHT_HPP

# include "Light.hpp"
# include <cstddef>

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

private:
	Vector3		m_direction;

};

#endif
