#include "ModelStorage.hpp"

#include "Console.hpp"
#include "Utils/String.hpp"

#include "Tile/Object/TileEntity.hpp"

bool SubMeshData::IsEmpty()
{
	return (this->DataIdx.size() <= 0);
}

SubMeshData::SubMeshData(const int& sub_mesh_idx)
{
	this->SubMeshIndex = sub_mesh_idx;
}

bool Model::IsEmpty() const
{
	return (this->subMeshData.size() <= 0 || (this->vertices.size() <= 0 && this->uvs.size() <= 0 && this->normals.size() <= 0));
}

void Model::WriteToFile(const glm::mat4& model_mat, WriterOffsetData& offset, std::ofstream& file, const TileEntity* pEntity) const
{
	for (const glm::vec3& vertex : this->vertices)
	{
		const glm::vec3 pVertPos = model_mat * glm::vec4(vertex, 1.0f);

		std::string output_str;
		output_str.append("v ");
		output_str.append(std::to_string(pVertPos.x));
		output_str.append(" ");
		output_str.append(std::to_string(pVertPos.y));
		output_str.append(" ");
		output_str.append(std::to_string(pVertPos.z));
		output_str.append("\n");

		file.write(output_str.c_str(), output_str.size());
	}

	for (const glm::vec2& uv : this->uvs)
	{
		std::string output_str;
		output_str.append("vt ");
		output_str.append(std::to_string(uv.x));
		output_str.append(" ");
		output_str.append(std::to_string(uv.y));
		output_str.append("\n");

		file.write(output_str.c_str(), output_str.size());
	}
	
	//implement normals here
	/*
	bool has_uv = (d_idx[1] > -1) && ConvertSettings::ExportUvs;
				bool has_normal = (d_idx[2] > -1) && ConvertSettings::ExportNormals;

				String::Combine(_f_str, " ", d_idx[0] + oData.Vertex + 1);

				if (!has_uv && !has_normal) continue;

				_f_str.append("/");

				if (has_uv)
					_f_str.append(std::to_string(d_idx[1] + oData.Texture + 1));

				if (has_normal)
					String::Combine(_f_str, "/", d_idx[2] + oData.Normal + 1);
	*/

	for (std::size_t mIdx = 0; mIdx < this->subMeshData.size(); mIdx++)
	{
		const SubMeshData* pSubMesh = this->subMeshData[mIdx];

		const std::string mtl_name = "usemtl " + pEntity->GetMtlName(pSubMesh->MaterialName, mIdx) + "\n";
		file.write(mtl_name.c_str(), mtl_name.size());

		for (std::size_t a = 0; a < pSubMesh->DataIdx.size(); a++)
		{
			std::string _f_str = "f";

			for (const std::vector<long long>& d_idx : pSubMesh->DataIdx[a])
			{
				_f_str.append(" ");
				_f_str.append(std::to_string(d_idx[0] + offset.Vertex + 1));

				const bool has_uv = (d_idx[1] > -1);

				if (!has_uv) continue;

				_f_str.append("/");

				if (has_uv) _f_str.append(std::to_string(d_idx[1] + offset.Uv + 1));
			}

			_f_str.append("\n");
			file.write(_f_str.c_str(), _f_str.size());
		}
	}

	offset.Vertex += this->vertices.size();
	offset.Uv	  += this->uvs.size();
	offset.Normal += this->normals.size();
}

Model::Model(const std::wstring& mesh_path)
{
	this->meshPath = mesh_path;
}

Model::~Model()
{
	for (SubMeshData*& data_ptr : this->subMeshData)
		delete data_ptr;
}



std::unordered_map<std::wstring, Model*> ModelStorage::CachedModels = {};
Assimp::Importer ModelStorage::Importer = Assimp::Importer();

const aiScene* ModelStorage::LoadScene(const std::wstring& path)
{
	return Importer.ReadFile(
		String::ToUtf8(path).c_str(),
		aiProcess_FindInvalidData |
		aiProcess_RemoveComponent |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_JoinIdenticalVertices |
		aiProcess_FindDegenerates |
		aiProcess_OptimizeMeshes
	);
}

