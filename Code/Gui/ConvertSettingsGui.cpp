#include "ConvertSettingsGui.h"

#include "Tile/TileConverter.hpp"
#include "Utils/String.hpp"

#include <msclr/marshal_cppstd.h>

#include <filesystem>
namespace fs = std::filesystem;
namespace WForms = System::Windows::Forms;

namespace TileConverter
{
	ConvertSettingsGui::ConvertSettingsGui(const std::wstring& tile_name)
	{
		this->InitializeComponent();

		this->ExportUvs_CB->Checked              = ConvertSettings::ExportUvs;
		this->ExportNormals_CB->Checked          = ConvertSettings::ExportNormals;
		this->ExportMaterials_CB->Checked        = ConvertSettings::ExportMaterials && ConvertSettings::ExportUvs;
		this->ExportGndTextures_CB->Checked      = ConvertSettings::ExportGroundTextures;
		this->Export8kGroundTextures_CB->Checked = ConvertSettings::ExportGroundTextures && ConvertSettings::Export8kGroundTextures;

		this->ExportMaterials_CB->Enabled        = this->ExportUvs_CB->Checked;
		this->Export8kGroundTextures_CB->Enabled = this->ExportGndTextures_CB->Checked;

		this->ExportClutter_CB->Checked      = ConvertSettings::ExportClutter;
		this->ExportAssets_CB->Checked       = ConvertSettings::ExportAssets;
		this->ExportPrefabs_CB->Checked      = ConvertSettings::ExportPrefabs;
		this->ExportBlueprints_CB->Checked   = ConvertSettings::ExportBlueprints;
		this->ExportHarvestables_CB->Checked = ConvertSettings::ExportHarvestables;
		this->ExportDecals_CB->Checked       = ConvertSettings::ExportDecals;

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
		const std::wstring tile_name = msclr::interop::marshal_as<std::wstring>(this->OutputName_TB->Text);
		
		std::error_code ec;
		const bool t_exists = fs::exists(L"./" + tile_name, ec);

		if (ec)
		{
			WForms::MessageBox::Show(
				"The specified file name is invalid!",
				"Invalid Filename",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);
		}
		else
		{
			this->CanConvert = true;
			this->Close();
		}
	}

	void ConvertSettingsGui::ExportUvs_CB_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		const bool is_active = this->ExportUvs_CB->Checked;

		this->ExportMaterials_CB->Enabled = is_active;

		if (!is_active)
			this->ExportMaterials_CB->Checked = false;
	}

	void ConvertSettingsGui::OutputName_TB_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e)
	{
		if (!String::IsPathCharacterAllowed(e->KeyChar) && e->KeyChar != 0x8 && e->KeyChar != 0x1)
			e->Handled = true;
	}

	void ConvertSettingsGui::ExportGndTextures_CB_CheckedChanged(System::Object^ sender, System::EventArgs^ e)
	{
		const bool v_isActive = this->ExportGndTextures_CB->Checked;

		this->Export8kGroundTextures_CB->Enabled = v_isActive;

		if (!v_isActive)
			this->Export8kGroundTextures_CB->Checked = false;
	}
}