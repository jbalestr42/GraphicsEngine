#ifndef MATERIAL_HPP
# define MATERIAL_HPP

# include <memory>
# include "Color.hpp"

class Texture;

class Material
{
public:
	Material(void);
	Material(Material const & material);
	virtual ~Material(void) = default;

	Material & operator=(Material const & material);

	Color						ka;
	Color						kd;
	Color						ks;
	float						shininess;
	std::shared_ptr<Texture>	diffuseTexture;
	std::shared_ptr<Texture>	specularTexture;

};

#endif
