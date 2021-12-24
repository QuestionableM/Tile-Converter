#include "Harvestable.hpp"

void Harvestable::SetColor(const Color& color)
{
	this->color = color;
}



Model* Harvestable::GetModel() const
{
	return this->pModel;
}

Color Harvestable::GetColor() const
{
	return this->color;
}

std::string Harvestable::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return uuid.ToString() + " " + color.StringHex() + " " + std::to_string(mIdx + 1);
}