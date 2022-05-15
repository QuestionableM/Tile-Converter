#include "Joint.hpp"

#include "ObjectDatabase/ObjectData.hpp"
#include "ObjectDatabase/Mod/ObjectRotations.hpp"
#include "ObjectDatabase/Mod/MaterialManager.hpp"

#include "Console.hpp"

Joint::Joint(PartData* pParent, Model* pModel, const Color& color, const int& xAxis, const int& zAxis)
{
	this->pParent = pParent;
	this->uuid = pParent->Uuid;
	this->pModel = pModel;
	this->color = color;
	this->xAxis = xAxis;
	this->zAxis = zAxis;
}

Color Joint::GetColor() const
{
	return this->color;
}

EntityType Joint::Type() const
{
	return EntityType::Joint;
}

std::string Joint::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	const SubMeshData* pSubMesh = pModel->subMeshData[mIdx];
	const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(mIdx) : pSubMesh->m_MaterialName);

	std::string material_idx = "m1";

	ObjectTexData oTexData;
	if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		material_idx = MaterialManager::GetMaterialA(oTexData.Textures.material);

	return uuid.ToString() + " " + color.StringHex() + " " + std::to_string(mIdx + 1) + " " + material_idx;
}

void Joint::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	const std::string mtl_first_part = this->uuid.ToString() + " " + this->color.StringHex() + " ";
	for (std::size_t a = 0; a < pModel->subMeshData.size(); a++)
	{
		const SubMeshData* pSubMesh = pModel->subMeshData[a];
		const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(a) : pSubMesh->m_MaterialName);

		ObjectTexData oTexData;
		if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		{
			oTexData.TexColor = this->color;

			const std::string mat_idx = MaterialManager::GetMaterialA(oTexData.Textures.material);
			const std::string mtl_name = mtl_first_part + std::to_string(a + 1) + " " + mat_idx;

			if (tex_map.find(mtl_name) != tex_map.end())
				continue;

			tex_map.insert(std::make_pair(mtl_name, oTexData));
		}
	}
}

glm::mat4 Joint::GetTransformMatrix() const
{
	const glm::mat4 joint_rotation = Rotations::GetRotationMatrix(this->xAxis, this->zAxis);
	const glm::vec3 pos_offset = Rotations::GetOffsetPosition(this->xAxis, this->zAxis);

	glm::mat4 model_matrix(1.0f);
	model_matrix *= glm::translate(this->position + pos_offset);
	model_matrix *= joint_rotation;
	model_matrix *= glm::translate(pParent->Bounds / 2.0f);

	return model_matrix;
}