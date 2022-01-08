#pragma once

namespace TileConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainGui
	/// </summary>
	public ref class MainGui : public System::Windows::Forms::Form
	{
	public:
		MainGui(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainGui()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^ Convert_BTN;
	protected:


	private: System::Windows::Forms::TextBox^ TilePath_TB;
	protected:




	private: System::Windows::Forms::MenuStrip^ menuStrip1;
	private: System::Windows::Forms::ToolStripMenuItem^ TS_About_BTN;

	private: System::Windows::Forms::ToolStripMenuItem^ optionsToolStripMenuItem;
	private: System::Windows::Forms::ToolStripMenuItem^ TS_ReloadDB_BTN;

	private: System::Windows::Forms::ToolStripMenuItem^ TS_Settings_BTN;
	private: System::ComponentModel::BackgroundWorker^ DatabaseLoader_BW;
	private: System::ComponentModel::BackgroundWorker^ TileConverter_BW;
	private: System::Windows::Forms::Button^ TilePathSelector_BTN;

	private: System::ComponentModel::IContainer^ components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->Convert_BTN = (gcnew System::Windows::Forms::Button());
			this->TilePath_TB = (gcnew System::Windows::Forms::TextBox());
			this->menuStrip1 = (gcnew System::Windows::Forms::MenuStrip());
			this->TS_About_BTN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->optionsToolStripMenuItem = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TS_ReloadDB_BTN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->TS_Settings_BTN = (gcnew System::Windows::Forms::ToolStripMenuItem());
			this->DatabaseLoader_BW = (gcnew System::ComponentModel::BackgroundWorker());
			this->TileConverter_BW = (gcnew System::ComponentModel::BackgroundWorker());
			this->TilePathSelector_BTN = (gcnew System::Windows::Forms::Button());
			this->menuStrip1->SuspendLayout();
			this->SuspendLayout();
			// 
			// Convert_BTN
			// 
			this->Convert_BTN->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->Convert_BTN->Enabled = false;
			this->Convert_BTN->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Convert_BTN->Location = System::Drawing::Point(180, 57);
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
			this->TilePath_TB->Size = System::Drawing::Size(222, 22);
			this->TilePath_TB->TabIndex = 2;
			this->TilePath_TB->TextChanged += gcnew System::EventHandler(this, &MainGui::TilePath_TB_TextChanged);
			// 
			// menuStrip1
			// 
			this->menuStrip1->Enabled = false;
			this->menuStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) { this->TS_About_BTN, this->optionsToolStripMenuItem });
			this->menuStrip1->Location = System::Drawing::Point(0, 0);
			this->menuStrip1->Name = L"menuStrip1";
			this->menuStrip1->Size = System::Drawing::Size(284, 24);
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
			this->optionsToolStripMenuItem->DropDownItems->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->TS_ReloadDB_BTN,
					this->TS_Settings_BTN
			});
			this->optionsToolStripMenuItem->Name = L"optionsToolStripMenuItem";
			this->optionsToolStripMenuItem->Size = System::Drawing::Size(61, 20);
			this->optionsToolStripMenuItem->Text = L"Options";
			// 
			// TS_ReloadDB_BTN
			// 
			this->TS_ReloadDB_BTN->Name = L"TS_ReloadDB_BTN";
			this->TS_ReloadDB_BTN->Size = System::Drawing::Size(232, 22);
			this->TS_ReloadDB_BTN->Text = L"Reload Object Database";
			this->TS_ReloadDB_BTN->Click += gcnew System::EventHandler(this, &MainGui::TS_ReloadDB_BTN_Click);
			// 
			// TS_Settings_BTN
			// 
			this->TS_Settings_BTN->Enabled = false;
			this->TS_Settings_BTN->Name = L"TS_Settings_BTN";
			this->TS_Settings_BTN->Size = System::Drawing::Size(232, 22);
			this->TS_Settings_BTN->Text = L"Settings (NOT IMPLEMENTED)";
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
			this->TilePathSelector_BTN->Location = System::Drawing::Point(240, 27);
			this->TilePathSelector_BTN->Name = L"TilePathSelector_BTN";
			this->TilePathSelector_BTN->Size = System::Drawing::Size(32, 22);
			this->TilePathSelector_BTN->TabIndex = 4;
			this->TilePathSelector_BTN->Text = L"...";
			this->TilePathSelector_BTN->UseVisualStyleBackColor = true;
			this->TilePathSelector_BTN->Click += gcnew System::EventHandler(this, &MainGui::TilePathSelector_BTN_Click);
			// 
			// MainGui
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(284, 103);
			this->Controls->Add(this->TilePathSelector_BTN);
			this->Controls->Add(this->TilePath_TB);
			this->Controls->Add(this->Convert_BTN);
			this->Controls->Add(this->menuStrip1);
			this->MainMenuStrip = this->menuStrip1;
			this->MinimumSize = System::Drawing::Size(300, 142);
			this->Name = L"MainGui";
			this->ShowIcon = false;
			this->Text = L"Tile Converter";
			this->Load += gcnew System::EventHandler(this, &MainGui::MainGui_Load);
			this->menuStrip1->ResumeLayout(false);
			this->menuStrip1->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void MainGui_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void LoadObjectDatabase();
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
	};
}
