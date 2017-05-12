#ifndef MESH_HPP
# define MESH_HPP

# if defined(__APPLE__) || defined(MACOSX)
#  include <OpenGL/gl3.h>
# else
#  include <GL/glew.h>
# endif
# include <vector>
# include <memory>
# include <assimp/scene.h>
# include "Vertex.hpp"
# include "Material.hpp"

class Texture;
class Shader;
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

		void draw(Shader & shader) const;

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
	void draw(Shader & shader) const;

private:
	std::string								m_filename;
	std::vector<std::unique_ptr<MeshEntry>>	m_meshEntries;

	Mesh(Mesh const & mesh);
	Mesh & operator=(Mesh const & mesh);
};

#endif
