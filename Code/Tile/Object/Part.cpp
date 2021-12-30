#include "Part.hpp"
#include "Console.hpp"
#include "ObjectDatabase/Mod/ObjectRotations.hpp"

Color Part::GetColor() const
{
	return this->color;
}

EntityType Part::Type() const
{
	return EntityType::Part;
}

std::string Part::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return uuid.ToString() + " " + color.StringHex() + " " + std::to_string(mIdx + 1);
}

void Part::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	for (std::size_t a = 0; a < pModel->subMeshData.size(); a++)
	{
		const std::string mtl_name = uuid.ToString() + " " + color.StringHex() + " " + std::to_string(a + 1);

		if (tex_map.find(mtl_name) != tex_map.end())
			continue;

		const SubMeshData* pSubMesh = pModel->subMeshData[a];
		const std::wstring tex_name = (pParent->Textures.Type() == TextureDataType::SubMeshList ? std::to_wstring(a) : pSubMesh->MaterialName);

		ObjectTexData oTexData;
		if (pParent->Textures.GetEntry(tex_name, oTexData.Textures))
		{
			oTexData.TexColor = color;

			tex_map.insert(std::make_pair(mtl_name, oTexData));
		}
	}
}

glm::mat4 Part::GetTransformMatrix() const
{
	const glm::mat4 axis_rotation = Rotations::GetRotationMatrix(this->xAxis, this->zAxis);

	glm::mat4 model_matrix(1.0f);
	model_matrix *= glm::translate(this->position);
	model_matrix *= axis_rotation;
	model_matrix *= glm::translate(pParent->Bounds / 2.0f);

	return model_matrix;
}