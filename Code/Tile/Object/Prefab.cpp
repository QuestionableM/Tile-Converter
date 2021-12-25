#include "Prefab.h"

#include "Tile/Object/Asset.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

Prefab::Prefab(const bool& loaded)
{
	this->loaded = loaded;
}

bool Prefab::IsLoaded() const
{
	return this->loaded;
}

std::wstring Prefab::GetPath() const
{
	return this->path;
}

std::wstring Prefab::GetFlag() const
{
	return this->flag;
}

std::vector<Prefab*> Prefab::GetPrefabs() const
{
	return this->Prefabs;
}

std::vector<Asset*> Prefab::GetAssets() const
{
	return this->Assets;
}

void Prefab::SetPath(const std::wstring& path)
{
	this->path = path;
}

void Prefab::SetFlag(const std::wstring& flag)
{
	this->flag = flag;
}

void Prefab::AddPrefab(Prefab* prefab)
{
	this->Prefabs.push_back(prefab);
}

void Prefab::AddAsset(Asset* asset)
{
	this->Assets.push_back(asset);
}

std::string Prefab::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return "PREFAB_NOT_IMPLEMENTED";
}

void Prefab::WriteToFile(std::ofstream& file, const glm::mat4& transform_mat, WriterOffsetData& mOffset)
{
	const glm::mat4 prefab_matrix = transform_mat * this->GetTransformMatrix();

	for (const Asset* cAsset : this->Assets)
	{
		const Model* pModel = cAsset->GetModel();
		const glm::mat4 model_matrix = prefab_matrix * cAsset->GetTransformMatrix();

		pModel->WriteToFile(model_matrix, mOffset, file, cAsset);
	}
}