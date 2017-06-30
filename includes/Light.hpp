#ifndef LIGHT_HPP
# define LIGHT_HPP

# include "Transformable.hpp"
# include "Color.hpp"

class Camera;
class Shader;

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

	void enableShadow(bool castShadow);
	bool isShadowEnabled(void) const;

	virtual void computeShadowMap(Camera const & camera) = 0;
	virtual void bindShadowMap(Shader & shader);

private:
	Color		m_color;
	float		m_ambientIntensity;
	bool		m_castShadow;

};

#endif
