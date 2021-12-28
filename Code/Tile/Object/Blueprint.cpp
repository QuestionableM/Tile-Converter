#include "Blueprint.hpp"

#include "Tile/Object/Block.hpp"
#include "Tile/Object/Part.hpp"

#include "ObjectDatabase/Mod/ObjectRotations.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"
#include "Utils/File.hpp"
#include "Console.hpp"

#include <gtx/quaternion.hpp>

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

void GetPartPosAndBounds(
	glm::vec3& pos,
	glm::vec3& bounds,
	const int& xAxis,
	const int& zAxis,
	const bool& isJoint
) {
	int _XAxisAbs = glm::abs(xAxis);
	int _ZAxisAbs = glm::abs(zAxis);

	switch (_XAxisAbs) {
	case 3:
		switch (_ZAxisAbs) {
		case 1:
			bounds = glm::vec3(bounds.z, bounds.y, bounds.x);
			break;
		case 2:
			bounds = glm::vec3(bounds.y, bounds.z, bounds.x);
			break;
		}
		break;
	case 2:
		switch (_ZAxisAbs)
		{
		case 1:
			bounds = glm::vec3(bounds.z, bounds.x, bounds.y);
			break;
		case 3:
			bounds = glm::vec3(bounds.y, bounds.x, bounds.z);
			break;
		}
		break;
	case 1:
		if (_ZAxisAbs == 2)
			bounds = glm::vec3(bounds.x, bounds.z, bounds.y);
		break;
	}

	if (!isJoint) {
		if (xAxis == -1 || zAxis == -1 || (xAxis == 2 && zAxis == 3) || (xAxis == 3 && zAxis == -2) || (xAxis == -2 && zAxis == -3) || (xAxis == -3 && zAxis == 2))
			pos.x -= bounds.x;
		if (xAxis == -2 || zAxis == -2 || (xAxis == -1 && zAxis == 3) || (xAxis == -3 && zAxis == -1) || (xAxis == 1 && zAxis == -3) || (xAxis == 3 && zAxis == 1))
			pos.y -= bounds.y;
		if (xAxis == -3 || zAxis == -3 || (xAxis == -2 && zAxis == 1) || (xAxis == -1 && zAxis == -2) || (xAxis == 1 && zAxis == 2) || (xAxis == 2 && zAxis == -1))
			pos.z -= bounds.z;
	}
	else {
		if (!(zAxis > 0 || !(bounds.x != 1 || bounds.y != 1 || bounds.z != 1))) {
			switch (zAxis) {
			case -1:
				pos.x -= bounds.x - 1;
				break;
			case -2:
				pos.y -= bounds.y - 1;
				break;
			case -3:
				pos.z -= bounds.z - 1;
				break;
			}
		}
	}
}

