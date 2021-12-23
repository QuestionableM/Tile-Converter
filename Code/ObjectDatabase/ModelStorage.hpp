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

struct SubMeshData
{
	std::wstring MaterialName;
	unsigned int SubMeshIndex;

	std::vector<std::vector<std::vector<long long>>> DataIdx;

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
	void WriteToFile(const glm::mat4& model_mat, WriterOffsetData& offset, std::ofstream& file, const class TileEntity* pEntity) const;

	Model(const std::wstring& mesh_path);
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
	static Model* LoadModel(const std::wstring& path, const bool& load_uvs, const bool& load_normals);

	static void ClearStorage();
};