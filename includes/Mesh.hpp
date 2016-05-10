#ifndef MESH_HPP
# define MESH_HPP

# include <GL/glew.h>
# include <vector>
# include <memory>
# include <scene.h>
# include "Vertex.hpp"
# include "Material.hpp" // TODO put MAterial in Resource Manager and use shared_ptr

class Texture;
class Shader;
class IView;
class Matrix;

class Mesh
{
	class MeshEntry
	{
	public:
		MeshEntry(void) = delete;
		MeshEntry(aiScene const * scene, aiMesh const * mesh, std::string const & dirPath);
		virtual ~MeshEntry(void);

		Material const & getMaterial(void) const;

		void draw(IView const & view, Matrix const & transform) const;

	private:
		static const std::size_t IndexCount = 2;
		enum VBOIndex : std::size_t
		{
			VertexBuffer = 0,
			Index = 1
		};

		GLuint					m_vertexArrayObject;
		GLuint					m_vertexBufferObject[IndexCount];
		std::size_t				m_indiceCount;
		Material				m_material;
		std::shared_ptr<Shader>	m_shader;

		MeshEntry(MeshEntry const & mesh);
		MeshEntry & operator=(MeshEntry const & mesh);

		void init(std::vector<Vertex> const & vertices, std::vector<GLuint> const & indices);
		int getTexture(aiMaterial const * material, aiTextureType textureType, std::string const & dirPath, std::string & fullPath);
		void initMaterial(aiScene const * scene, std::size_t materialIndex, std::string const & filename);
	};

public:
	Mesh(void) = delete;
	Mesh(std::string const & filename);
	virtual ~Mesh(void) = default;

	std::string const & getFilename(void) const;
	void draw(IView const & view, Matrix const & transform) const;

private:
	std::string								m_filename;
	std::vector<std::unique_ptr<MeshEntry>>	m_meshEntries;

	Mesh(Mesh const & mesh);
	Mesh & operator=(Mesh const & mesh);
};

#endif
