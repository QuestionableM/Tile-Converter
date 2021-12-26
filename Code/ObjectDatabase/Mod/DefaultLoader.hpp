#pragma once

#include "Utils/Json.hpp"
#include "ObjectDatabase/ObjectData.hpp"

class DefaultLoader
{
public:
	static TextureList LoadTextureList(const nlohmann::json& texList);
	static void AddSubMesh(const nlohmann::json& subMesh, TextureData& tData, const std::wstring& idx);
	static bool LoadTextureData(const nlohmann::json& jLodList, TextureData& tData);
	static bool LoadRenderableData(const nlohmann::json& jRenderable, TextureData& tData, std::wstring& mesh);
	static bool LoadRenderable(const nlohmann::json& jAsset, TextureData& tData, std::wstring& mesh);
};