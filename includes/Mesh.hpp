#ifndef MESH_HPP
# define MESH_HPP

# include <memory>
# include "Transformable.hpp"

class MeshData;
class Shader;

class Mesh : public Transformable
{
public:
	Mesh(void);
	Mesh(std::string const & filename);
	Mesh(Mesh const & mesh);
	Mesh(Mesh && mesh);
	virtual ~Mesh(void) = default;

	Mesh & operator=(Mesh const & mesh);
	Mesh & operator=(Mesh && mesh);

	void draw(Shader & shader);
	void loadMesh(std::string const & filename);

private:
	std::shared_ptr<MeshData>	m_meshData;

};

#endif
