#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Shader.hpp"

Model::Model(void) :
	Transformable(),
	m_mesh(nullptr)
{ }

Model::Model(std::string const & filename) :
	Transformable(),
	m_mesh(nullptr)
{
	loadModel(filename);
}

Model::Model(Model const & mesh) :
	Transformable(mesh)
{
	*this = mesh;
}

Model::Model(Model && mesh) :
	Transformable(std::move(mesh))
{
	*this = std::move(mesh);
}

Model & Model::operator=(Model const & mesh)
{
	Transformable::operator=(mesh);
	m_mesh = mesh.m_mesh;
	return (*this);
}

Model & Model::operator=(Model && mesh)
{
	Transformable::operator=(std::move(mesh));
	m_mesh = std::move(mesh.m_mesh);
	return (*this);
}

void Model::loadModel(std::string const & filename)
{
	m_mesh = ResourceManager::getInstance().getMesh(filename);
}

void Model::draw(Shader & shader)
{
	shader.bind();
	shader.setParameter("ModelMatrix", getMatrix());
	if (m_mesh)
		m_mesh->draw();
}
