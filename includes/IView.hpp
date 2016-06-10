#ifndef IVIEW_HPP
# define IVIEW_HPP

# include "ResourceManager.hpp"
# include "Shader.hpp"

class Vector3;
class Matrix;

class IView
{
public:
	virtual Matrix const & getProjectionMatrix(void) const = 0;
	virtual Matrix const & getViewMatrix(void) const = 0;
	virtual Vector3 const & getPosition(void) const = 0;

	virtual void setViewProjectionMatrices(void)
	{
		ResourceManager::ShaderMap shaders = ResourceManager::getInstance().getAllShaders();
		for (auto shader : shaders)
		{
			if (shader.second->isActive())
			{
				shader.second->setParameter("ProjectionMatrix", getProjectionMatrix());
				shader.second->setParameter("ViewMatrix", getViewMatrix());
				shader.second->setParameter("view_position", getPosition());
			}
		}
	}

};

#endif
