#pragma once

#include <string>
#include <vector>

namespace TileConverter
{
	struct SaveSettings
	{
		std::wstring GamePath;
		std::vector<std::wstring> ModFolders;

		SaveSettings(const std::wstring& game_path, const std::vector<std::wstring>& mod_folders);
	};

	public ref class SettingsGui : public System::Windows::Forms::Form
	{
		SaveSettings* old_settings = nullptr;
		SaveSettings* new_settings = nullptr;

		bool* sm_path_changed = nullptr;
		bool* mod_paths_changed = nullptr;

	public:
		SettingsGui();
		bool* update_after_close = nullptr;

	protected:
		~SettingsGui();

	private:
		System::Windows::Forms::Button^ Save_BTN;
		System::Windows::Forms::TabControl^ SettingsTabs_TC;
		System::Windows::Forms::TabPage^ General_Tab;
		System::Windows::Forms::Button^ SM_BrowsePath_BTN;
		System::Windows::Forms::TextBox^ SM_Path_TB;
		System::Windows::Forms::TabPage^ WS_Dirs_Tab;
		System::Windows::Forms::Label^ label1;

		System::Windows::Forms::Button^ RemoveSelected_BTN;
		System::Windows::Forms::Button^ AddModPath_BTN;
		System::Windows::Forms::Button^ BrowseModPath_BTN;
		System::Windows::Forms::TextBox^ ModDirPath_TB;
		System::Windows::Forms::Label^ label2;
		System::Windows::Forms::ListBox^ ModDirPaths_LB;

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->Save_BTN = (gcnew System::Windows::Forms::Button());
			this->SettingsTabs_TC = (gcnew System::Windows::Forms::TabControl());
			this->General_Tab = (gcnew System::Windows::Forms::TabPage());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->SM_BrowsePath_BTN = (gcnew System::Windows::Forms::Button());
			this->SM_Path_TB = (gcnew System::Windows::Forms::TextBox());
			this->WS_Dirs_Tab = (gcnew System::Windows::Forms::TabPage());
			this->RemoveSelected_BTN = (gcnew System::Windows::Forms::Button());
			this->AddModPath_BTN = (gcnew System::Windows::Forms::Button());
			this->BrowseModPath_BTN = (gcnew System::Windows::Forms::Button());
			this->ModDirPath_TB = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->ModDirPaths_LB = (gcnew System::Windows::Forms::ListBox());
			this->SettingsTabs_TC->SuspendLayout();
			this->General_Tab->SuspendLayout();
			this->WS_Dirs_Tab->SuspendLayout();
			this->SuspendLayout();
			// 
			// Save_BTN
			// 
			this->Save_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->Save_BTN->Enabled = false;
			this->Save_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Save_BTN->Location = System::Drawing::Point(261, 222);
			this->Save_BTN->Name = L"Save_BTN";
			this->Save_BTN->Size = System::Drawing::Size(111, 28);
			this->Save_BTN->TabIndex = 0;
			this->Save_BTN->Text = L"Save Changes";
			this->Save_BTN->UseVisualStyleBackColor = true;
			this->Save_BTN->Click += gcnew System::EventHandler(this, &SettingsGui::Save_BTN_Click);
			// 
			// SettingsTabs_TC
			// 
			this->SettingsTabs_TC->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->SettingsTabs_TC->Controls->Add(this->General_Tab);
			this->SettingsTabs_TC->Controls->Add(this->WS_Dirs_Tab);
			this->SettingsTabs_TC->Location = System::Drawing::Point(12, 12);
			this->SettingsTabs_TC->Name = L"SettingsTabs_TC";
			this->SettingsTabs_TC->SelectedIndex = 0;
			this->SettingsTabs_TC->Size = System::Drawing::Size(360, 204);
			this->SettingsTabs_TC->TabIndex = 1;
			// 
			// General_Tab
			// 
			this->General_Tab->Controls->Add(this->label1);
			this->General_Tab->Controls->Add(this->SM_BrowsePath_BTN);
			this->General_Tab->Controls->Add(this->SM_Path_TB);
			this->General_Tab->Location = System::Drawing::Point(4, 22);
			this->General_Tab->Name = L"General_Tab";
			this->General_Tab->Padding = System::Windows::Forms::Padding(3);
			this->General_Tab->Size = System::Drawing::Size(352, 178);
			this->General_Tab->TabIndex = 0;
			this->General_Tab->Text = L"General";
			this->General_Tab->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label1->Location = System::Drawing::Point(3, 3);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(151, 16);
			this->label1->TabIndex = 2;
			this->label1->Text = L"Path to Scrap Mechanic:";
			// 
			// SM_BrowsePath_BTN
			// 
			this->SM_BrowsePath_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->SM_BrowsePath_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->SM_BrowsePath_BTN->Location = System::Drawing::Point(308, 22);
			this->SM_BrowsePath_BTN->Name = L"SM_BrowsePath_BTN";
			this->SM_BrowsePath_BTN->Size = System::Drawing::Size(38, 22);
			this->SM_BrowsePath_BTN->TabIndex = 1;
			this->SM_BrowsePath_BTN->Text = L"...";
			this->SM_BrowsePath_BTN->UseVisualStyleBackColor = true;
			this->SM_BrowsePath_BTN->Click += gcnew System::EventHandler(this, &SettingsGui::SM_BrowsePath_BTN_Click);
			// 
			// SM_Path_TB
			// 
			this->SM_Path_TB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->SM_Path_TB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->SM_Path_TB->Location = System::Drawing::Point(6, 22);
			this->SM_Path_TB->MaxLength = 1024;
			this->SM_Path_TB->Name = L"SM_Path_TB";
			this->SM_Path_TB->Size = System::Drawing::Size(296, 22);
			this->SM_Path_TB->TabIndex = 0;
			// 
			// WS_Dirs_Tab
			// 
			this->WS_Dirs_Tab->Controls->Add(this->RemoveSelected_BTN);
			this->WS_Dirs_Tab->Controls->Add(this->AddModPath_BTN);
			this->WS_Dirs_Tab->Controls->Add(this->BrowseModPath_BTN);
			this->WS_Dirs_Tab->Controls->Add(this->ModDirPath_TB);
			this->WS_Dirs_Tab->Controls->Add(this->label2);
			this->WS_Dirs_Tab->Controls->Add(this->ModDirPaths_LB);
			this->WS_Dirs_Tab->Location = System::Drawing::Point(4, 22);
			this->WS_Dirs_Tab->Name = L"WS_Dirs_Tab";
			this->WS_Dirs_Tab->Padding = System::Windows::Forms::Padding(3);
			this->WS_Dirs_Tab->Size = System::Drawing::Size(358, 177);
			this->WS_Dirs_Tab->TabIndex = 1;
			this->WS_Dirs_Tab->Text = L"Mod Directories";
			this->WS_Dirs_Tab->UseVisualStyleBackColor = true;
			// 
			// RemoveSelected_BTN
			// 
			this->RemoveSelected_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->RemoveSelected_BTN->Enabled = false;
			this->RemoveSelected_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->RemoveSelected_BTN->Location = System::Drawing::Point(277, 99);
			this->RemoveSelected_BTN->Name = L"RemoveSelected_BTN";
			this->RemoveSelected_BTN->Size = System::Drawing::Size(75, 44);
			this->RemoveSelected_BTN->TabIndex = 5;
			this->RemoveSelected_BTN->Text = L"Remove Selected";
			this->RemoveSelected_BTN->UseVisualStyleBackColor = true;
			this->RemoveSelected_BTN->Click += gcnew System::EventHandler(this, &SettingsGui::RemoveSelected_BTN_Click);
			// 
			// AddModPath_BTN
			// 
			this->AddModPath_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->AddModPath_BTN->Enabled = false;
			this->AddModPath_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AddModPath_BTN->Location = System::Drawing::Point(300, 149);
			this->AddModPath_BTN->Name = L"AddModPath_BTN";
			this->AddModPath_BTN->Size = System::Drawing::Size(52, 22);
			this->AddModPath_BTN->TabIndex = 4;
			this->AddModPath_BTN->Text = L"Add";
			this->AddModPath_BTN->UseVisualStyleBackColor = true;
			this->AddModPath_BTN->Click += gcnew System::EventHandler(this, &SettingsGui::AddModPath_BTN_Click);
			// 
			// BrowseModPath_BTN
			// 
			this->BrowseModPath_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->BrowseModPath_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->BrowseModPath_BTN->Location = System::Drawing::Point(251, 149);
			this->BrowseModPath_BTN->Name = L"BrowseModPath_BTN";
			this->BrowseModPath_BTN->Size = System::Drawing::Size(43, 22);
			this->BrowseModPath_BTN->TabIndex = 3;
			this->BrowseModPath_BTN->Text = L"...";
			this->BrowseModPath_BTN->UseVisualStyleBackColor = true;
			this->BrowseModPath_BTN->Click += gcnew System::EventHandler(this, &SettingsGui::BrowseModPath_BTN_Click);
			// 
			// ModDirPath_TB
			// 
			this->ModDirPath_TB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ModDirPath_TB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ModDirPath_TB->Location = System::Drawing::Point(6, 149);
			this->ModDirPath_TB->MaxLength = 1024;
			this->ModDirPath_TB->Name = L"ModDirPath_TB";
			this->ModDirPath_TB->Size = System::Drawing::Size(239, 22);
			this->ModDirPath_TB->TabIndex = 2;
			this->ModDirPath_TB->TextChanged += gcnew System::EventHandler(this, &SettingsGui::ModDirPath_TB_TextChanged);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(3, 3);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(156, 16);
			this->label2->TabIndex = 1;
			this->label2->Text = L"Paths to Mod Directories:";
			// 
			// ModDirPaths_LB
			// 
			this->ModDirPaths_LB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Bottom)
				| System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ModDirPaths_LB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ModDirPaths_LB->FormattingEnabled = true;
			this->ModDirPaths_LB->IntegralHeight = false;
			this->ModDirPaths_LB->ItemHeight = 16;
			this->ModDirPaths_LB->Location = System::Drawing::Point(6, 22);
			this->ModDirPaths_LB->Name = L"ModDirPaths_LB";
			this->ModDirPaths_LB->Size = System::Drawing::Size(265, 121);
			this->ModDirPaths_LB->TabIndex = 0;
			this->ModDirPaths_LB->SelectedIndexChanged += gcnew System::EventHandler(this, &SettingsGui::ModDirPaths_LB_SelectedIndexChanged);
			// 
			// SettingsGui
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(384, 261);
			this->Controls->Add(this->SettingsTabs_TC);
			this->Controls->Add(this->Save_BTN);
			this->MinimumSize = System::Drawing::Size(400, 300);
			this->Name = L"SettingsGui";
			this->ShowIcon = false;
			this->Text = L"Settings";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SettingsGui::SettingsGui_FormClosing);
			this->Load += gcnew System::EventHandler(this, &SettingsGui::SettingsGui_Load);
			this->SettingsTabs_TC->ResumeLayout(false);
			this->General_Tab->ResumeLayout(false);
			this->General_Tab->PerformLayout();
			this->WS_Dirs_Tab->ResumeLayout(false);
			this->WS_Dirs_Tab->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
		System::Void SettingsGui_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void SM_BrowsePath_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void SM_Path_TB_TextChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void UpdateSaveSettings();
		System::Void ApplyGamePathChanges();
		System::Void ApplyModPathsChanges();
		System::Void Save_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void BrowseModPath_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ModDirPath_TB_TextChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void AddModPath_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ModDirPaths_LB_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void RemoveSelected_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Boolean IsModPathsChanged();
		System::Void SettingsGui_FormClosing(System::Object^ sender, System::Windows::Forms::FormClosingEventArgs^ e);
	};
}