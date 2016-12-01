#include "Model.hpp"
#include "ResourceManager.hpp"
#include "Mesh.hpp"
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

Model & Model::operator=(Model const & model)
{
	Transformable::operator=(model);
	m_mesh = model.m_mesh;
	return (*this);
}

Model & Model::operator=(Model && model)
{
	Transformable::operator=(std::move(model));
	m_mesh = std::move(model.m_mesh);
	return (*this);
}

void Model::loadModel(std::string const & filename)
{
	m_mesh = ResourceManager::getInstance().getMesh(filename);
}

// TODO check if we can make a mutable matrix to have getMatrix const
void Model::draw(Shader & shader)
{
	shader.setParameter("ModelMatrix", getMatrix());
	if (m_mesh)
		m_mesh->draw(shader);
}
