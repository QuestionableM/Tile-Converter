#include "TileEntity.hpp"

#include "ObjectDatabase/ModelStorage.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

glm::vec3 TileEntity::GetPosition() const
{
	return position;
}

glm::quat TileEntity::GetRotation() const
{
	return rotation;
}

glm::vec3 TileEntity::GetSize() const
{
	return size;
}

SMUuid TileEntity::GetUuid() const
{
	return uuid;
}

Model* TileEntity::GetModel() const
{
	return pModel;
}

glm::mat4 TileEntity::GetTransformMatrix() const
{
	glm::mat4 transform(1.0f);

	transform *= glm::translate(this->position);
	transform *= glm::toMat4(this->rotation);
	transform *= glm::scale(this->size);

	return transform;
}


void TileEntity::WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const
{
	const glm::mat4 model_matrix = transform_matrix * this->GetTransformMatrix();

	pModel->WriteToFile(model_matrix, mOffset, file, this);

	ProgCounter::ProgressValue++;
}


std::size_t TileEntity::GetAmountOfObjects() const
{
	return 1;
}

void TileEntity::SetPosition(const glm::vec3& pos)
{
	this->position = pos;
}

void TileEntity::SetRotation(const glm::quat & rot)
{
	this->rotation = rot;
}

void TileEntity::SetSize(const glm::vec3& size)
{
	this->size = size;
}

void TileEntity::SetUuid(const SMUuid& uuid)
{
	this->uuid = uuid;
}

void TileEntity::SetModel(Model* model)
{
	this->pModel = model;
}