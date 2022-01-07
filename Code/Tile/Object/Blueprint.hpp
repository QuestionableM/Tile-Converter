#pragma once

#include "Tile/Object/TileEntity.hpp"
#include "Tile/Object/Block.hpp"
#include "Tile/Object/Part.hpp"
#include "Tile/Object/Joint.hpp"
#include "Utils/Json.hpp"

class Blueprint : public TileEntity
{
	Blueprint();

public:
	static Blueprint* LoadAutomatic(const std::string& str);
	static Blueprint* FromFile(const std::wstring& path);
	static Blueprint* FromJsonString(const std::string& json_str);

	//This vector contains blocks, parts and joints
	std::vector<TileEntity*> Objects = {};

	void AddObject(TileEntity* object);

	EntityType Type() const override;
	std::string GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const override;
	void FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const override;
	void WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const override;

	~Blueprint();

private:
	static glm::vec3 JsonToVector(const nlohmann::json& vec_json);

	void LoadBodies(const nlohmann::json& pJson);
	void LoadJoints(const nlohmann::json& pJson);
};