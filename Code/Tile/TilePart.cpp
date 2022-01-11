#include "TilePart.hpp"

#include "Tile/Tile.hpp"
#include "Utils/String.hpp"

#include <gtx/quaternion.hpp>
#include <gtx/transform.hpp>

TilePart::TilePart(Tile* parent)
{
	this->Parent = parent;

	VertexColor.resize(33 * 33);
	VertexHeight.resize(33 * 33);
	Ground.resize(65 * 65);
	Clutter.resize(128 * 128);
}

TilePart::~TilePart()
{
	for (TileClutter*& pClutter : this->ClutterMap)
	{
		if (!pClutter) continue;

		delete pClutter;
	}

	for (std::size_t a = 0; a < this->Objects.size(); a++)
	{
		for (TileEntity*& pEntity : this->Objects[a])
			delete pEntity;
	}
}

void TilePart::SetVertexColor(const std::vector<int>& vert_array)
{
	this->VertexColor = vert_array;
}

void TilePart::SetVertexHeight(const std::vector<float>& height_array)
{
	this->VertexHeight = height_array;
}

void TilePart::SetGroundMaterials(const std::vector<long long>& material_array)
{
	this->Ground = material_array;
}

void TilePart::SetClutter(const std::vector<SignedByte>& clutter_array)
{
	this->Clutter = clutter_array;
}

void TilePart::AddObject(TileEntity* object, const int& index)
{
	assert(object != nullptr);
	assert(0 <= index && index <= 3);
	assert(object->Type() == EntityType::Asset || object->Type() == EntityType::Harvestable || object->Type() == EntityType::Blueprint || object->Type() == EntityType::Prefab);
	assert((index > 0 && object->Type() == EntityType::Asset || object->Type() == EntityType::Harvestable) || index == 0);

	this->Objects[index].push_back(object);
}

Tile* TilePart::GetParent()
{
	return this->Parent;
}

void TilePart::WriteToFile(std::ofstream& model, WriterOffsetData& mOffsetData, const int& xPos, const int& zPos)
{
	constexpr const float rot_offset = 1.0f * glm::pi<float>();

	constexpr const float tile_size = 64.0f;
	const float tWidth  = this->Parent->GetWidth()  * tile_size;
	const float tHeight = this->Parent->GetHeight() * tile_size;

	const glm::vec3 half_point(tWidth / 2.0f, tHeight / 2.0f, 0.0f);

	glm::mat4 transform(1.0f);
	transform *= glm::translate(-half_point);
	transform *= glm::rotate(rot_offset, glm::vec3(0.0f, 0.0f, 1.0f));
	transform *= glm::translate(half_point);
	transform *= glm::translate(glm::vec3((float)xPos * 64.0f, (float)zPos * 64.0f, 0.0f));
	transform *= glm::translate(glm::vec3(-tWidth * 1.5f, -tHeight * 1.5f, 0.0f));

	for (std::size_t vec_idx = 0; vec_idx < this->Objects.size(); vec_idx++)
	{
		const std::vector<TileEntity*>& entity_array = this->Objects[vec_idx];

		for (std::size_t a = 0; a < entity_array.size(); a++)
			entity_array[a]->WriteObjectToFile(model, mOffsetData, transform);
	}
}

void TilePart::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tData) const
{
	for (std::size_t a = 0; a < Objects.size(); a++)
	{
		for (const TileEntity* pEntity : Objects[a])
			pEntity->FillTextureMap(tData);

		for (const TileClutter* pClutter : ClutterMap)
		{
			if (!pClutter) continue;

			pClutter->FillTextureMap(tData);
		}
	}
}

std::size_t TilePart::GetAmountOfObjects() const
{
	std::size_t output = 0;

	for (const std::vector<TileEntity*>& tEntityVec : this->Objects)
	{
		for (const TileEntity* tEntity : tEntityVec)
			output += tEntity->GetAmountOfObjects();
	}

	return output;
}