#include "MainGui.h"
#include "Console.hpp"

#include <CommCtrl.h>
#include <msclr/marshal_cppstd.h>

#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"

#include "Tile/TileConverter.hpp"
#include "Utils/File.hpp"

#include "Gui/AboutGui.h"
#include "Gui/SettingsGui.h"
#include "Gui/ConvertSettingsGui.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace WForms = System::Windows::Forms;

namespace TileConverter
{
	MainGui::MainGui()
	{
		this->InitializeComponent();
	}

	MainGui::~MainGui()
	{
		if (components) delete components;
	}

	void MainGui::MainGui_Load(System::Object^ sender, System::EventArgs^ e)
	{
		DebugOutL("MainGui has been loaded!");

		SendMessage(
			static_cast<HWND>(TilePath_TB->Handle.ToPointer()),
			EM_SETCUEBANNER,
			0,
			(LPARAM)L"Path to Tile"
		);

		DatabaseLoader::InitializeDatabase();

		if (DatabaseConfig::GamePath.empty() || !File::Exists(DatabaseConfig::GamePath))
		{
			WForms::DialogResult dr = WForms::MessageBox::Show(
				"The program couldn't find a path to Scrap Mechanic.\n\nWould you like to specify the path to Scrap Mechanic by yourself?",
				"No Game Path",
				WForms::MessageBoxButtons::YesNo,
				WForms::MessageBoxIcon::Question
			);

			if (dr == WForms::DialogResult::Yes)
			{
				this->TS_Settings_BTN_Click(nullptr, nullptr);
				return;
			}
		}

		this->LoadObjectDatabase();
	}

	void MainGui::LoadObjectDatabase()
	{
		if (this->DatabaseLoader_BW->IsBusy) return;

		DebugOutL(ConCol::BLUE_INT, "Loading object database...");

		this->ChangeGuiState(false, false);
		this->DatabaseLoader_BW->RunWorkerAsync();
	}

	void MainGui::DatabaseLoader_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
	{
		DebugOutL("Loading object database from another thread...");
		DatabaseLoader::LoadDatabase();
	}

