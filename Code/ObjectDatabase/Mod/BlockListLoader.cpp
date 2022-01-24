#include "BlockListLoader.hpp"

#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/KeywordReplacer.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Utils/Uuid.hpp"
#include "Utils/String.hpp"

#include "Console.hpp"

static const std::string blkTexNames[3] = { "dif", "asg", "nor" };
bool BlockListLoader::GetBlockTextures(const nlohmann::json& block, TextureList& tex)
{
	for (int a = 0; a < 3; a++)
	{
		const auto& bTexture = JsonReader::Get(block, blkTexNames[a]);

		if (bTexture.is_string())
		{
			std::wstring& strRef = tex.GetStringRef(a);

			strRef = String::ToWide(bTexture.get<std::string>());
			strRef = KeywordReplacer::ReplaceKey(strRef);
		}
	}

	return (!tex.dif.empty() || !tex.asg.empty() || !tex.nor.empty());
}

void BlockListLoader::GetBlockMaterial(const nlohmann::json& block, TextureList& tex)
{
	const auto& bGlass = JsonReader::Get(block, "glass");
	const auto& bAlpha = JsonReader::Get(block, "alpha");

	if (bGlass.is_boolean() && bGlass.get<bool>())
	{
		tex.material = L"BlockGlass";
	}
	else
	{
		tex.material = L"BlockDifAsgNor";

		if (bAlpha.is_boolean() && bAlpha.get<bool>())
			tex.material.append(L"Alpha");
	}
}

void BlockListLoader::Load(const nlohmann::json& fBlocks, Mod* mod)
{
	if (!fBlocks.is_array()) return;
	DebugOutL("Loading Block List...");

	ProgCounter::ProgressMax += fBlocks.size();
	for (const auto& fBlock : fBlocks)
	{
		if (!fBlock.is_object()) continue;

		const auto& bUuid = JsonReader::Get(fBlock, "uuid");
		const auto& bTiling = JsonReader::Get(fBlock, "tiling");
		const auto& bColor = JsonReader::Get(fBlock, "color");

		if (!bUuid.is_string()) continue;

		SMUuid block_uuid = bUuid.get<std::string>();
		if (Mod::BlockStorage.find(block_uuid) != Mod::BlockStorage.end())
		{
			DebugWarningL("Block with this uuid already exists! (", block_uuid.ToString(), ")");
			continue;
		}

		TextureList tList;
		if (!BlockListLoader::GetBlockTextures(fBlock, tList)) continue;
		BlockListLoader::GetBlockMaterial(fBlock, tList);

		int tiling_value = (bTiling.is_number() ? bTiling.get<int>() : 4);
		if (tiling_value > 16 || tiling_value <= 0) tiling_value = 4;

		BlockData* new_block = new BlockData();
		new_block->Uuid = block_uuid;
		new_block->Textures = tList;
		new_block->Tiling = tiling_value;
		new_block->DefaultColor = bColor.get<std::string>();
		new_block->pMod = mod;
		
		const auto new_pair = std::make_pair(new_block->Uuid, new_block);

		Mod::BlockStorage.insert(new_pair);
		mod->Blocks.insert(new_pair);

		ProgCounter::ProgressValue++;
	}
}