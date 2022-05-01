#include "Blueprint.hpp"

#include "ObjectDatabase/KeywordReplacer.hpp"
#include "ObjectDatabase/Mod/ObjectRotations.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "ObjectDatabase/ProgCounter.hpp"

#include "Utils/String.hpp"
#include "Utils/File.hpp"

#include "Console.hpp"

#include <gtx/quaternion.hpp>

Blueprint::Blueprint()
{
	this->size = glm::vec3(0.25f);
}

static const std::string bp_secret = "?JB:";

Blueprint* Blueprint::LoadAutomatic(const std::string& str)
{
	const std::size_t secret_idx = str.find(bp_secret);
	if (secret_idx != std::string::npos)
	{
		const std::string bp_str = str.substr(secret_idx + bp_secret.size());

		DebugOutL(0b0101_fg, "LoadingBlueprintJsonString: ", bp_str);

		return Blueprint::FromJsonString(bp_str);
	}
	else
	{
		const std::wstring wide_str = String::ToWide(str);
		const std::wstring bp_path = KeywordReplacer::ReplaceKey(wide_str);

		DebugOutL(0b0011_fg, "LoadingBlueprintPath: ", bp_path);

		return Blueprint::FromFile(bp_path);
	}

	return nullptr;
}

Blueprint* Blueprint::FromFile(const std::wstring& path)
{
	const std::string file_string = File::ReadToString(path);
	if (file_string.empty())
	{
		DebugErrorL("Couldn't load the specified blueprint: ", path);
		return nullptr;
	}

	return Blueprint::FromJsonString(file_string);
}

Blueprint* Blueprint::FromJsonString(const std::string& json_str)
{
	nlohmann::json bp_json = JsonReader::ParseJsonString(json_str);
	if (!bp_json.is_object()) return nullptr;

	Blueprint* nBlueprint = new Blueprint();

	nBlueprint->LoadBodies(bp_json);
	nBlueprint->LoadJoints(bp_json);

	return nBlueprint;
}

void Blueprint::AddObject(TileEntity* object)
{
	assert(object->Type() == EntityType::Block || object->Type() == EntityType::Part || object->Type() == EntityType::Joint);

	this->Objects.push_back(object);
}

EntityType Blueprint::Type() const
{
	return EntityType::Blueprint;
}

std::string Blueprint::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return "BLUEPRINT_MTL_NAME_NOT_NEEDED";
}

void Blueprint::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	for (const TileEntity* pEntity : this->Objects)
		pEntity->FillTextureMap(tex_map);
}

void Blueprint::WriteObjectToFile(std::ofstream& file, WriterOffsetData& mOffset, const glm::mat4& transform_matrix) const
{
	const glm::mat4 blueprint_matrix = transform_matrix * this->GetTransformMatrix();

	for (const TileEntity* pEntity : this->Objects)
		pEntity->WriteObjectToFile(file, mOffset, blueprint_matrix);
}

std::size_t Blueprint::GetAmountOfObjects() const
{
	return this->Objects.size();
}

Blueprint::~Blueprint()
{
	for (TileEntity*& pObject : this->Objects)
		delete pObject;
}

glm::vec3 Blueprint::JsonToVector(const nlohmann::json& vec_json)
{
	if (vec_json.is_object())
	{
		const auto& pos_x = JsonReader::Get(vec_json, "x");
		const auto& pos_y = JsonReader::Get(vec_json, "y");
		const auto& pos_z = JsonReader::Get(vec_json, "z");

		if (pos_x.is_number() && pos_y.is_number() && pos_z.is_number())
			return glm::vec3(pos_x.get<float>(), pos_y.get<float>(), pos_z.get<float>());
	}

	return glm::vec3(0.0f, 0.0f, 0.0f);
}