//written by Brent Batch in C# and translated by Questionable Mark into C++
glm::mat4 GetPartRotationMatrix(glm::vec3& bounds, const int& xAxis, const int& zAxis)
{
	bool _XPos = (xAxis > 0);
	bool _ZPos = (zAxis > 0);
	int _AbsX = glm::abs(xAxis);
	int _AbsZ = glm::abs(zAxis);

	glm::mat4 _XRotation(1.0f);
	glm::mat4 _ZRotation(1.0f);

	switch (_AbsX)
	{
	case 1:
		if (!_XPos) _XRotation = glm::rotate(_XRotation, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		switch (_AbsZ)
		{
		case 2:
			_ZRotation = glm::rotate(_ZRotation, glm::radians(-90.0f), glm::vec3(_ZPos ? -1.0f : 1.0f, 0.0f, 0.0f));
			break;
		case 3:
			if (!_ZPos) _ZRotation = glm::rotate(_ZRotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		}
		break;
	case 2:
		_XRotation = glm::rotate(_XRotation, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, _XPos ? 1.0f : -1.0f));
		switch (_AbsZ)
		{
		case 1:
			_ZRotation = glm::rotate(_ZRotation, glm::radians(-90.0f), glm::vec3(0.0f, _ZPos ? 1.0f : -1.0f, 0.0f));
			break;
		case 3:
			if (!_ZPos) _ZRotation = glm::rotate(_ZRotation, glm::radians(-180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		}
		break;
	case 3:
		_XRotation = glm::rotate(_XRotation, glm::radians(90.0f/*-90.0f*/), glm::vec3(0.0f, _XPos ? -1.0f : 1.0f, 0.0f));
		switch (_AbsZ)
		{
		case 1:
			if (_ZPos == _XPos) _ZRotation = glm::rotate(_ZRotation, glm::radians(180.0f/*180.0f*/), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case 2:
			_ZRotation = glm::rotate(_ZRotation, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, (_ZPos == _XPos) ? -1.0f : 1.0f));
			break;
		}
		break;
	}
	/*switch (_AbsX)
	{
	case 1:
		if (!_XPos) _XRotation = glm::rotate(_XRotation, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		switch (_AbsZ)
		{
		case 2:
			_ZRotation = glm::rotate(_ZRotation, glm::radians(90.0f), glm::vec3(_ZPos ? -1.0f : 1.0f, 0.0f, 0.0f));
			break;
		case 3:
			if (!_ZPos) _ZRotation = glm::rotate(_ZRotation, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			break;
		}
		break;
	case 2:
		_XRotation = glm::rotate(_XRotation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, _XPos ? 1.0f : -1.0f));
		switch (_AbsZ)
		{
		case 1:
			_ZRotation = glm::rotate(_ZRotation, glm::radians(90.0f), glm::vec3(0.0f, _ZPos ? 1.0f : -1.0f, 0.0f));
			break;
		case 3:
			if (!_ZPos) _ZRotation = glm::rotate(_ZRotation, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			break;
		}
		break;
	case 3:
		_XRotation = glm::rotate(_XRotation, glm::radians(90.0f), glm::vec3(0.0f, _XPos ? -1.0f : 1.0f, 0.0f));
		switch (_AbsZ)
		{
		case 1:
			if (_ZPos == _XPos) _ZRotation = glm::rotate(_ZRotation, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			break;
		case 2:
			_ZRotation = glm::rotate(_ZRotation, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, (_ZPos == _XPos) ? -1.0f : 1.0f));
			break;
		}
		break;
	}*/

	/*
	_TranslatedPos = glm::vec4(_TranslatedPos, 1.0f) * _XRotation;
	_TranslatedPos = glm::vec4(_TranslatedPos, 1.0f) * _ZRotation;
	_TranslatedPos += bounds / 2.0f;
	*/

	return _XRotation * _ZRotation;
}

std::string Blueprint::GetMtlName(const std::wstring& mat_name, const std::size_t& mIdx) const
{
	return "BLUEPRINT_MTL_NAME_NOT_NEEDED";
}

void Blueprint::FillTextureMap(std::unordered_map<std::string, ObjectTexData>& tex_map) const
{
	//Implement later
}

void Blueprint::WriteToFile(std::ofstream& file, const glm::mat4& transform_mat, WriterOffsetData& mOffset) const
{
	const glm::mat4 blueprint_matrix = transform_mat * this->GetTransformMatrix();

	for (const Part* pPart : this->Parts)
	{
		const Model* pModel = pPart->GetModel();
		const glm::mat4 model_matrix = blueprint_matrix * pPart->GetTransformMatrix();

		pModel->WriteToFile(model_matrix, mOffset, file, pPart);
	}
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
				BlockData* b_data = Mod::GetGlobalBlock(obj_uuid);
				if (!b_data)
				{
					DebugErrorL("Couldn't find a block with the specified uuid: ", obj_uuid.ToString());
					continue;
				}
			}
			else
			{
				PartData* p_data = Mod::GetGlobalPart(obj_uuid);
				if (!p_data)
				{
					DebugErrorL("Couldn't find a part with the specified uuid: ", obj_uuid.ToString());
					continue;
				}

				Model* pModel = ModelStorage::LoadModel(p_data->Mesh, true, true);
				if (!pModel) continue;

				Part* new_part = new Part();
				new_part->uuid = obj_uuid;
				new_part->color = obj_color;
				new_part->pModel = pModel;
				new_part->pParent = p_data;
				new_part->xAxis = xAxisInt;
				new_part->zAxis = zAxisInt;

				//glm::vec3 part_position = sPositionVec;
				//glm::vec3 part_bounds = p_data->Bounds;

				//GetPartPosAndBounds(part_position, part_bounds, xAxisInt, zAxisInt, false);

				//glm::mat4 part_rotation = GetPartRotationMatrix(part_bounds, xAxisInt, zAxisInt);

				new_part->SetPosition(sPositionVec);
				//new_part->SetRotation(glm::toQuat(part_rotation));

				this->Parts.push_back(new_part);
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