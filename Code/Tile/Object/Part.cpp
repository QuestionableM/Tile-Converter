#include "Part.hpp"
#include "Console.hpp"
#include "ObjectDatabase/Mod/ObjectRotations.hpp"

Model* Part::GetModel() const
{
	return pModel;
}

Color Part::GetColor() const
{
	return this->color;
}

std::string Part::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	//IMPLEMENT LATER
	return "PART_" + std::to_string(this->xAxis) + "_" + std::to_string(this->zAxis);
}

void Part::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	//IMPLEMENT LATER
}

glm::mat4 Part::GetTransformMatrix() const
{
	const glm::mat4 axis_rotation = Rotations::GetRotationMatrix(this->xAxis, this->zAxis);
	const glm::vec3 bVec = pParent->Bounds;

	glm::mat4 model_matrix(1.0f);
	model_matrix *= glm::translate(this->position);
	model_matrix *= axis_rotation;
	model_matrix *= glm::translate(bVec / 2.0f);
	//model_matrix *= glm::translate(bVec / 2.0f);
	//model_matrix *= axis_rotation;
	//model_matrix *= glm::translate(this->position);

	return model_matrix;
	/*
	
	iData.RotationMatrix = Rotations::GetRotationMatrix(this->xAxis, this->zAxis);
		const Engine::Vec3 bVec = this->pPartData->Bounds;

		iData.TransformMatrix = (Engine::Matrix().Translate(bVec / 2.0f) * iData.RotationMatrix).Translate(this->Position);*/
}