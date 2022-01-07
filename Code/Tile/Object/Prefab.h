#pragma once

#include "Tile/Object/TileEntity.hpp"
#include "Tile/WriterOffset.hpp"

class Asset;
class Blueprint;

class Prefab : public TileEntity
{
	std::wstring path;
	std::wstring flag;

	//This vector contains blueprints, prefabs and assets
	std::vector<TileEntity*> Objects = {};

public:
	Prefab(const std::wstring& path, const std::wstring& flag = L"");
	Prefab(Prefab&) = delete;
	Prefab(const Prefab&) = delete;
	~Prefab();

	std::wstring GetPath() const;
	std::wstring GetFlag() const;
	std::vector<TileEntity*> GetObjects() const;

	void AddObject(TileEntity* object);

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const override;
};