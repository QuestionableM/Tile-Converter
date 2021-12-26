#pragma once

#include "Tile/Object/TileEntity.hpp"
#include "Utils/Json.hpp"

class Blueprint : public TileEntity
{
public:
	static Blueprint* FromFile(const std::wstring& path);
	static Blueprint* FromJsonString(const std::string& json_str);

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;

private:
	static glm::vec3 JsonToVector(const nlohmann::json& vec_json);

	void LoadBodies(const nlohmann::json& pJson);
	void LoadJoints(const nlohmann::json& pJson);
};