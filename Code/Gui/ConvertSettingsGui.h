#pragma once

#include <string>

namespace TileConverter
{
	public ref class ConvertSettingsGui : public System::Windows::Forms::Form
	{
	public:
		bool CanConvert = false;
		ConvertSettingsGui(const std::wstring& tile_name);

	protected:
		~ConvertSettingsGui();

	public:
		System::Windows::Forms::TextBox^ OutputName_TB;
		
		System::Windows::Forms::CheckBox^ ExportUvs_CB;
		System::Windows::Forms::CheckBox^ ExportNormals_CB;
		System::Windows::Forms::CheckBox^ ExportMaterials_CB;

		System::Windows::Forms::CheckBox^ ExportClutter_CB;
		System::Windows::Forms::CheckBox^ ExportAssets_CB;
		System::Windows::Forms::CheckBox^ ExportPrefabs_CB;
		System::Windows::Forms::CheckBox^ ExportBlueprints_CB;
		System::Windows::Forms::CheckBox^ ExportHarvestables_CB;

	private:
		System::Windows::Forms::GroupBox^ ModelSettings_GB;
		System::Windows::Forms::GroupBox^ DataSettings_GB;
		System::Windows::Forms::GroupBox^ OutName_GB;
		System::Windows::Forms::Button^ Convert_BTN;

		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->OutName_GB = (gcnew System::Windows::Forms::GroupBox());
			this->OutputName_TB = (gcnew System::Windows::Forms::TextBox());
			this->ModelSettings_GB = (gcnew System::Windows::Forms::GroupBox());
			this->ExportHarvestables_CB = (gcnew System::Windows::Forms::CheckBox());
			this->ExportBlueprints_CB = (gcnew System::Windows::Forms::CheckBox());
			this->ExportPrefabs_CB = (gcnew System::Windows::Forms::CheckBox());
			this->ExportAssets_CB = (gcnew System::Windows::Forms::CheckBox());
			this->ExportClutter_CB = (gcnew System::Windows::Forms::CheckBox());
			this->DataSettings_GB = (gcnew System::Windows::Forms::GroupBox());
			this->ExportNormals_CB = (gcnew System::Windows::Forms::CheckBox());
			this->ExportMaterials_CB = (gcnew System::Windows::Forms::CheckBox());
			this->ExportUvs_CB = (gcnew System::Windows::Forms::CheckBox());
			this->Convert_BTN = (gcnew System::Windows::Forms::Button());
			this->OutName_GB->SuspendLayout();
			this->ModelSettings_GB->SuspendLayout();
			this->DataSettings_GB->SuspendLayout();
			this->SuspendLayout();
			// 
			// OutName_GB
			// 
			this->OutName_GB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->OutName_GB->Controls->Add(this->OutputName_TB);
			this->OutName_GB->Location = System::Drawing::Point(12, 12);
			this->OutName_GB->Name = L"OutName_GB";
			this->OutName_GB->Size = System::Drawing::Size(174, 48);
			this->OutName_GB->TabIndex = 0;
			this->OutName_GB->TabStop = false;
			this->OutName_GB->Text = L"OutputFileName";
			// 
			// OutputName_TB
			// 
			this->OutputName_TB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->OutputName_TB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->OutputName_TB->Location = System::Drawing::Point(6, 19);
			this->OutputName_TB->MaxLength = 64;
			this->OutputName_TB->Name = L"OutputName_TB";
			this->OutputName_TB->Size = System::Drawing::Size(162, 22);
			this->OutputName_TB->TabIndex = 0;
			this->OutputName_TB->TextChanged += gcnew System::EventHandler(this, &ConvertSettingsGui::OutputName_TB_TextChanged);
			this->OutputName_TB->KeyPress += gcnew System::Windows::Forms::KeyPressEventHandler(this, &ConvertSettingsGui::OutputName_TB_KeyPress);
			// 
			// ModelSettings_GB
			// 
			this->ModelSettings_GB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ModelSettings_GB->Controls->Add(this->ExportHarvestables_CB);
			this->ModelSettings_GB->Controls->Add(this->ExportBlueprints_CB);
			this->ModelSettings_GB->Controls->Add(this->ExportPrefabs_CB);
			this->ModelSettings_GB->Controls->Add(this->ExportAssets_CB);
			this->ModelSettings_GB->Controls->Add(this->ExportClutter_CB);
			this->ModelSettings_GB->Location = System::Drawing::Point(12, 66);
			this->ModelSettings_GB->Name = L"ModelSettings_GB";
			this->ModelSettings_GB->Size = System::Drawing::Size(174, 149);
			this->ModelSettings_GB->TabIndex = 1;
			this->ModelSettings_GB->TabStop = false;
			this->ModelSettings_GB->Text = L"Model Settings";
			// 
			// ExportHarvestables_CB
			// 
			this->ExportHarvestables_CB->AutoSize = true;
			this->ExportHarvestables_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ExportHarvestables_CB->Location = System::Drawing::Point(6, 123);
			this->ExportHarvestables_CB->Name = L"ExportHarvestables_CB";
			this->ExportHarvestables_CB->Size = System::Drawing::Size(148, 20);
			this->ExportHarvestables_CB->TabIndex = 4;
			this->ExportHarvestables_CB->Text = L"Export Harvestables";
			this->ExportHarvestables_CB->UseVisualStyleBackColor = true;
			// 
			// ExportBlueprints_CB
			// 
			this->ExportBlueprints_CB->AutoSize = true;
			this->ExportBlueprints_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ExportBlueprints_CB->Location = System::Drawing::Point(6, 97);
			this->ExportBlueprints_CB->Name = L"ExportBlueprints_CB";
			this->ExportBlueprints_CB->Size = System::Drawing::Size(126, 20);
			this->ExportBlueprints_CB->TabIndex = 3;
			this->ExportBlueprints_CB->Text = L"Export Blueprints";
			this->ExportBlueprints_CB->UseVisualStyleBackColor = true;
			// 
			// ExportPrefabs_CB
			// 
			this->ExportPrefabs_CB->AutoSize = true;
			this->ExportPrefabs_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ExportPrefabs_CB->Location = System::Drawing::Point(6, 71);
			this->ExportPrefabs_CB->Name = L"ExportPrefabs_CB";
			this->ExportPrefabs_CB->Size = System::Drawing::Size(114, 20);
			this->ExportPrefabs_CB->TabIndex = 2;
			this->ExportPrefabs_CB->Text = L"Export Prefabs";
			this->ExportPrefabs_CB->UseVisualStyleBackColor = true;
			// 
			// ExportAssets_CB
			// 
			this->ExportAssets_CB->AutoSize = true;
			this->ExportAssets_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ExportAssets_CB->Location = System::Drawing::Point(6, 45);
			this->ExportAssets_CB->Name = L"ExportAssets_CB";
			this->ExportAssets_CB->Size = System::Drawing::Size(108, 20);
			this->ExportAssets_CB->TabIndex = 1;
			this->ExportAssets_CB->Text = L"Export Assets";
			this->ExportAssets_CB->UseVisualStyleBackColor = true;
			// 
			// ExportClutter_CB
			// 
			this->ExportClutter_CB->AutoSize = true;
			this->ExportClutter_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ExportClutter_CB->Location = System::Drawing::Point(6, 19);
			this->ExportClutter_CB->Name = L"ExportClutter_CB";
			this->ExportClutter_CB->Size = System::Drawing::Size(104, 20);
			this->ExportClutter_CB->TabIndex = 0;
			this->ExportClutter_CB->Text = L"Export Clutter";
			this->ExportClutter_CB->UseVisualStyleBackColor = true;
			// 
			// DataSettings_GB
			// 
			this->DataSettings_GB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->DataSettings_GB->Controls->Add(this->ExportNormals_CB);
			this->DataSettings_GB->Controls->Add(this->ExportMaterials_CB);
			this->DataSettings_GB->Controls->Add(this->ExportUvs_CB);
			this->DataSettings_GB->Location = System::Drawing::Point(12, 221);
			this->DataSettings_GB->Name = L"DataSettings_GB";
			this->DataSettings_GB->Size = System::Drawing::Size(174, 97);
			this->DataSettings_GB->TabIndex = 2;
			this->DataSettings_GB->TabStop = false;
			this->DataSettings_GB->Text = L"Data Settings";
			// 
			// ExportNormals_CB
			// 
			this->ExportNormals_CB->AutoSize = true;
			this->ExportNormals_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ExportNormals_CB->Location = System::Drawing::Point(6, 71);
			this->ExportNormals_CB->Name = L"ExportNormals_CB";
			this->ExportNormals_CB->Size = System::Drawing::Size(118, 20);
			this->ExportNormals_CB->TabIndex = 2;
			this->ExportNormals_CB->Text = L"Export Normals";
			this->ExportNormals_CB->UseVisualStyleBackColor = true;
			// 
			// ExportMaterials_CB
			// 
			this->ExportMaterials_CB->AutoSize = true;
			this->ExportMaterials_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->ExportMaterials_CB->Location = System::Drawing::Point(6, 19);
			this->ExportMaterials_CB->Name = L"ExportMaterials_CB";
			this->ExportMaterials_CB->Size = System::Drawing::Size(122, 20);
			this->ExportMaterials_CB->TabIndex = 1;
			this->ExportMaterials_CB->Text = L"Export Materials";
			this->ExportMaterials_CB->UseVisualStyleBackColor = true;
			// 
			// ExportUvs_CB
			// 
			this->ExportUvs_CB->AutoSize = true;
			this->ExportUvs_CB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->ExportUvs_CB->Location = System::Drawing::Point(6, 45);
			this->ExportUvs_CB->Name = L"ExportUvs_CB";
			this->ExportUvs_CB->Size = System::Drawing::Size(162, 20);
			this->ExportUvs_CB->TabIndex = 0;
			this->ExportUvs_CB->Text = L"Export UV Coordinates";
			this->ExportUvs_CB->UseVisualStyleBackColor = true;
			this->ExportUvs_CB->CheckedChanged += gcnew System::EventHandler(this, &ConvertSettingsGui::ExportUvs_CB_CheckedChanged);
			// 
			// Convert_BTN
			// 
			this->Convert_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->Convert_BTN->Enabled = false;
			this->Convert_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Convert_BTN->Location = System::Drawing::Point(12, 324);
			this->Convert_BTN->Name = L"Convert_BTN";
			this->Convert_BTN->Size = System::Drawing::Size(174, 29);
			this->Convert_BTN->TabIndex = 3;
			this->Convert_BTN->Text = L"Convert";
			this->Convert_BTN->UseVisualStyleBackColor = true;
			this->Convert_BTN->Click += gcnew System::EventHandler(this, &ConvertSettingsGui::Convert_BTN_Click);
			// 
			// ConvertSettingsGui
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(198, 365);
			this->Controls->Add(this->Convert_BTN);
			this->Controls->Add(this->DataSettings_GB);
			this->Controls->Add(this->ModelSettings_GB);
			this->Controls->Add(this->OutName_GB);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"ConvertSettingsGui";
			this->ShowIcon = false;
			this->Text = L"Convert Settings";
			this->OutName_GB->ResumeLayout(false);
			this->OutName_GB->PerformLayout();
			this->ModelSettings_GB->ResumeLayout(false);
			this->ModelSettings_GB->PerformLayout();
			this->DataSettings_GB->ResumeLayout(false);
			this->DataSettings_GB->PerformLayout();
			this->ResumeLayout(false);

		}
#pragma endregion
		System::Void OutputName_TB_TextChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void Convert_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ExportUvs_CB_CheckedChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void OutputName_TB_KeyPress(System::Object^ sender, System::Windows::Forms::KeyPressEventArgs^ e);
	};
}