void Blueprint::LoadBodies(const nlohmann::json& pJson)
{
	const auto& pBodies = JsonReader::Get(pJson, "bodies");
	if (!pBodies.is_array()) return;

	for (const auto& pBody : pBodies)
	{
		const auto& bChilds = JsonReader::Get(pBody, "childs");
		if (!bChilds.is_array()) continue;

		for (const auto& bChild : bChilds)
		{
			if (!bChild.is_object()) continue;

			const auto& sUuid = JsonReader::Get(bChild, "shapeId");
			const auto& sPosition = JsonReader::Get(bChild, "pos");
			const auto& xAxis = JsonReader::Get(bChild, "xaxis");
			const auto& zAxis = JsonReader::Get(bChild, "zaxis");
			const auto& sBounds = JsonReader::Get(bChild, "bounds");
			const auto& sColor = JsonReader::Get(bChild, "color");

			if (!sUuid.is_string() || !sColor.is_string()) continue;

			const int xAxisInt = (xAxis.is_number() ? xAxis.get<int>() : 1);
			const int zAxisInt = (zAxis.is_number() ? zAxis.get<int>() : 3);

			const glm::vec3 sPositionVec = Blueprint::JsonToVector(sPosition);

			SMUuid obj_uuid = sUuid.get<std::string>();
			Color obj_color = sColor.get<std::string>();

			if (sBounds.is_object())
			{
				const glm::vec3 obj_bounds = Blueprint::JsonToVector(sBounds);

				if (!(obj_bounds.x > 0.0f && obj_bounds.y > 0.0f && obj_bounds.z > 0.0f))
					continue;

				BlockData* block_data = Mod::GetGlobalBlock(obj_uuid);
				if (!block_data) continue;

				Block* new_block = new Block(block_data, obj_bounds, obj_color, xAxisInt, zAxisInt);
				new_block->SetPosition(sPositionVec);
				new_block->SetSize(obj_bounds);

				this->AddObject(new_block);
			}
			else
			{
				PartData* p_data = Mod::GetGlobalPart(obj_uuid);
				if (!p_data) continue;

				Model* pModel = ModelStorage::LoadModel(p_data->Mesh);
				if (!pModel) continue;

				Part* new_part = new Part(p_data, pModel, obj_color, xAxisInt, zAxisInt);
				new_part->SetPosition(sPositionVec);

				this->AddObject(new_part);
			}
		}
	}
}

void Blueprint::LoadJoints(const nlohmann::json& pJson)
{
	const auto& pJoints = JsonReader::Get(pJson, "joints");
	if (!pJoints.is_array()) return;

	for (const auto& pJoint : pJoints)
	{
		const auto& xAxis = JsonReader::Get(pJoint, "xaxisA");
		const auto& zAxis = JsonReader::Get(pJoint, "zaxisA");
		const auto& jUuid = JsonReader::Get(pJoint, "shapeId");
		const auto& jColor = JsonReader::Get(pJoint, "color");
		//const auto& jChildA = JsonReader::Get(pJoint, "childA"); might implement later
		const auto& jPosition = JsonReader::Get(pJoint, "posA");

		if (!jUuid.is_string() || !jColor.is_string()) continue;

		const int xAxisInt = (xAxis.is_number() ? xAxis.get<int>() : 1);
		const int zAxisInt = (zAxis.is_number() ? zAxis.get<int>() : 3);

		const glm::vec3 jPositionVec = Blueprint::JsonToVector(jPosition);

		SMUuid joint_uuid = jUuid.get<std::string>();
		Color joint_color = jColor.get<std::string>();

		PartData* joint_data = Mod::GetGlobalPart(joint_uuid);
		if (!joint_data) continue;

		Model* pModel = ModelStorage::LoadModel(joint_data->Mesh);
		if (!pModel) continue;

		Joint* new_joint = new Joint(joint_data, pModel, joint_color, xAxisInt, zAxisInt);
		new_joint->SetPosition(jPositionVec);

		this->AddObject(new_joint);
	}
}