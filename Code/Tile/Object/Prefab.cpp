#include "Prefab.h"

#include "Tile/Object/Asset.hpp"
#include "Tile/Object/Blueprint.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

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

void Prefab::AddBlueprint(Blueprint* blueprint)
{
	this->Blueprints.push_back(blueprint);
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

void Prefab::WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const
{
	const glm::mat4 prefab_matrix = transform_matrix * this->GetTransformMatrix();

	for (const Asset* cAsset : this->Assets)
		cAsset->WriteObjectToFile(file, mOffset, prefab_matrix);

	for (const Blueprint* cBlueprint : this->Blueprints)
		cBlueprint->WriteObjectToFile(file, mOffset, prefab_matrix);

	for (const Prefab* cPrefab : this->Prefabs)
		cPrefab->WriteObjectToFile(file, mOffset, prefab_matrix);
}

void Prefab::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	for (const Asset* cAsset : this->Assets)
		cAsset->FillTextureMap(tex_map);

	for (const Blueprint* cBlueprint : this->Blueprints)
		cBlueprint->FillTextureMap(tex_map);

	for (const Prefab* cPrefab : this->Prefabs)
		cPrefab->FillTextureMap(tex_map);
}