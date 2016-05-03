#include "MeshData.hpp"
#include "ResourceManager.hpp"
#include <iostream>
#include <cassert>
#include <postprocess.h>
#include <Importer.hpp>

MeshData::MeshData(std::string const & filename) :
	m_filename(filename)
{
	Assimp::Importer Importer;
	const aiScene * pScene = Importer.ReadFile(filename.c_str(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_JoinIdenticalVertices |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes);
	if (!pScene)
		std::cout << "error pscene" << std::endl;
	else
	{
		for (std::size_t i = 0; i < pScene->mNumMeshes; i++)
			m_meshEntries.emplace_back(new MeshEntry(pScene->mMeshes[i]));
		//TODO init all materials
		initMaterials(pScene, filename);
	}
	Importer.FreeScene();
}

//void MeshData::loadMaterialTextures(aiMaterial * material, aiTextureType type)
//{
//
//}

void MeshData::initMaterials(aiScene const * scene, std::string const & filename)
{
	// Extract the directory part from the file name
	std::string::size_type slashIndex = filename.find_last_of("/");
	std::string dirPath;
	std::string fullPath;

	if (slashIndex == std::string::npos)
		dirPath = ".";
	else if (slashIndex == 0)
		dirPath = "/";
	else
		dirPath = filename.substr(0, slashIndex);

	// Initialize materials
	for (std::size_t i = 0u; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = scene->mMaterials[i];

		if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			//TODO get all textures and store them per MeshEntry ?
			aiString path;
			if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
			{
				std::string fullPath = dirPath + "/" + path.data;
				m_textures.push_back(ResourceManager::getInstance().getTexture(fullPath));
			}
			else
				std::cout << "Error while loading texture : " << fullPath << std::endl;
		}
		else
			m_textures.push_back(ResourceManager::getInstance().getTexture("resources/nyan.bmp")); // TODO load white texture instead
	}
}

std::string const & MeshData::getFilename(void) const
{
	return (m_filename);
}

void MeshData::draw(void) const
{
	//shader.setParameter("ModelMatrix", );
	for (auto & mesh : m_meshEntries)
	{
		//TODO move if condition in meshentry draw
		if (mesh->getMaterialIndex() < m_textures.size() && mesh->getMaterialIndex())
			m_textures[mesh->getMaterialIndex()]->bind(GL_TEXTURE0, GL_TEXTURE_2D);
		mesh->draw();
	}
}

MeshData::MeshEntry::MeshEntry(aiMesh * mesh) :
	m_indiceCount(0u),
	m_materialIndex(0u)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (std::size_t i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D * pPos = &(mesh->mVertices[i]);
		const aiVector3D * pNormal = &(mesh->mNormals[i]);
		const aiVector3D * pTexCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
		vertices.emplace_back(Vector3(pPos->x, pPos->y, pPos->z),
				Vector2(pTexCoord->x, pTexCoord->y),
				Vector3(pNormal->x, pNormal->y, pNormal->z),
				Color());
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace& Face = mesh->mFaces[i];
		assert(Face.mNumIndices == 3);
		indices.push_back(Face.mIndices[0]);
		indices.push_back(Face.mIndices[1]);
		indices.push_back(Face.mIndices[2]);
	}

	m_indiceCount = mesh->mNumFaces * 3;
	m_materialIndex = mesh->mMaterialIndex;
	init(vertices, indices);
}

MeshData::MeshEntry::~MeshEntry(void)
{
	glDeleteBuffers(IndexCount, m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

std::size_t MeshData::MeshEntry::getMaterialIndex(void) const
{
	return (m_materialIndex);
}

void MeshData::MeshEntry::init(std::vector<Vertex> const & vertices, std::vector<GLuint> const & indices)
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(IndexCount, m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[VBOIndex::VertexBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//TODO found a way to use attriblocation from shader, maybe each frame ?
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_vertexBufferObject[VBOIndex::Index]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR)
		std::cout << "mesh " << error << std::endl;
	// Add Gl error check TODO
	glBindVertexArray(0);
}

void MeshData::MeshEntry::draw(void) const
{
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);
}


