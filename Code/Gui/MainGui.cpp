#include "MainGui.h"
#include "Console.hpp"

#include "ObjectDatabase/ObjectDatabase.hpp"
#include "ObjectDatabase/DatabaseConfig.hpp"
#include "ObjectDatabase/ProgCounter.hpp"
#include "ObjectDatabase/Mod/Mod.hpp"

#include "Gui/AboutGui.h"
#include "Gui/SettingsGui.h"
#include "Gui/ConvertSettingsGui.h"

#include "Tile/TileConverter.hpp"
#include "Utils/File.hpp"

#include <CommCtrl.h>
#include <msclr/marshal_cppstd.h>
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

		DebugOutL(0b0011_fg, "Loading object database...");

		this->ChangeGuiState(false, false);
		this->ProgressUpdater->Start();
		this->DatabaseLoader_BW->RunWorkerAsync();
	}

	void MainGui::DatabaseLoader_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
	{
		DatabaseLoader::LoadDatabase();
	}

	void MainGui::DatabaseLoader_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		this->ProgressUpdater->Stop();
		this->ChangeGuiState(true, false);

		ProgCounter::SetState(ProgState::None, 0);

		this->ConvertProgress_PB->Value = 0;
		this->ConvertProgress_PB->Maximum = 0;

		const std::size_t mod_count = Mod::GetAmountOfMods();
		const std::size_t obj_count = Mod::GetAmountOfObjects();
		const std::wstring load_msg = L"Successfully loaded " + std::to_wstring(obj_count) + L" objects from " + std::to_wstring(mod_count) + L" mods";
		this->Progress_LBL->Text = gcnew System::String(load_msg.c_str());
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

			System::Array^ thread_data = gcnew cli::array<System::Object^>(11);
			thread_data->SetValue(this->TilePath_TB->Text, (int)0);
			thread_data->SetValue(conv_settings->OutputName_TB->Text, (int)1);

			thread_data->SetValue(conv_settings->ExportUvs_CB->Checked,         (int)2);
			thread_data->SetValue(conv_settings->ExportNormals_CB->Checked,     (int)3);
			thread_data->SetValue(conv_settings->ExportMaterials_CB->Checked,   (int)4);
			thread_data->SetValue(conv_settings->ExportGndTextures_CB->Checked, (int)5);

			thread_data->SetValue(conv_settings->ExportClutter_CB->Checked,      (int)6);
			thread_data->SetValue(conv_settings->ExportAssets_CB->Checked,       (int)7);
			thread_data->SetValue(conv_settings->ExportPrefabs_CB->Checked,      (int)8);
			thread_data->SetValue(conv_settings->ExportBlueprints_CB->Checked,   (int)9);
			thread_data->SetValue(conv_settings->ExportHarvestables_CB->Checked, (int)10);

			this->ProgressUpdater->Start();
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
		const bool export_gnd_materials = safe_cast<bool>(tData->GetValue((int)5));

		const bool export_clutter      = safe_cast<bool>(tData->GetValue((int)6));
		const bool export_assets       = safe_cast<bool>(tData->GetValue((int)7));
		const bool export_prefabs      = safe_cast<bool>(tData->GetValue((int)8));
		const bool export_blueprints   = safe_cast<bool>(tData->GetValue((int)9));
		const bool export_harvestables = safe_cast<bool>(tData->GetValue((int)10));

		ConvertSettings::ExportUvs       = export_uvs;
		ConvertSettings::ExportNormals   = export_normals;
		ConvertSettings::ExportMaterials = export_materials && export_uvs;
		ConvertSettings::ExportGroundTextures = export_gnd_materials;

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
		this->ProgressUpdater->Stop();

		this->ProgressUpdater_Tick(nullptr, nullptr);

		bool has_error = safe_cast<bool>(result_array->GetValue((int)0));
		if (has_error)
		{
			System::String^ error_msg = safe_cast<System::String^>(result_array->GetValue((int)1));
			ProgCounter::SetState(ProgState::ConvertFailure, 0);

			WForms::MessageBox::Show(
				error_msg,
				"Convert Error",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Error
			);

		}
		else
		{
			ProgCounter::SetState(ProgState::ConvertSuccess, 0);

			WForms::MessageBox::Show(
				"Tile has been successfully converted!",
				"Convert Success",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Information
			);
		}

		this->ProgressUpdater_Tick(nullptr, nullptr);
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

		if (settings_gui->update_after_close)
		{
			DatabaseConfig::ReadConfig();

			this->LoadObjectDatabase();
		}
	}

	void MainGui::ProgressUpdater_Tick(System::Object^ sender, System::EventArgs^ e)
	{
		if (ProgCounter::State == ProgState::None) return;

		std::wstring state_output = ProgCounter::GetStateString();
		if (ProgCounter::StateHasNumbers())
		{
			std::size_t max_value = ProgCounter::ProgressMax;
			std::size_t cur_value = ProgCounter::ProgressValue;

			this->ConvertProgress_PB->Maximum = (int)max_value;
			
			std::size_t max_cast = this->ConvertProgress_PB->Maximum;
			if (max_cast < cur_value)
			{
				cur_value = max_cast;
			}

			this->ConvertProgress_PB->Value = (int)cur_value;

			state_output += (L"(" + std::to_wstring(cur_value) + L" / " + std::to_wstring(max_value) + L")");
		}

		this->Progress_LBL->Text = gcnew System::String(state_output.c_str());
	}

	void MainGui::ResetProgressBar()
	{

	}
}