#pragma once

#include "Tile/Object/TileEntity.hpp"
#include "Tile/Object/Block.hpp"
#include "Tile/Object/Part.hpp"
#include "Utils/Json.hpp"

class Blueprint : public TileEntity
{
public:
	static Blueprint* FromFile(const std::wstring& path);
	static Blueprint* FromJsonString(const std::string& json_str);

	std::vector<Block*> Blocks = {};
	std::vector<Part*> Parts = {};

	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;

	void WriteToFile(std::ofstream& file, const glm::mat4& transform_mat, WriterOffsetData& mOffset) const;

	~Blueprint() = default;

private:
	static glm::vec3 JsonToVector(const nlohmann::json& vec_json);

	void LoadBodies(const nlohmann::json& pJson);
	void LoadJoints(const nlohmann::json& pJson);
};