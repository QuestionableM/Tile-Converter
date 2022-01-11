#include "SettingsGui.h"

#include "ObjectDatabase/DatabaseConfig.hpp"
#include "Utils/File.hpp"
#include "Utils/String.hpp"

#include <msclr/marshal_cppstd.h>
#include <filesystem>
namespace fs = std::filesystem;
namespace WForms = System::Windows::Forms;

#include "Console.hpp"

namespace TileConverter
{
	SaveSettings::SaveSettings(const std::wstring& game_path, const std::vector<std::wstring>& mod_folders)
	{
		this->GamePath = String::ToLower(game_path);

		this->ModFolders.reserve(mod_folders.size());
		for (const std::wstring& mFolder : mod_folders)
		{
			this->ModFolders.push_back(String::ToLower(mFolder));
		}
	}



	SettingsGui::SettingsGui()
	{
		this->InitializeComponent();

		this->old_settings = new SaveSettings(DatabaseConfig::GamePath, DatabaseConfig::ModFolders);
		this->new_settings = new SaveSettings(DatabaseConfig::GamePath, DatabaseConfig::ModFolders);
	}

	SettingsGui::~SettingsGui()
	{
		if (components) delete components;

		delete this->old_settings;
		delete this->new_settings;
	}

	void SettingsGui::SettingsGui_Load(System::Object^ sender, System::EventArgs^ e)
	{
		this->SM_Path_TB->Text = gcnew System::String(DatabaseConfig::GamePath.c_str());

		{
			this->ModDirPaths_LB->BeginUpdate();

			for (const std::wstring& pModDir : old_settings->ModFolders)
				this->ModDirPaths_LB->Items->Add(gcnew System::String(pModDir.c_str()));

			this->ModDirPaths_LB->EndUpdate();
		}

		this->SM_Path_TB->TextChanged += gcnew System::EventHandler(this, &SettingsGui::SM_Path_TB_TextChanged);
	}

	void SettingsGui::SM_BrowsePath_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		const std::wstring file_path = File::OpenFileDialog(
            L"Select Scrap Mechanic folder",
			FOS_PICKFOLDERS | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST,
            L"All Files (*.*)\0*.*\0",
            static_cast<HWND>(this->Handle.ToPointer())
        );

		if (file_path.empty()) return;

