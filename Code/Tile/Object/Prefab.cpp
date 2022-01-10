#include "Prefab.h"

#include "Tile/Object/Asset.hpp"
#include "Tile/Object/Blueprint.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

Prefab::Prefab(const std::wstring& path, const std::wstring& flag)
{
	this->path = path;
	this->flag = flag;
}

Prefab::~Prefab()
{
	for (TileEntity*& pObject : this->Objects)
		delete pObject;
}

std::wstring Prefab::GetPath() const
{
	return this->path;
}

std::wstring Prefab::GetFlag() const
{
	return this->flag;
}

std::vector<TileEntity*> Prefab::GetObjects() const
{
	return this->Objects;
}

void Prefab::AddObject(TileEntity* object)
{
	assert(object->Type() == EntityType::Harvestable || object->Type() == EntityType::Asset || object->Type() == EntityType::Blueprint);

	this->Objects.push_back(object);
}


EntityType Prefab::Type() const
{
	return EntityType::Prefab;
}

std::string Prefab::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return "PREFAB_NOT_IMPLEMENTED";
}

void Prefab::WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const
{
	const glm::mat4 prefab_matrix = transform_matrix * this->GetTransformMatrix();

	for (const TileEntity* pEntity : this->Objects)
		pEntity->WriteObjectToFile(file, mOffset, prefab_matrix);
}

void Prefab::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	for (const TileEntity* pEntity : this->Objects)
		pEntity->FillTextureMap(tex_map);
}