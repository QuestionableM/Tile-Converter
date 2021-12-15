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
/*
glm::vec4
rotateAround(
  glm::vec4 aPointToRotate,
  glm::vec4 aRotationCenter,
  glm::mat4x4 aRotationMatrix )
{
  glm::mat4x4 translate =
	glm::translate(
	  glm::mat4(),
	  glm::vec3( aRotationCenter.x, aRotationCenter.y, aRotationCenter.z ) );
  glm::mat4x4 invTranslate = glm::inverse( translate );

  // The idea:
  // 1) Translate the object to the center
  // 2) Make the rotation
  // 3) Translate the object back to its original location

  glm::mat4x4 transform = translate * aRotationMatrix * invTranslate;

  return transform * aPointToRotate;
}
*/

void TilePart::WriteToFile(std::ofstream& model, WriterOffsetData& mOffsetData, const int& xPos, const int& zPos)
{
	std::size_t asset_idx = 0;

	glm::vec3 tile_offset((float)xPos * 64.0f, 0.0f, (float)zPos * 64.0f);
	//glm::mat4 tile_matrix = glm::translate(glm::vec3((float)xPos * 64.0f, 0.0f, (float)zPos * 64.0f));
	/*
	float tile_x = x * 64;
	float tile_y = y * 64;
	float tile_z = cliff_level;

	int rot = TileData.getTileRotation(x, y);
		float rot_offset = rot * (float)(Math.PI / 2.0);
		Matrix4f transform = new Matrix4f()
			.translateLocal(-32, -32, 0).rotateLocalZ(rot_offset).translateLocal(32, 32, 0)
			.translateLocal(tile_x, tile_y, tile_z);
		
		TileCache tile_cache = new TileCache();
		tile_cache.mesh = parts.getMesh(ox, oy);
		tile_cache.mesh_modelMatrix = transform;
		
		for(int i = 0; i < 4; i++) {
			List<Asset> assets = part.assets[i];
			for(Asset asset : assets) {
				WorldAssetCache cache = handler.getAssetCache(asset.getUuid());
				if(cache != null) {
					Matrix4f modelMatrix = new Matrix4f(transform)
						.mul(MathUtils.getModelMatrix(asset));
					
					List<TileObject<Asset>> list = tile_cache.assets.get(cache);
					if(list == null) {
						list = new ArrayList<>();
						tile_cache.assets.put(cache, list);
					}
					
					list.add(new TileObject<>(asset, modelMatrix));
				}
			}
	*/

	constexpr const int tile_size = 32;

	const float half_width = (float)(this->Parent->GetWidth() * tile_size) / 2.0f;
	const float half_height = (float)(this->Parent->GetHeight() * tile_size) / 2.0f;

	for (std::size_t vec_idx = 0; vec_idx < this->Assets.size(); vec_idx++)
	{
		const std::vector<Asset*>& mAssetVec = this->Assets[vec_idx];

		//const glm::mat4 rotation_offset = glm::rotate(glm::radians(rotation_angle), glm::vec3(0.0f, 1.0f, 0.0f));

	//	glm::mat4 axis_rotation(1.0f);
		//axis_rotation = glm::translate(axis_rotation, glm::vec3(half_width, 0.0f, half_height));
		//axis_rotation *= rotation_offset;
		//axis_rotation = glm::translate(axis_rotation, glm::vec3(-half_width, 0.0f, -half_height));


		model << "o AssetGroup_" << vec_idx << "\n";

		for (Asset* cAsset : mAssetVec)
		{
			Model* pModel = cAsset->GetModel();

			const glm::vec3 cAssetPosOld = cAsset->GetPosition();

			const glm::vec3 cAssetPos = glm::vec3(cAssetPosOld.x, cAssetPosOld.z, cAssetPosOld.y) + tile_offset;
			//const glm::vec3 cAssetPos = glm::vec4(glm::vec3(cAssetPosOld.x, cAssetPosOld.z, cAssetPosOld.y) + tile_offset, 1.0f);
			const glm::quat oAssetRot = cAsset->GetRotation();

			//sm rotation xyzw
			//glm rotation wxyz
			const glm::quat cAssetRot(oAssetRot.w, oAssetRot.x, oAssetRot.y, oAssetRot.z);

			glm::mat4 model_mat(1.0f);
			model_mat *= (glm::toMat4(cAssetRot) * glm::rotate(glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
			model_mat *= glm::scale(cAsset->GetSize());

			for (const glm::vec3& vertex : pModel->vertices)
			{
				//const glm::vec3 pRotated = model_mat * glm::vec4(vertex, 1.0f);
				const glm::vec3 pRotated = glm::vec4(vertex, 1.0f) * model_mat;
				glm::vec3 pVertPos = pRotated + cAssetPos;

				std::string output_str;
				output_str.append("v ");
				output_str.append(std::to_string(pVertPos.x));
				output_str.append(" ");
				output_str.append(std::to_string(pVertPos.y));
				output_str.append(" ");
				output_str.append(std::to_string(pVertPos.z));
				output_str.append("\n");

				model.write(output_str.c_str(), output_str.size());
			}

			for (SubMeshData*& pSubMesh : pModel->subMeshData)
			{
				for (std::size_t a = 0; a < pSubMesh->DataIdx.size(); a++)
				{
					std::string _f_str = "f";

					for (const std::vector<long long>& d_idx : pSubMesh->DataIdx[a])
					{
						_f_str.append(" ");
						_f_str.append(std::to_string(d_idx[0] + mOffsetData.Vertex + 1));
					}

					_f_str.append("\n");
					model.write(_f_str.c_str(), _f_str.size());
				}
			}

			mOffsetData.Vertex += pModel->vertices.size();
			mOffsetData.Uv += pModel->uvs.size();
			mOffsetData.Normal += pModel->normals.size();
		}
	}
}