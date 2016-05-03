#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"

Mesh::Mesh(void) :
	Transformable(),
	m_meshData(nullptr)
{ }

Mesh::Mesh(std::string const & filename) :
	Transformable(),
	m_meshData(nullptr)
{
	loadMesh(filename);
}

Mesh::Mesh(Mesh const & mesh) :
	Transformable(mesh)
{
	*this = mesh;
}

Mesh::Mesh(Mesh && mesh) :
	Transformable(std::move(mesh))
{
	*this = std::move(mesh);
}

Mesh & Mesh::operator=(Mesh const & mesh)
{
	Transformable::operator=(mesh);
	m_meshData = mesh.m_meshData;
	return (*this);
}

Mesh & Mesh::operator=(Mesh && mesh)
{
	Transformable::operator=(std::move(mesh));
	m_meshData = std::move(mesh.m_meshData);
	return (*this);
}

void Mesh::loadMesh(std::string const & filename)
{
	m_meshData = ResourceManager::getInstance().getMeshData(filename);
}

void Mesh::draw(Shader & shader)
{
	shader.bind();
	shader.setParameter("ModelMatrix", getMatrix());
	if (m_meshData)
		m_meshData->draw();
}
