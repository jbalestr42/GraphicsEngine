#ifndef RESOURCEMANAGER_HPP
# define RESOURCEMANAGER_HPP

# include <map>
# include <memory>

class Mesh;
class Texture;
class Shader;

class ResourceManager
{
public:
	typedef std::map<std::string, std::shared_ptr<Mesh>>	MeshMap;
	typedef std::map<std::string, std::shared_ptr<Texture>>	TextureMap;
	typedef std::map<int, std::shared_ptr<Shader>>			ShaderMap;

	virtual ~ResourceManager(void) = default;

	static ResourceManager & getInstance(void);
	std::shared_ptr<Mesh> getMesh(std::string const & name);
	std::shared_ptr<Texture> getTexture(std::string const & name);
	std::shared_ptr<Shader> getShader(int index);
	ShaderMap & getAllShaders(void);

	void addShader(int index, std::string const & fragment, std::string const & vertex);

private:
	MeshMap		m_meshes;
	TextureMap	m_textures;
	ShaderMap	m_shaders;

	static std::unique_ptr<ResourceManager>	m_instance;

	ResourceManager(void);
	ResourceManager(ResourceManager const & meshManager);
	ResourceManager & operator=(ResourceManager const & meshManager);

};

#endif
