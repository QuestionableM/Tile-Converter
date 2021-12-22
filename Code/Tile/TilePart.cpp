#include "TilePart.hpp"

#include "Tile/Tile.hpp"

#include <gtx/quaternion.hpp>
#include <gtx/transform.hpp>

TilePart::TilePart(Tile* parent)
{
	this->Parent = parent;

	VertexColor.resize(33 * 33);
	VertexHeight.resize(33 * 33);
	Ground.resize(65 * 65);
	Clutter.resize(128 * 128);

	Assets.resize(4);
	//Harvestables.reserve(4);
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

void TilePart::SetClutter(const std::vector<Byte>& clutter_array)
{
	this->Clutter = clutter_array;
}

void TilePart::AddAsset(Asset* asset, const int& index)
{
	assert(asset != nullptr);
	assert(0 <= index && index <= 3);

	Assets[index].push_back(asset);
}

//void AddHarvestable(Harvestable* harvestable, const int& index)
//{
//	assert(harvestable == nullptr);
//	assert(index < 0 || index > 3);

//	Harvestables[index].push_back(harvestable);
//}

//void AddNode(Node* node)
//{
//	assert(node == nullptr);

//	Nodes.push_back(node);
//}

//void AddPrefab(Prefab* prefab)
//{
//	assert(prefab == nullptr);

//	Prefabs.push_back(prefab);
//}

//void AddBlueprint(Blueprint* blueprint)
//{
//	assert(blueprint == nullptr);

//	Blueprints.push_back(blueprint);
//}

//void AddDecal(Decal* decal)
//{
//	assert(decal == nullptr);

//	Decals.push_back(decal);
//}

Tile* TilePart::GetParent()
{
	return this->Parent;
}

glm::vec3 rotate_around(const glm::vec3& point, const glm::vec3& center, const glm::mat4& rot_mat)
{
	glm::mat4 translate = glm::translate(glm::mat4(1.0f), center);
	glm::mat4 invTranslate = glm::inverse(translate);

	glm::mat4 transform = translate * rot_mat * invTranslate;

	return transform * glm::vec4(point, 1.0f);
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

	for (std::size_t vec_idx = 0; vec_idx < this->Assets.size(); vec_idx++)
	{
		for (Asset*& cAsset : this->Assets[vec_idx])
		{
			const Model* pModel = cAsset->GetModel();
			const glm::mat4 model_matrix = transform * cAsset->GetTransformMatrix();

			pModel->WriteToFile(model_matrix, mOffsetData, model);
		}
	}
}