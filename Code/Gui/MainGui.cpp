#include "MainGui.h"
#include "Console.hpp"

#include <CommCtrl.h>
#include <msclr/marshal_cppstd.h>

#include "ObjectDatabase/ObjectDatabase.hpp"
#include "Tile/TileConverter.hpp"
#include "Utils/File.hpp"

#include "Gui/AboutGui.h"

#include <filesystem>
namespace fs = std::filesystem;

namespace WForms = System::Windows::Forms;

namespace TileConverter
{
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

		this->LoadObjectDatabase();
	}

	void MainGui::LoadObjectDatabase()
	{
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

		this->ChangeGuiState(true, true);

		System::Array^ thread_data = gcnew cli::array<System::Object^>(1);
		thread_data->SetValue(this->TilePath_TB->Text, (int)0);

		this->TileConverter_BW->RunWorkerAsync(thread_data);
	}

	void MainGui::TileConverter_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
	{
		System::Array^ tData = safe_cast<System::Array^>(e->Argument);
		System::String^ bp_path = safe_cast<System::String^>(tData->GetValue((int)0));

		const std::wstring bp_path_wstr = msclr::interop::marshal_as<std::wstring>(bp_path);

		DebugOutL("Converting: ", bp_path_wstr);
		TileConv::ConvertToModel(bp_path_wstr);
	}

	void MainGui::TileConverter_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		this->ChangeGuiState(true, false);
	}

	void MainGui::TilePathSelector_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		const std::wstring wstr_path = File::OpenFileDialog(L"Select Tile");
		if (wstr_path.empty()) return;

		this->TilePath_TB->Text = gcnew System::String(wstr_path.c_str());
	}

	void MainGui::TS_About_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		AboutGui^ about_gui = gcnew AboutGui();
		about_gui->ShowDialog();
	}
}