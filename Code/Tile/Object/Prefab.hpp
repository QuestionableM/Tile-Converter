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
	inline Prefab(const std::wstring& path, const std::wstring& flag = L"")
	{
		this->path = path;
		this->flag = flag;
	}

	Prefab(Prefab&) = delete;
	Prefab(const Prefab&) = delete;

	inline ~Prefab()
	{
		for (TileEntity*& pObject : this->Objects)
			delete pObject;
	}

	inline std::wstring GetPath() const { return this->path; }
	inline std::wstring GetFlag() const { return this->flag; }
	inline std::vector<TileEntity*> GetObjects() const { return this->Objects; }

	inline void AddObject(TileEntity* object)
	{
		//Check if the object is valid (Valid objects: Harvestable, Blueprint, Prefab, Asset)
		assert((static_cast<unsigned char>(object->Type()) & 0b100011011) != 0);

		this->Objects.push_back(object);
	}

	inline EntityType Type() const override { return EntityType::Prefab; }
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const override;
	std::size_t GetAmountOfObjects() const override;
};