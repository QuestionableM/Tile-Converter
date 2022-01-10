#include "ConvertSettingsGui.h"

#include "Tile/TileConverter.hpp"

namespace TileConverter
{
	ConvertSettingsGui::ConvertSettingsGui(const std::wstring& tile_name)
	{
		this->InitializeComponent();

		this->ExportUvs_CB->Checked       = ConvertSettings::ExportUvs;
		this->ExportNormals_CB->Checked   = ConvertSettings::ExportNormals;
		this->ExportMaterials_CB->Checked = ConvertSettings::ExportMaterials;

		this->ExportClutter_CB->Checked      = ConvertSettings::ExportClutter;
		this->ExportAssets_CB->Checked       = ConvertSettings::ExportAssets;
		this->ExportPrefabs_CB->Checked      = ConvertSettings::ExportPrefabs;
		this->ExportBlueprints_CB->Checked   = ConvertSettings::ExportBlueprints;
		this->ExportHarvestables_CB->Checked = ConvertSettings::ExportHarvestables;

		this->OutputName_TB->Text = gcnew System::String(tile_name.c_str());
	}

	ConvertSettingsGui::~ConvertSettingsGui()
	{
		if (components) delete components;
	}

	void ConvertSettingsGui::OutputName_TB_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		this->Convert_BTN->Enabled = (this->OutputName_TB->TextLength > 0);
	}

	void ConvertSettingsGui::Convert_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->CanConvert = true;
		this->Close();
	}

	void ConvertSettingsGui::ExportUvs_CB_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		const bool is_active = this->ExportUvs_CB->Checked;

		this->ExportMaterials_CB->Enabled = is_active;

		if (!is_active)
			this->ExportMaterials_CB->Checked = false;
	}
}