	void MainGui::DatabaseLoader_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		DebugOutL("Loading object database on a separate thread has been successfully finished!");
		this->ChangeGuiState(true, false);
	}

	void MainGui::TS_ReloadDB_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->LoadObjectDatabase();
	}

	void MainGui::ChangeGuiState(const bool& db_loaded, const bool& tile_gen)
	{
		const bool ui_active = (db_loaded && !tile_gen);

		this->menuStrip1->Enabled = ui_active;
		this->TilePath_TB->Enabled = ui_active;
		this->Convert_BTN->Enabled = (ui_active && this->TilePath_TB->TextLength > 0);
		this->TilePathSelector_BTN->Enabled = ui_active;
	}

	void MainGui::TilePath_TB_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		this->Convert_BTN->Enabled = (this->TilePath_TB->TextLength > 0);
	}

	void MainGui::Convert_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (this->TilePath_TB->TextLength <= 0) return;

		const std::wstring tile_path = msclr::interop::marshal_as<std::wstring>(this->TilePath_TB->Text);

		std::error_code ec;
		if (!fs::exists(tile_path, ec) || ec)
		{
			WForms::MessageBox::Show(
				"The specified path does not exist!",
				"Invalid Path",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);
			return;
		}

		if (!fs::is_regular_file(tile_path, ec) || ec)
		{
			WForms::MessageBox::Show(
				"The specified path does not contain a path to the file",
				"Invalid Path",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);
			return;
		}

		fs::path tPath = tile_path;
		const std::wstring tile_name = tPath.has_stem() ? tPath.stem().wstring() : L"UnkonwnTile";

		ConvertSettingsGui^ conv_settings = gcnew ConvertSettingsGui(tile_name);
		conv_settings->ShowDialog();

		if (conv_settings->CanConvert)
		{
			this->ChangeGuiState(true, true);

			System::Array^ thread_data = gcnew cli::array<System::Object^>(10);
			thread_data->SetValue(this->TilePath_TB->Text, (int)0);
			thread_data->SetValue(conv_settings->OutputName_TB->Text, (int)1);

			thread_data->SetValue(conv_settings->ExportUvs_CB->Checked, (int)2);
			thread_data->SetValue(conv_settings->ExportNormals_CB->Checked, (int)3);
			thread_data->SetValue(conv_settings->ExportMaterials_CB->Checked, (int)4);

			thread_data->SetValue(conv_settings->ExportClutter_CB->Checked, (int)5);
			thread_data->SetValue(conv_settings->ExportAssets_CB->Checked, (int)6);
			thread_data->SetValue(conv_settings->ExportPrefabs_CB->Checked, (int)7);
			thread_data->SetValue(conv_settings->ExportBlueprints_CB->Checked, (int)8);
			thread_data->SetValue(conv_settings->ExportHarvestables_CB->Checked, (int)9);

			this->TileConverter_BW->RunWorkerAsync(thread_data);
		}
	}

	void MainGui::TileConverter_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
	{
		System::Array^ tData = safe_cast<System::Array^>(e->Argument);

		System::String^ tile_path = safe_cast<System::String^>(tData->GetValue((int)0));
		System::String^ tile_name = safe_cast<System::String^>(tData->GetValue((int)1));
		
		const bool export_uvs       = safe_cast<bool>(tData->GetValue((int)2));
		const bool export_normals   = safe_cast<bool>(tData->GetValue((int)3));
		const bool export_materials = safe_cast<bool>(tData->GetValue((int)4));

		const bool export_clutter      = safe_cast<bool>(tData->GetValue((int)5));
		const bool export_assets       = safe_cast<bool>(tData->GetValue((int)6));
		const bool export_prefabs      = safe_cast<bool>(tData->GetValue((int)7));
		const bool export_blueprints   = safe_cast<bool>(tData->GetValue((int)8));
		const bool export_harvestables = safe_cast<bool>(tData->GetValue((int)9));

		ConvertSettings::ExportUvs       = export_uvs;
		ConvertSettings::ExportNormals   = export_normals;
		ConvertSettings::ExportMaterials = export_materials && export_uvs;

		ConvertSettings::ExportClutter      = export_clutter;
		ConvertSettings::ExportAssets       = export_assets;
		ConvertSettings::ExportPrefabs      = export_prefabs;
		ConvertSettings::ExportBlueprints   = export_blueprints;
		ConvertSettings::ExportHarvestables = export_harvestables;

		const std::wstring tile_path_wstr = msclr::interop::marshal_as<std::wstring>(tile_path);
		const std::wstring tile_name_wstr = msclr::interop::marshal_as<std::wstring>(tile_name);

		ConvertError cError;
		TileConv::ConvertToModel(tile_path_wstr, tile_name_wstr, cError);

		System::Array^ result_data = nullptr;
		if (cError)
		{
			result_data = gcnew cli::array<System::Object^>(2);
			result_data->SetValue(true, (int)0);
			result_data->SetValue(gcnew System::String(cError.GetErrorMsg().c_str()), (int)1);
		}
		else
		{
			result_data = gcnew cli::array<System::Object^>(1);
			result_data->SetValue(false, (int)0);
		}
		
		e->Result = result_data;
	}

	void MainGui::TileConverter_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		System::Array^ result_array = safe_cast<System::Array^>(e->Result);

		bool has_error = safe_cast<bool>(result_array->GetValue((int)0));
		if (has_error)
		{
			System::String^ error_msg = safe_cast<System::String^>(result_array->GetValue((int)1));

			WForms::MessageBox::Show(
				error_msg,
				"Convert Error",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Error
			);
		}

		this->ChangeGuiState(true, false);
	}

	void MainGui::TilePathSelector_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		const std::wstring wstr_path = File::OpenFileDialog(
			L"Select Tile",
			0,
			L"All Files (*.*)\0*.*\0",
			static_cast<HWND>(this->Handle.ToPointer())
		);
		if (wstr_path.empty()) return;

		this->TilePath_TB->Text = gcnew System::String(wstr_path.c_str());
	}

	void MainGui::TS_About_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		AboutGui^ about_gui = gcnew AboutGui();
		about_gui->ShowDialog();
	}

	void MainGui::TS_Settings_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		SettingsGui^ settings_gui = gcnew SettingsGui();
		settings_gui->ShowDialog();

		if (*settings_gui->update_after_close)
		{
			DatabaseConfig::ReadConfig();

			this->LoadObjectDatabase();
		}
	}
}