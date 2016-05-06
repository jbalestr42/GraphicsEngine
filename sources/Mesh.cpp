#include "Mesh.hpp"
#include "ResourceManager.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "IView.hpp"
#include "Matrix.hpp"
#include <iostream>
#include <cassert>
#include <postprocess.h>
#include <Importer.hpp>

Mesh::Mesh(std::string const & filename) :
	m_filename(filename)
{
	Assimp::Importer importer;
	const aiScene * scene = importer.ReadFile(filename.c_str(),
			aiProcess_Triangulate |
			aiProcess_GenSmoothNormals |
			aiProcess_GenUVCoords |
			aiProcess_OptimizeMeshes |
			aiProcess_FlipUVs |
			aiProcess_JoinIdenticalVertices |
			aiProcess_CalcTangentSpace |
			aiProcess_SortByPType |
			aiProcess_ImproveCacheLocality |
			aiProcess_RemoveRedundantMaterials |
			aiProcess_FindInvalidData |
			aiProcess_TransformUVCoords |
			aiProcess_FindInstances |
			aiProcess_LimitBoneWeights |
			aiProcess_SplitByBoneCount |
			aiProcess_FixInfacingNormals);
	if (!scene)
		std::cout << "Importer error : " << importer.GetErrorString() << std::endl;
	else
	{
		// Extract directory path
		std::string::size_type slashIndex = filename.find_last_of("/");
		std::string dirPath;
		std::string fullPath;

		if (slashIndex == std::string::npos)
			dirPath = ".";
		else if (slashIndex == 0)
			dirPath = "/";
		else
			dirPath = filename.substr(0, slashIndex);

		for (std::size_t i = 0; i < scene->mNumMeshes; i++)
			m_meshEntries.emplace_back(new MeshEntry(scene, scene->mMeshes[i], dirPath));
	}
	importer.FreeScene();
}

std::string const & Mesh::getFilename(void) const
{
	return (m_filename);
}

void Mesh::draw(IView const & view, Matrix const & transform) const
{
	for (auto & mesh : m_meshEntries)
		mesh->draw(view, transform);
}

Mesh::MeshEntry::MeshEntry(aiScene const * scene, aiMesh const * mesh, std::string const & dirPath) :
	m_indiceCount(0u)
{
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	for (std::size_t i = 0; i < mesh->mNumVertices; i++)
	{
		const aiVector3D * pos = &(mesh->mVertices[i]);
		const aiVector3D * normal = &(mesh->mNormals[i]);
		const aiVector3D * texCoord = mesh->HasTextureCoords(0) ? &(mesh->mTextureCoords[0][i]) : &Zero3D;
		vertices.emplace_back(Vector3(pos->x, pos->y, pos->z),
				Vector2(texCoord->x, texCoord->y),
				Vector3(normal->x, normal->y, normal->z).normalize(),
				Color());
	}

	for (std::size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace const & face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	m_indiceCount = mesh->mNumFaces * 3;
	initMaterial(scene, mesh->mMaterialIndex, dirPath);
	init(vertices, indices);
}

Mesh::MeshEntry::~MeshEntry(void)
{
	glDeleteBuffers(IndexCount, m_vertexBufferObject);
	glDeleteVertexArrays(1, &m_vertexArrayObject);
}

void Mesh::MeshEntry::init(std::vector<Vertex> const & vertices, std::vector<GLuint> const & indices)
{
	glGenVertexArrays(1, &m_vertexArrayObject);
	glBindVertexArray(m_vertexArrayObject);
	glGenBuffers(IndexCount, m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject[VBOIndex::VertexBuffer]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	//TODO maybe remove color from attribpointer
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

int Mesh::MeshEntry::getTexture(aiMaterial const * material, aiTextureType textureType, std::string const & dirPath, std::string & fullPath)
{
	if (material->GetTextureCount(textureType) > 0)
	{
		//TODO get all textures and store them per MeshEntry ?
		aiString path;
		if (material->GetTexture(textureType, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			fullPath = dirPath + "/" + path.data;
			return (1);
		}
		else // TODO load error texture
			std::cout << "Error while loading texture." << std::endl;
	}
	return (0);
}

void Mesh::MeshEntry::initMaterial(aiScene const * scene, std::size_t materialIndex, std::string const & dirPath)
{
	// Initialize materials
	assert(materialIndex < scene->mNumMaterials);
	std::string fullPath;
	aiMaterial const * material = scene->mMaterials[materialIndex];
	if (getTexture(material, aiTextureType_DIFFUSE, dirPath, fullPath))
		m_material.diffuseTexture = ResourceManager::getInstance().getTexture(fullPath);

	aiString name;
	material->Get(AI_MATKEY_NAME, name); //TODO add in resource manager
	int shadingModel;
	material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
		m_shader = ResourceManager::getInstance().getShader(shadingModel);
	aiColor3D c;
	if (material->Get(AI_MATKEY_COLOR_AMBIENT, c))
		m_material.ka = Color(c.r, c.g, c.b);
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, c))
		m_material.kd = Color(c.r, c.g, c.b);
	if (material->Get(AI_MATKEY_COLOR_SPECULAR, c))
		m_material.ks = Color(c.r, c.g, c.b);
}

void Mesh::MeshEntry::draw(IView const & view, Matrix const & transform) const
{
	m_shader->setParameter("ProjectionMatrix", view.getProjectionMatrix());
	m_shader->setParameter("ViewMatrix", view.getViewMatrix());
	m_shader->setParameter("ModelMatrix", transform);
	m_shader->setParameter("camera_position", view.getPosition());
	if (m_material.diffuseTexture)
		m_material.diffuseTexture->bind(GL_TEXTURE0, GL_TEXTURE_2D);
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);
}