		this->SM_Path_TB->Text = gcnew System::String(file_path.c_str());
	}

	void SettingsGui::SM_Path_TB_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		std::wstring new_path = msclr::interop::marshal_as<std::wstring>(this->SM_Path_TB->Text);
		new_path = String::ToLower(new_path);

		this->sm_path_changed = (old_settings->GamePath != new_path);
		if (this->sm_path_changed)
		{
			new_settings->GamePath = new_path;
		}

		this->UpdateSaveSettings();
	}

	void SettingsGui::UpdateSaveSettings()
	{
		this->Save_BTN->Enabled = (this->sm_path_changed || this->mod_paths_changed);
	}

	void SettingsGui::ApplyGamePathChanges()
	{
		if (!this->sm_path_changed) return;

		if (File::Exists(new_settings->GamePath))
		{
			if (!File::Equivalent(old_settings->GamePath, new_settings->GamePath))
			{
				DatabaseConfig::GamePath = new_settings->GamePath;
				old_settings->GamePath = new_settings->GamePath;

				return;
			}
		}
		else
		{
			this->sm_path_changed   = false;
			this->mod_paths_changed = false;

			WForms::MessageBox::Show(
				"The specified path to the game doesn't exist!",
				"Invalid Game Path",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);
		}

		this->SM_Path_TB->Text = gcnew System::String(old_settings->GamePath.c_str());
		new_settings->GamePath = old_settings->GamePath;
	}

	void SettingsGui::ApplyModPathsChanges()
	{
		if (!this->mod_paths_changed) return;

		std::wstring error_msg = L"Mod Directory ";
		std::vector<std::size_t> path_ids = {};

		for (std::size_t a = 0; a < new_settings->ModFolders.size(); a++)
		{
			const std::wstring& cur_path = new_settings->ModFolders[a];

			if (!File::Exists(cur_path))
				path_ids.push_back(a + 1);
		}

		if (!path_ids.empty())
		{
			const std::size_t path_ids_sz = path_ids.size() - 1;
			for (std::size_t a = 0; a < path_ids.size(); a++)
			{
				if (a > 0)
				{
					if (a == (path_ids.size() - 1))
						error_msg.append(L" and ");
					else
						error_msg.append(L", ");
				}

				error_msg.append(std::to_wstring(path_ids[a]));
			}

			error_msg.append(L" is invalid!");

			WForms::MessageBox::Show(
				gcnew System::String(error_msg.c_str()),
				"Invalid Paths",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);

			this->sm_path_changed   = false;
			this->mod_paths_changed = false;
		}
		else
		{
			DatabaseConfig::ModFolders = new_settings->ModFolders;
			old_settings->ModFolders   = new_settings->ModFolders;
		}
	}

	void SettingsGui::Save_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		this->ApplyGamePathChanges();
		this->ApplyModPathsChanges();

		if (this->sm_path_changed || this->mod_paths_changed)
		{
			this->sm_path_changed = false;
			this->mod_paths_changed = false;

			DatabaseConfig::SaveConfig();

			this->update_after_close = true;
		}

		this->UpdateSaveSettings();
	}

	void SettingsGui::BrowseModPath_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		const std::wstring file_path = File::OpenFileDialog(
			L"Select a Mod Directory",
			FOS_PICKFOLDERS | FOS_FILEMUSTEXIST | FOS_PATHMUSTEXIST,
			L"All Files (*.*)\0*.*\0",
			static_cast<HWND>(this->Handle.ToPointer())
		);

		if (file_path.empty()) return;

		this->ModDirPath_TB->Text = gcnew System::String(file_path.c_str());
	}

	void SettingsGui::ModDirPath_TB_TextChanged(System::Object^ sender, System::EventArgs^ e)
	{
		this->AddModPath_BTN->Enabled = (this->ModDirPath_TB->TextLength > 0);
	}

	void SettingsGui::AddModPath_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		std::wstring mod_path = msclr::interop::marshal_as<std::wstring>(this->ModDirPath_TB->Text);
		mod_path = String::ToLower(mod_path);
		
		std::error_code ec;

		if (!(fs::exists(mod_path, ec) && !ec))
		{
			WForms::MessageBox::Show(
				"The specified mod directory does not exist!",
				"Invalid Path",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);

			return;
		}

		if (!(fs::is_directory(mod_path, ec) && !ec))
		{
			WForms::MessageBox::Show(
				"The specified path does not lead to a directory!",
				"Invalid Path",
				WForms::MessageBoxButtons::OK,
				WForms::MessageBoxIcon::Warning
			);

			return;
		}

		for (const std::wstring& fPath : new_settings->ModFolders)
		{
			if (fPath == mod_path || File::Equivalent(fPath, mod_path))
			{
				WForms::MessageBox::Show(
					"The specified path is already in the list!",
					"Invalid Path",
					WForms::MessageBoxButtons::OK,
					WForms::MessageBoxIcon::Warning
				);

				return;
			}
		}

		new_settings->ModFolders.push_back(mod_path);
		this->ModDirPaths_LB->Items->Add(gcnew System::String(mod_path.c_str()));
		this->ModDirPath_TB->Clear();

		this->mod_paths_changed = this->IsModPathsChanged();
		this->UpdateSaveSettings();
	}

	void SettingsGui::ModDirPaths_LB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e)
	{
		const bool has_items = (this->ModDirPaths_LB->Items->Count > 0);
		const bool is_selected = (this->ModDirPaths_LB->SelectedIndex > -1);

		this->RemoveSelected_BTN->Enabled = (has_items && is_selected);
	}

	void SettingsGui::RemoveSelected_BTN_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (this->ModDirPaths_LB->Items->Count <= 0) return;
		if (this->ModDirPaths_LB->SelectedIndex <= -1) return;

		const int sel_idx = this->ModDirPaths_LB->SelectedIndex;

		this->ModDirPaths_LB->Items->RemoveAt(sel_idx);
		new_settings->ModFolders.erase(new_settings->ModFolders.begin() + sel_idx);

		if (this->ModDirPaths_LB->Items->Count > 0)
		{
			const bool is_last = (this->ModDirPaths_LB->Items->Count == sel_idx);

			this->ModDirPaths_LB->SelectedIndex = (is_last ? sel_idx - 1 : sel_idx);
		}

		this->mod_paths_changed = this->IsModPathsChanged();
		this->UpdateSaveSettings();
	}

	bool SettingsGui::IsModPathsChanged()
	{
		if (old_settings->ModFolders.size() != new_settings->ModFolders.size()) return true;

		for (std::size_t a = 0; a < old_settings->ModFolders.size(); a++)
		{
			const std::wstring& new_path = new_settings->ModFolders[a];
			const std::wstring& old_path = old_settings->ModFolders[a];

			if (new_path != old_path || !File::Equivalent(new_path, old_path))
				return true;
		}

		return false;
	}

	void SettingsGui::SettingsGui_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e)
	{
		if (this->Save_BTN->Enabled)
		{
			WForms::DialogResult dr = WForms::MessageBox::Show(
				"Are you sure that you want to leave without saving?\n\nAll unsaved changes will be lost!",
				"Unsaved Data",
				WForms::MessageBoxButtons::YesNo,
				WForms::MessageBoxIcon::Question
			);

			if (dr == WForms::DialogResult::No)
				e->Cancel = true;
		}
	}
}