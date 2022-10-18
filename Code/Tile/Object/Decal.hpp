#pragma once

#include "ObjectDatabase\ObjectData.hpp"
#include "TileEntity.hpp"

class Decal : public TileEntity
{
	const DecalData* m_Data;
public:
	Decal() = default;
};