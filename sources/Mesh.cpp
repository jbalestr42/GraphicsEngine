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

	int posAtt = m_shader->getAttribute(Shader::Attribute::PositionAtt);
	if (posAtt >= 0)
	{
		glEnableVertexAttribArray(posAtt);
		glVertexAttribPointer(posAtt, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	}

	int texAtt = m_shader->getAttribute(Shader::Attribute::TexCoordAtt);
	if (texAtt >= 0)
	{
		glEnableVertexAttribArray(texAtt);
		glVertexAttribPointer(texAtt, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3)));
	}

	int normalAtt = m_shader->getAttribute(Shader::Attribute::NormalAtt);
	if (normalAtt >= 0)
	{
		glEnableVertexAttribArray(normalAtt);
		glVertexAttribPointer(normalAtt, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2)));
	}

	int colAtt = m_shader->getAttribute(Shader::Attribute::ColorAtt);
	if (colAtt >= 0)
	{
		glEnableVertexAttribArray(colAtt);
		glVertexAttribPointer(colAtt, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(sizeof(Vector3) + sizeof(Vector2) + sizeof(Vector3)));
	}

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
		//TODO get all textures
		aiString path;
		if (material->GetTexture(textureType, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			fullPath = dirPath + "/" + path.data;
			return (1);
		}
		else
		{
			std::cout << "Error while loading texture : Error texture used instead." << std::endl;
			fullPath = dirPath + "/" + "error.jpg";
			return (1);
		}
	}
	return (0);
}

void Mesh::MeshEntry::initMaterial(aiScene const * scene, std::size_t materialIndex, std::string const & dirPath)
{
	assert(materialIndex < scene->mNumMaterials);

	std::string fullPath;
	aiMaterial const * material = scene->mMaterials[materialIndex];
	if (getTexture(material, aiTextureType_DIFFUSE, dirPath, fullPath))
		m_material.diffuseTexture = ResourceManager::getInstance().getTexture(fullPath);
	if (getTexture(material, aiTextureType_SPECULAR, dirPath, fullPath))
		m_material.specularTexture = ResourceManager::getInstance().getTexture(fullPath);

	int shadingModel;
	material->Get(AI_MATKEY_SHADING_MODEL, shadingModel);
	// TODO add check to avoid index out of bound
	//m_shader = ResourceManager::getInstance().getShader(shadingModel);
	m_shader = ResourceManager::getInstance().getShader(2);

	aiString name;
	material->Get(AI_MATKEY_NAME, name); //TODO add in resource manager

	aiColor3D c(0.f, 0.f, 0.f);
	if (material->Get(AI_MATKEY_COLOR_AMBIENT, c) != -1)
		m_material.ka = Color(c.r, c.g, c.b);
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, c) != -1)
		m_material.kd = Color(c.r, c.g, c.b);
	if (material->Get(AI_MATKEY_COLOR_SPECULAR, c) != -1)
		m_material.ks = Color(c.r, c.g, c.b);
	float shininess = 0.f;
	if (material->Get(AI_MATKEY_SHININESS, shininess) != -1 && shininess > 0.f)
		m_material.shininess = shininess;
	m_material.kd.a = m_material.diffuseTexture ? 1.f : 0.f;
	m_material.ks.a = m_material.specularTexture ? 1.f : 0.f;
}

void Mesh::MeshEntry::draw(IView const & view, Matrix const & transform) const
{
	//TODO use the view from window to get used shader and only update the modified data
	// Set shader parameters
	m_shader->setParameter("ProjectionMatrix", view.getProjectionMatrix());
	m_shader->setParameter("ViewMatrix", view.getViewMatrix());
	m_shader->setParameter("ModelMatrix", transform);
	m_shader->setParameter("view_position", view.getPosition());
	m_shader->setParameter("material", m_material);

	// Bind textures
	if (m_material.diffuseTexture)
		m_material.diffuseTexture->bind(GL_TEXTURE0, GL_TEXTURE_2D);
	if (m_material.specularTexture)
		m_material.specularTexture->bind(GL_TEXTURE1, GL_TEXTURE_2D);

	// Draw
	glBindVertexArray(m_vertexArrayObject);
	glDrawElements(GL_TRIANGLES, m_indiceCount, GL_UNSIGNED_INT, (GLvoid*)0);
	glBindVertexArray(0);
}
