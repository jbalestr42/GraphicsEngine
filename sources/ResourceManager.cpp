#include "ResourceManager.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"

std::unique_ptr<ResourceManager> ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager(void)
{
	ilInit();

	//TODO all other shaders
	// aiShadingMode_Flat
	//	Flat shading. Shading is done on per-face base, diffuse only. Also known as 'faceted shading'.

	// aiShadingMode_Gouraud
	//	Simple Gouraud shading.
	addShader(1, "resources/phong.frag", "resources/phong.vert");

	// aiShadingMode_Phong
	//	Phong-Shading.
	addShader(2, "resources/phong.frag", "resources/phong.vert");

	// aiShadingMode_Blinn
	//	Phong-Blinn-Shading.

	// aiShadingMode_Toon
	//	Toon-Shading per pixel. Also known as 'comic' shader.

	// aiShadingMode_OrenNayar
	//	OrenNayar-Shading per pixel. Extension to standard Lambertian shading, taking the roughness of the material into account

	// aiShadingMode_Minnaert
	//	Minnaert-Shading per pixel. Extension to standard Lambertian shading, taking the "darkness" of the material into account

	// aiShadingMode_CookTorrance
	//	CookTorrance-Shading per pixel. Special shader for metallic surfaces.

	// aiShadingMode_NoShading
	//	No shading at all. Constant light influence of 1.0.

	// aiShadingMode_Fresnel
	//	Fresnel shading.
}

ResourceManager::ResourceManager(ResourceManager const & meshManager)
{
	*this = meshManager;
}

ResourceManager::~ResourceManager(void) { }

ResourceManager & ResourceManager::operator=(ResourceManager const & meshManager)
{
	(void)meshManager;
	return (*this);
}

ResourceManager & ResourceManager::getInstance(void)
{
	if (m_instance == nullptr)
		m_instance.reset(new ResourceManager());
	return *m_instance;
}

std::shared_ptr<Mesh> ResourceManager::getMesh(std::string const & name)
{
	auto it = m_meshes.find(name);
	if (it != m_meshes.end())
		return it->second;
	std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>(name);
	m_meshes[name] = mesh;
	std::cout << "Add new mesh : " << name << std::endl;
	return (mesh);
}

std::shared_ptr<Texture> ResourceManager::getTexture(std::string const & name)
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
		return it->second;
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(name);
	m_textures[name] = texture;
	std::cout << "Add new texture : " << name << std::endl;
	return (texture);
}

std::shared_ptr<Shader> ResourceManager::getShader(int index)
{
	auto it = m_shaders.find(index);
	if (it != m_shaders.end())
		return it->second;
	return (nullptr);
}

void ResourceManager::addShader(int index, std::string const & fragment, std::string const & vertex)
{
	std::cout << "Add new shader : " << fragment << " - " << vertex << std::endl;
	m_shaders[index] = std::make_shared<Shader>(fragment, vertex);
}
