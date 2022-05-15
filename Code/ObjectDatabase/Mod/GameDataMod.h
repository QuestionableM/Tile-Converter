#pragma once

#include "Mod.hpp"

class GameDataMod : public Mod
{
public:
	GameDataMod();
	~GameDataMod() = default;

	ModType Type() const override;
	void LoadObjectDatabase() override;
};