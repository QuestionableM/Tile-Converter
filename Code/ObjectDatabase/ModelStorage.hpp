#pragma once

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>

#include <unordered_map>
#include <string>
#include <vector>
#include <fstream>

#include "Tile/WriterOffset.hpp"

#include <glm.hpp>

struct VertexData
{
	std::size_t m_Vert;
	std::size_t m_Uv;
	std::size_t m_Norm;
};

struct SubMeshData
{
	std::wstring m_MaterialName;
	unsigned int m_SubMeshIdx;

	std::vector<std::vector<VertexData>> m_DataIdx;

	bool has_normals;
	bool has_uvs;

	bool IsEmpty();
	SubMeshData(const int& sub_mesh_idx);
	~SubMeshData() = default;
};

struct Model
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<SubMeshData*> subMeshData;

	std::wstring meshPath;

	bool IsEmpty() const;
	void WriteToFile(const glm::mat4& model_mat, WriterOffsetData& offset, std::ofstream& file, const class TileEntity* pEntity);

	Model() = default;
	Model(const std::wstring& mesh_path);
	Model(const Model&) = delete;
	Model(Model&) = delete;
	~Model();
};

class ModelStorage
{
	static std::unordered_map<std::wstring, Model*> CachedModels;
	static Assimp::Importer Importer;

	static const aiScene* LoadScene(const std::wstring& path);
	static void LoadVertices(const aiMesh*& mesh, Model*& model);
	static void LoadMaterialName(const aiScene*& scene, const aiMesh*& mesh, SubMeshData*& sub_mesh);
	static void LoadIndices(const aiMesh*& mesh, Model*& model, SubMeshData*& sub_mesh);
	static void LoadSubMeshes(const aiScene*& scene, Model*& model);
public:
	static Model* LoadModel(const std::wstring& path);

	static void ClearStorage();
};