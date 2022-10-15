#include "Prefab.h"

#include "Tile/Object/Asset.hpp"
#include "Tile/Object/Blueprint.hpp"
#include "ObjectDatabase/ModelStorage.hpp"

void Prefab::AddObject(TileEntity* object)
{
	//Check if the object is valid (Valid objects: Harvestable, Blueprint, Prefab, Asset)
	assert((static_cast<unsigned char>(object->Type()) & 0b00011011) != 0);

	this->Objects.push_back(object);
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

std::size_t Prefab::GetAmountOfObjects() const
{
	std::size_t output = 0;

	for (const TileEntity* tEntity : this->Objects)
		output += tEntity->GetAmountOfObjects();

	return output;
}