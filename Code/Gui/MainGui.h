#pragma once

namespace TileConverter
{
	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public ref class MainGui : public System::Windows::Forms::Form
	{
	public:
		MainGui();

	protected:
		~MainGui();

	private:
		System::Windows::Forms::Button^ Convert_BTN;
		System::Windows::Forms::TextBox^ TilePath_TB;
		System::Windows::Forms::MenuStrip^ menuStrip1;
		System::Windows::Forms::ToolStripMenuItem^ TS_About_BTN;
		System::Windows::Forms::ToolStripMenuItem^ optionsToolStripMenuItem;
		System::Windows::Forms::ToolStripMenuItem^ TS_ReloadDB_BTN;
		System::Windows::Forms::ToolStripMenuItem^ TS_Settings_BTN;
		System::ComponentModel::BackgroundWorker^ DatabaseLoader_BW;
		System::ComponentModel::BackgroundWorker^ TileConverter_BW;
		System::Windows::Forms::Button^ TilePathSelector_BTN;
		System::Windows::Forms::ToolStripSeparator^ toolStripSeparator1;
	private: System::Windows::Forms::ProgressBar^ ConvertProgress_PB;
	private: System::Windows::Forms::Label^ Progress_LBL;
	private: System::Windows::Forms::Timer^ ProgressUpdater;
	private: System::Windows::Forms::ComboBox^ CB_SelectedGeneratorType;
	private: System::Windows::Forms::Label^ label1;

		System::ComponentModel::IContainer^ components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->Convert_BTN = (gcnew System::Windows::Forms::Button());
			this->TilePath_TB = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->TS_About_BTN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TS_ReloadDB_BTN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->toolStripSeparator1 = (gcnew System::Windows::Forms::ToolStripSeparator());
			this->TS_Settings_BTN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DatabaseLoader_BW = (gcnew System::ComponentModel::BackgroundWorker());
			this->TileConverter_BW = (gcnew System::ComponentModel::BackgroundWorker());
			this->TilePathSelector_BTN = (gcnew System::Windows::Forms::Button());
			this->ConvertProgress_PB = (gcnew System::Windows::Forms::ProgressBar());
			this->Progress_LBL = (gcnew System::Windows::Forms::Label());
			this->ProgressUpdater = (gcnew System::Windows::Forms::Timer(this->components));
			this->CB_SelectedGeneratorType = (gcnew System::Windows::Forms::ComboBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// Convert_BTN
			// 
			this->Convert_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->Convert_BTN->Enabled = false;
			this->Convert_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Convert_BTN->Location = System::Drawing::Point(310, 101);
			this->Convert_BTN->Name = L"Convert_BTN";
			this->Convert_BTN->Size = System::Drawing::Size(92, 36);
			this->Convert_BTN->TabIndex = 0;
			this->Convert_BTN->Text = L"Convert";
			this->Convert_BTN->UseVisualStyleBackColor = true;
			this->Convert_BTN->Click += gcnew System::EventHandler(this, &MainGui::Convert_BTN_Click);
			// 
			// TilePath_TB
			// 
			this->TilePath_TB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->TilePath_TB->Enabled = false;
			this->TilePath_TB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->TilePath_TB->Location = System::Drawing::Point(12, 27);
			this->TilePath_TB->MaxLength = 512;
			this->TilePath_TB->Name = L"TilePath_TB";
			this->TilePath_TB->Size = System::Drawing::Size(352, 22);
			this->TilePath_TB->TabIndex = 2;
			this->TilePath_TB->TextChanged += gcnew System::EventHandler(this, &MainGui::TilePath_TB_TextChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Enabled = false;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->TS_About_BTN, this->optionsToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(414, 24);
			this->menuStrip1->TabIndex = 3;
			this->menuStrip1->Text = L"menuStrip1";
			// 
			// TS_About_BTN
			// 
			this->TS_About_BTN->Name = L"TS_About_BTN";
			this->TS_About_BTN->Size = System::Drawing::Size(52, 20);
			this->TS_About_BTN->Text = L"About";
			this->TS_About_BTN->Click += gcnew System::EventHandler(this, &MainGui::TS_About_BTN_Click);
			// 
			// optionsToolStripMenuItem
			// 
			this->optionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(3) {
				this->TS_ReloadDB_BTN,
					this->toolStripSeparator1, this->TS_Settings_BTN
			});
			this->optionsToolStripMenuItem->Name = L"optionsToolStripMenuItem";
			this->optionsToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->optionsToolStripMenuItem->Text = L"Options";
			// 
			// TS_ReloadDB_BTN
			// 
			this->TS_ReloadDB_BTN->Name = L"TS_ReloadDB_BTN";
			this->TS_ReloadDB_BTN->Size = System::Drawing::Size(199, 22);
			this->TS_ReloadDB_BTN->Text = L"Reload Object Database";
			this->TS_ReloadDB_BTN->Click += gcnew System::EventHandler(this, &MainGui::TS_ReloadDB_BTN_Click);
			// 
			// toolStripSeparator1
			// 
			this->toolStripSeparator1->Name = L"toolStripSeparator1";
			this->toolStripSeparator1->Size = System::Drawing::Size(196, 6);
			// 
			// TS_Settings_BTN
			// 
			this->TS_Settings_BTN->Name = L"TS_Settings_BTN";
			this->TS_Settings_BTN->Size = System::Drawing::Size(199, 22);
			this->TS_Settings_BTN->Text = L"Settings";
			this->TS_Settings_BTN->Click += gcnew System::EventHandler(this, &MainGui::TS_Settings_BTN_Click);
			// 
			// DatabaseLoader_BW
			// 
			this->DatabaseLoader_BW->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainGui::DatabaseLoader_BW_DoWork);
			this->DatabaseLoader_BW->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MainGui::DatabaseLoader_BW_RunWorkerCompleted);
			// 
			// TileConverter_BW
			// 
			this->TileConverter_BW->DoWork += gcnew System::ComponentModel::DoWorkEventHandler(this, &MainGui::TileConverter_BW_DoWork);
			this->TileConverter_BW->RunWorkerCompleted += gcnew System::ComponentModel::RunWorkerCompletedEventHandler(this, &MainGui::TileConverter_BW_RunWorkerCompleted);
			// 
			// TilePathSelector_BTN
			// 
			this->TilePathSelector_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Right));
			this->TilePathSelector_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->TilePathSelector_BTN->Location = System::Drawing::Point(370, 27);
			this->TilePathSelector_BTN->Name = L"TilePathSelector_BTN";
			this->TilePathSelector_BTN->Size = System::Drawing::Size(32, 22);
			this->TilePathSelector_BTN->TabIndex = 4;
			this->TilePathSelector_BTN->Text = L"...";
			this->TilePathSelector_BTN->UseVisualStyleBackColor = true;
			this->TilePathSelector_BTN->Click += gcnew System::EventHandler(this, &MainGui::TilePathSelector_BTN_Click);
			// 
			// ConvertProgress_PB
			// 
			this->ConvertProgress_PB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->ConvertProgress_PB->Location = System::Drawing::Point(12, 119);
			this->ConvertProgress_PB->Name = L"ConvertProgress_PB";
			this->ConvertProgress_PB->Size = System::Drawing::Size(292, 18);
			this->ConvertProgress_PB->TabIndex = 5;
			// 
			// Progress_LBL
			// 
			this->Progress_LBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->Progress_LBL->AutoSize = true;
			this->Progress_LBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Progress_LBL->Location = System::Drawing::Point(9, 100);
			this->Progress_LBL->Name = L"Progress_LBL";
			this->Progress_LBL->Size = System::Drawing::Size(100, 16);
			this->Progress_LBL->TabIndex = 6;
			this->Progress_LBL->Text = L"ProgressOutput";
			// 
			// ProgressUpdater
			// 
			this->ProgressUpdater->Interval = 50;
			this->ProgressUpdater->Tick += gcnew System::EventHandler(this, &MainGui::ProgressUpdater_Tick);
			// 
			// CB_SelectedGeneratorType
			// 
			this->CB_SelectedGeneratorType->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->CB_SelectedGeneratorType->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->CB_SelectedGeneratorType->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->CB_SelectedGeneratorType->FormattingEnabled = true;
			this->CB_SelectedGeneratorType->Items->AddRange(gcnew cli::array< System::Object^  >(2) { L"Generate from .tile file", L"Generate from .lua script" });
			this->CB_SelectedGeneratorType->Location = System::Drawing::Point(12, 71);
			this->CB_SelectedGeneratorType->Name = L"CB_SelectedGeneratorType";
			this->CB_SelectedGeneratorType->Size = System::Drawing::Size(390, 24);
			this->CB_SelectedGeneratorType->TabIndex = 7;
			this->CB_SelectedGeneratorType->SelectedIndexChanged += gcnew System::EventHandler(this, &MainGui::CB_SelectedGeneratorType_SelectedIndexChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F));
			this->label1->Location = System::Drawing::Point(9, 52);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(102, 16);
			this->label1->TabIndex = 8;
			this->label1->Text = L"Generator Type";
			// 
			// MainGui
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(414, 147);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->CB_SelectedGeneratorType);
			this->Controls->Add(this->Progress_LBL);
			this->Controls->Add(this->ConvertProgress_PB);
			this->Controls->Add(this->TilePathSelector_BTN);
			this->Controls->Add(this->TilePath_TB);
			this->Controls->Add(this->Convert_BTN);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MinimumSize = System::Drawing::Size(430, 186);
			this->Name = L"MainGui";
			this->ShowIcon = false;
			this->Text = L"Tile Converter";
			this->Shown += gcnew System::EventHandler(this, &MainGui::MainGui_Shown);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void MainGui_Shown(System::Object^ sender, System::EventArgs^ e);
		System::Void LoadObjectDatabase(const bool& v_should_reload);
		System::Void DatabaseLoader_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		System::Void DatabaseLoader_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);
		System::Void TS_ReloadDB_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ChangeGuiState(const bool& db_loaded, const bool& tile_gen);
		System::Void TilePath_TB_TextChanged(System::Object^ sender, System::EventArgs^ e);
		System::Void Convert_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void TileConverter_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e);
		System::Void TileConverter_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e);
		System::Void TilePathSelector_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void TS_About_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void TS_Settings_BTN_Click(System::Object^ sender, System::EventArgs^ e);
		System::Void ProgressUpdater_Tick(System::Object^ sender, System::EventArgs^ e);
		System::Void CB_SelectedGeneratorType_SelectedIndexChanged(System::Object^ sender, System::EventArgs^ e);
	};
}
