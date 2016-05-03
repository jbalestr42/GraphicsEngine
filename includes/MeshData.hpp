#ifndef MESHDATA_HPP
# define MESHDATA_HPP

# include <GL/glew.h>
# include <vector>
# include <memory>
# include <scene.h>
# include "Vertex.hpp"

class Texture;

class MeshData
{
	class MeshEntry
	{
	public:
		MeshEntry(void) = delete;
		MeshEntry(aiMesh * mesh);
		virtual ~MeshEntry(void);

		std::size_t getMaterialIndex(void) const;
		void draw(void) const;

	private:
		static const std::size_t IndexCount = 2;
		enum VBOIndex : std::size_t
		{
			VertexBuffer = 0,
			Index = 1
		};

		GLuint				m_vertexArrayObject;
		GLuint				m_vertexBufferObject[IndexCount];
		std::size_t			m_indiceCount;
		std::size_t			m_materialIndex;
		//TODO vector with materials per mesh entry

		MeshEntry(MeshEntry const & mesh);
		MeshEntry & operator=(MeshEntry const & mesh);

		void init(std::vector<Vertex> const & vertices, std::vector<GLuint> const & indices);
	};

public:
	MeshData(void) = delete;
	MeshData(std::string const & filename);
	virtual ~MeshData(void) = default;

	std::string const & getFilename(void) const;
	void draw(void) const;

private:
	std::string								m_filename;
	std::vector<std::unique_ptr<MeshEntry>>	m_meshEntries;
	std::vector<std::shared_ptr<Texture>>	m_textures;

	MeshData(MeshData const & mesh);
	MeshData & operator=(MeshData const & mesh);

	void initMaterials(aiScene const * scene, std::string const & filename);
};

#endif
