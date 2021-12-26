#include "Blueprint.hpp"

#include "Utils/File.hpp"
#include "Console.hpp"

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

std::string Blueprint::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return "BLUEPRINT_MTL_NAME_NOT_NEEDED";
}

void Blueprint::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	//Implement later
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

			DebugOutL("Uuid: ", obj_uuid.ToString());
			DebugOutL("Color: ", obj_color.StringHex());
			DebugOutL("xAxis: ", xAxisInt);
			DebugOutL("zAxis: ", zAxisInt);
			DebugOutL("Position: ", sPositionVec.x, ", ", sPositionVec.y, ", ", sPositionVec.z);

			if (sBounds.is_object())
			{
				const glm::vec3 obj_bounds = Blueprint::JsonToVector(sBounds);

				if (!(obj_bounds.x > 0.0f && obj_bounds.y > 0.0f && obj_bounds.z > 0.0f))
					continue;

				DebugOutL("Bounds: ", obj_bounds.x, ", ", obj_bounds.y, ", ", obj_bounds.z);
			}
			else
			{

			}

			DebugOutL("");
			/*
			const auto& _ShapeId = Json::Get(_Child, "shapeId");
				const auto& _Pos	 = Json::Get(_Child, "pos");
				const auto& _XAxis	 = Json::Get(_Child, "xaxis");
				const auto& _ZAxis	 = Json::Get(_Child, "zaxis");
				const auto& _Bounds  = Json::Get(_Child, "bounds");
				const auto& _Color	 = Json::Get(_Child, "color");

				if (!(_ShapeId.is_string() && _Pos.is_object() && _XAxis.is_number() && _ZAxis.is_number())) continue;
				const auto& _PosX = Json::Get(_Pos, "x");
				const auto& _PosY = Json::Get(_Pos, "y");
				const auto& _PosZ = Json::Get(_Pos, "z");

				if (!(_PosX.is_number() && _PosY.is_number() && _PosZ.is_number())) continue;
				glm::vec3 _PosVec(_PosX.get<float>(), _PosY.get<float>(), _PosZ.get<float>());

				SMBC::Uuid uuid_obj(_ShapeId.get<std::string>());
				std::string color_str = (_Color.is_string() ? _Color.get<std::string>() : "000000");

				if (_Bounds.is_object())
				{
					const auto& _BoundX = Json::Get(_Bounds, "x");
					const auto& _BoundY = Json::Get(_Bounds, "y");
					const auto& _BoundZ = Json::Get(_Bounds, "z");

					if (!(_BoundX.is_number() && _BoundY.is_number() && _BoundZ.is_number())) continue;
					glm::vec3 _BoundsVec(_BoundX.get<float>(), _BoundY.get<float>(), _BoundZ.get<float>());

					const SMBC::BlockData* _BlockD = Mod::GetBlock(uuid_obj);
					if (!_BlockD) continue;

					SMBC::Block* _Block = new SMBC::Block();
					_Block->blkPtr	    = (SMBC::BlockData*)_BlockD;
					_Block->Bounds	    = _BoundsVec;
					_Block->Color	    = color_str;
					_Block->Position    = _PosVec;
					_Block->Uuid	    = _BlockD->Uuid;
					_Block->xAxis	    = _XAxis.get<int>();
					_Block->zAxis	    = _ZAxis.get<int>();
					_Block->ObjectIndex	= this->objectIndexValue;

					this->CollectionBindFunction(*this, _Block, false, body_index);
				}
				else
				{
					const SMBC::PartData* part_data = SMBC::Mod::GetPart(uuid_obj);
					if (!part_data) continue;

					SMBC::Part* _Part  = new SMBC::Part();
					_Part->objPtr	   = (SMBC::PartData*)part_data;
					_Part->Uuid		   = _Part->objPtr->Uuid;
					_Part->Color	   = color_str;
					_Part->Bounds	   = _Part->objPtr->Bounds;
					_Part->Position    = _PosVec;
					_Part->xAxis	   = _XAxis.get<int>();
					_Part->zAxis	   = _ZAxis.get<int>();
					_Part->ObjectIndex = this->objectIndexValue;

					BPFunction::GetPartPosAndBounds(_Part->Position, _Part->Bounds, _Part->xAxis, _Part->zAxis, false);

					this->CollectionBindFunction(*this, _Part, false, body_index);
				}

				this->objectIndexValue++;
				ConvData::ProgressValue++;
			*/
		}
	}
}

void Blueprint::LoadJoints(const nlohmann::json& pJson)
{
	const auto& pJoints = JsonReader::Get(pJson, "joints");
	if (!pJoints.is_array()) return;

	DebugOutL("Joints: ", pJoints.type_name());
}