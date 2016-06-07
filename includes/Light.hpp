#ifndef LIGHT_HPP
# define LIGHT_HPP

# include "Transformable.hpp"
# include "Color.hpp"

class Light : public Transformable
{
public:
	Light(void);
	Light(Color const & color);
	Light(Light const & light);
	virtual ~Light(void) = default;

	Light & operator=(Light const & light);

	Color const & getColor(void) const;
	void setAmbientIntensity(float ambientIntensity);
	float getAmbientIntensity(void) const;
	//TODO isActive

private:
	Color		m_color;
	float		m_ambientIntensity;

};

#endif
