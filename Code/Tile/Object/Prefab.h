#pragma once

#include "Tile/Object/TileEntity.hpp"
#include "Tile/WriterOffset.hpp"

class Asset;

class Prefab : public TileEntity
{
	std::wstring path;
	std::wstring flag;

	//std::vector<Blueprint*> Blueprints; //IMPLEMENT LATER
	std::vector<Prefab*> Prefabs;

	//std::vector<Node*> Nodes;
	std::vector<Asset*> Assets;

	std::vector<std::string> BlueprintPaths;
	std::vector<std::string> PrefabPaths;

	bool loaded;

public:
	Prefab(const bool& loaded = false);

	bool IsLoaded() const;
	std::wstring GetPath() const;
	std::wstring GetFlag() const;
	//std::vector<Blueprint*> GetBlueprints() const;
	std::vector<Prefab*> GetPrefabs() const;
	std::vector<Asset*> GetAssets() const;

	void SetPath(const std::wstring& path);
	void SetFlag(const std::wstring& flag);

	//void AddBlueprint(Blueprint* blueprint);
	void AddPrefab(Prefab* prefab);
	void AddAsset(Asset* asset);

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void WriteToFile(std::ofstream& file, const glm::mat4& transform_mat, WriterOffsetData& mOffset);
};