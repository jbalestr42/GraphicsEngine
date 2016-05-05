#ifndef MODEL_HPP
# define MODEL_HPP

# include <memory>
# include "Transformable.hpp"

class Mesh;
class Shader;

class Model : public Transformable
{
public:
	Model(void);
	Model(std::string const & filename);
	Model(Model const & mesh);
	Model(Model && mesh);
	virtual ~Model(void) = default;

	Model & operator=(Model const & mesh);
	Model & operator=(Model && mesh);

	void draw(Shader & shader);
	void loadModel(std::string const & filename);

private:
	std::shared_ptr<Mesh>	m_mesh;

};

#endif
