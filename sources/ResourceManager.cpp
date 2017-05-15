#include "ResourceManager.hpp"
#include "Mesh.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include <iostream>

std::unique_ptr<ResourceManager> ResourceManager::m_instance = nullptr;

ResourceManager::ResourceManager(void)
{
	// Initialize texture library
	ilInit();

	// TODO use const for no_texture and black
	addShader(0, "resources/phong.frag", "resources/phong.vert");
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
	{
		it->second->setActive(true);
		return it->second;
	}
	std::cout << "Shader (" << index << ") not found." << std::endl;
	return (nullptr);
}

ResourceManager::ShaderMap & ResourceManager::getAllShaders(void)
{
	return (m_shaders);
}

std::shared_ptr<Shader> ResourceManager::addShader(int index, std::string const & fragment, std::string const & vertex)
{
	std::cout << "Add new shader (" << index << ") : " << fragment << " - " << vertex << std::endl;
	m_shaders[index] = std::make_shared<Shader>(fragment, vertex);
	return getShader(index);
}