void ModelStorage::LoadVertices(const aiMesh*& mesh, Model*& model)
{
	const bool has_uvs = mesh->HasTextureCoords(0);
	const bool has_normals = mesh->HasNormals();

	if (has_uvs) model->uvs.reserve(mesh->mNumVertices);
	if (has_normals) model->normals.reserve(mesh->mNumVertices);

	model->vertices.reserve(mesh->mNumVertices);
	for (unsigned int a = 0; a < mesh->mNumVertices; a++)
	{
		model->vertices.push_back(*reinterpret_cast<glm::vec3*>(&mesh->mVertices[a]));

		if (has_uvs) model->uvs.push_back(*reinterpret_cast<glm::vec2*>(&mesh->mTextureCoords[0][a]));
		if (has_normals) model->normals.push_back(*reinterpret_cast<glm::vec3*>(&mesh->mNormals[a]));
	}
}

void ModelStorage::LoadMaterialName(const aiScene*& scene, const aiMesh*& mesh, SubMeshData*& sub_mesh)
{
	if (!scene->HasMaterials()) return;

	const aiMaterial* cMeshMat = scene->mMaterials[mesh->mMaterialIndex];
	aiString cMatName;
	cMeshMat->Get(AI_MATKEY_NAME, cMatName);

	const std::string cStrMatName = std::string(cMatName.C_Str(), cMatName.length);
	sub_mesh->MaterialName = String::ToWide(cStrMatName);
}

void ModelStorage::LoadIndices(const aiMesh*& mesh, Model*& model, SubMeshData*& sub_mesh)
{
	const long long mVertOffset = model->vertices.size();
	const long long mUvOffset = model->uvs.size();
	const long long mNormalOffset = model->normals.size();

	const bool has_uvs = mesh->HasTextureCoords(0);
	const bool has_normals = mesh->HasNormals();

	sub_mesh->DataIdx.reserve(mesh->mNumFaces);
	for (unsigned int a = 0; a < mesh->mNumFaces; a++)
	{
		const aiFace& cFace = mesh->mFaces[a];
		std::vector<std::vector<long long>> d_idx;

		d_idx.reserve(cFace.mNumIndices);
		for (unsigned int b = 0; b < cFace.mNumIndices; b++)
		{
			long long ind_idx = (long long)cFace.mIndices[b];

			d_idx.push_back({
				ind_idx + mVertOffset,
				(has_uvs ? (ind_idx + mUvOffset) : -1),
				(has_normals ? (ind_idx + mNormalOffset) : -1)
				});
		}

		sub_mesh->DataIdx.push_back(d_idx);
	}
}

void ModelStorage::LoadSubMeshes(const aiScene*& scene, Model*& model)
{
	model->subMeshData.reserve(scene->mNumMeshes);
	for (unsigned int a = 0; a < scene->mNumMeshes; a++)
	{
		const aiMesh* cMesh = scene->mMeshes[a];
		SubMeshData* cSubMesh = new SubMeshData(a);

		ModelStorage::LoadIndices(cMesh, model, cSubMesh);
		ModelStorage::LoadVertices(cMesh, model);
		ModelStorage::LoadMaterialName(scene, cMesh, cSubMesh);

		model->subMeshData.push_back(cSubMesh);
	}
}

Model* ModelStorage::LoadModel(
	const std::wstring& path,
	const bool& load_uvs,
	const bool& load_normals
) {
	if (CachedModels.find(path) != CachedModels.end())
		return CachedModels.at(path);

	const aiScene* ModelScene = ModelStorage::LoadScene(path);
	if (ModelScene && ModelScene->HasMeshes())
	{
		Model* newModel = new Model(path);
		ModelStorage::LoadSubMeshes(ModelScene, newModel);

		DebugOutL("[Model] Loaded: ", path);

		CachedModels.insert(std::make_pair(path, newModel));
		Importer.FreeScene();

		return newModel;
	}

	Importer.FreeScene();
	return nullptr;
}

void ModelStorage::ClearStorage()
{
	for (auto& model_data : CachedModels)
		delete model_data.second;

	CachedModels.clear();
}