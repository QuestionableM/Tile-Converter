#pragma once

namespace TileConverter {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for AboutGui
	/// </summary>
	public ref class AboutGui : public System::Windows::Forms::Form
	{
	public:
		AboutGui();

	protected:
		~AboutGui();

	private:
		System::Windows::Forms::TextBox^ AppDesc_TB;
		System::Windows::Forms::LinkLabel^ CreatorQM_LLBL;
		System::Windows::Forms::Label^ Creators_LBL;
		System::Windows::Forms::LinkLabel^ CreatorHardcoded_LLBL;
		System::Windows::Forms::LinkLabel^ LibAssimp_LLBL;
		System::Windows::Forms::LinkLabel^ LibLodePNG_LLBL;
		System::Windows::Forms::LinkLabel^ LibLZ4_LLBL;
		System::Windows::Forms::LinkLabel^ LibNlohmannJson_LLBL;
		System::Windows::Forms::LinkLabel^ LibPerlinNoise_LLBL;
		System::Windows::Forms::LinkLabel^ LibUUID_LLBL;
		System::Windows::Forms::Label^ Version_LBL;
	private: System::Windows::Forms::LinkLabel^ LibGLM_LLBL;


	private:
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		void InitializeComponent(void)
		{
			this->AppDesc_TB = (gcnew System::Windows::Forms::TextBox());
			this->CreatorQM_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->Creators_LBL = (gcnew System::Windows::Forms::Label());
			this->CreatorHardcoded_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->LibAssimp_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->LibLodePNG_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->LibLZ4_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->LibNlohmannJson_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->LibPerlinNoise_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->LibUUID_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->Version_LBL = (gcnew System::Windows::Forms::Label());
			this->LibGLM_LLBL = (gcnew System::Windows::Forms::LinkLabel());
			this->SuspendLayout();
			// 
			// AppDesc_TB
			// 
			this->AppDesc_TB->Anchor = static_cast<System::Windows::Forms::AnchorStyles>(((System::Windows::Forms::AnchorStyles::Top | System::Windows::Forms::AnchorStyles::Left)
				| System::Windows::Forms::AnchorStyles::Right));
			this->AppDesc_TB->BackColor = System::Drawing::SystemColors::MenuBar;
			this->AppDesc_TB->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->AppDesc_TB->Cursor = System::Windows::Forms::Cursors::Arrow;
			this->AppDesc_TB->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->AppDesc_TB->Location = System::Drawing::Point(12, 12);
			this->AppDesc_TB->Multiline = true;
			this->AppDesc_TB->Name = L"AppDesc_TB";
			this->AppDesc_TB->ReadOnly = true;
			this->AppDesc_TB->ShortcutsEnabled = false;
			this->AppDesc_TB->Size = System::Drawing::Size(375, 46);
			this->AppDesc_TB->TabIndex = 0;
			this->AppDesc_TB->TabStop = false;
			this->AppDesc_TB->Text = L"Tile Converter allows you to convert Tiles from Scrap Mechanic into 3D models!";
			this->AppDesc_TB->MouseDown += gcnew System::Windows::Forms::MouseEventHandler(this, &AboutGui::MouseKillFocus);
			this->AppDesc_TB->MouseMove += gcnew System::Windows::Forms::MouseEventHandler(this, &AboutGui::MouseKillFocus);
			// 
			// CreatorQM_LLBL
			// 
			this->CreatorQM_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->CreatorQM_LLBL->AutoSize = true;
			this->CreatorQM_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->CreatorQM_LLBL->Location = System::Drawing::Point(9, 120);
			this->CreatorQM_LLBL->Name = L"CreatorQM_LLBL";
			this->CreatorQM_LLBL->Size = System::Drawing::Size(120, 16);
			this->CreatorQM_LLBL->TabIndex = 1;
			this->CreatorQM_LLBL->TabStop = true;
			this->CreatorQM_LLBL->Text = L"Questionable Mark";
			this->CreatorQM_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::CreatorQM_LLBL_LinkClicked);
			// 
			// Creators_LBL
			// 
			this->Creators_LBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->Creators_LBL->AutoSize = true;
			this->Creators_LBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Creators_LBL->Location = System::Drawing::Point(9, 104);
			this->Creators_LBL->Name = L"Creators_LBL";
			this->Creators_LBL->Size = System::Drawing::Size(61, 16);
			this->Creators_LBL->TabIndex = 2;
			this->Creators_LBL->Text = L"Creators:";
			// 
			// CreatorHardcoded_LLBL
			// 
			this->CreatorHardcoded_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->CreatorHardcoded_LLBL->AutoSize = true;
			this->CreatorHardcoded_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->CreatorHardcoded_LLBL->Location = System::Drawing::Point(9, 136);
			this->CreatorHardcoded_LLBL->Name = L"CreatorHardcoded_LLBL";
			this->CreatorHardcoded_LLBL->Size = System::Drawing::Size(130, 16);
			this->CreatorHardcoded_LLBL->TabIndex = 3;
			this->CreatorHardcoded_LLBL->TabStop = true;
			this->CreatorHardcoded_LLBL->Text = L"Kariaro (Hardcoded)";
			this->CreatorHardcoded_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::CreatorHardcoded_LLBL_LinkClicked);
			// 
			// LibAssimp_LLBL
			// 
			this->LibAssimp_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LibAssimp_LLBL->AutoSize = true;
			this->LibAssimp_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LibAssimp_LLBL->Location = System::Drawing::Point(335, 120);
			this->LibAssimp_LLBL->Name = L"LibAssimp_LLBL";
			this->LibAssimp_LLBL->Size = System::Drawing::Size(52, 16);
			this->LibAssimp_LLBL->TabIndex = 5;
			this->LibAssimp_LLBL->TabStop = true;
			this->LibAssimp_LLBL->Text = L"Assimp";
			this->LibAssimp_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibAssimp_LLBL_LinkClicked);
			// 
			// LibLodePNG_LLBL
			// 
			this->LibLodePNG_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LibLodePNG_LLBL->AutoSize = true;
			this->LibLodePNG_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LibLodePNG_LLBL->Location = System::Drawing::Point(262, 120);
			this->LibLodePNG_LLBL->Name = L"LibLodePNG_LLBL";
			this->LibLodePNG_LLBL->Size = System::Drawing::Size(67, 16);
			this->LibLodePNG_LLBL->TabIndex = 6;
			this->LibLodePNG_LLBL->TabStop = true;
			this->LibLodePNG_LLBL->Text = L"LodePNG";
			this->LibLodePNG_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibLodePNG_LLBL_LinkClicked);
			// 
			// LibLZ4_LLBL
			// 
			this->LibLZ4_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LibLZ4_LLBL->AutoSize = true;
			this->LibLZ4_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LibLZ4_LLBL->Location = System::Drawing::Point(358, 104);
			this->LibLZ4_LLBL->Name = L"LibLZ4_LLBL";
			this->LibLZ4_LLBL->Size = System::Drawing::Size(29, 16);
			this->LibLZ4_LLBL->TabIndex = 7;
			this->LibLZ4_LLBL->TabStop = true;
			this->LibLZ4_LLBL->Text = L"LZ4";
			this->LibLZ4_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibLZ4_LLBL_LinkClicked);
			// 
			// LibNlohmannJson_LLBL
			// 
			this->LibNlohmannJson_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LibNlohmannJson_LLBL->AutoSize = true;
			this->LibNlohmannJson_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->LibNlohmannJson_LLBL->Location = System::Drawing::Point(280, 136);
			this->LibNlohmannJson_LLBL->Name = L"LibNlohmannJson_LLBL";
			this->LibNlohmannJson_LLBL->Size = System::Drawing::Size(107, 16);
			this->LibNlohmannJson_LLBL->TabIndex = 8;
			this->LibNlohmannJson_LLBL->TabStop = true;
			this->LibNlohmannJson_LLBL->Text = L"Nlohmann JSON";
			this->LibNlohmannJson_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibNlohmannJson_LLBL_LinkClicked);
			// 
			// LibPerlinNoise_LLBL
			// 
			this->LibPerlinNoise_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LibPerlinNoise_LLBL->AutoSize = true;
			this->LibPerlinNoise_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular,
				System::Drawing::GraphicsUnit::Point, static_cast<System::Byte>(0)));
			this->LibPerlinNoise_LLBL->Location = System::Drawing::Point(307, 88);
			this->LibPerlinNoise_LLBL->Name = L"LibPerlinNoise_LLBL";
			this->LibPerlinNoise_LLBL->Size = System::Drawing::Size(80, 16);
			this->LibPerlinNoise_LLBL->TabIndex = 9;
			this->LibPerlinNoise_LLBL->TabStop = true;
			this->LibPerlinNoise_LLBL->Text = L"Perlin Noise";
			this->LibPerlinNoise_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibPerlinNoise_LLBL_LinkClicked);
			// 
			// LibUUID_LLBL
			// 
			this->LibUUID_LLBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->LibUUID_LLBL->AutoSize = true;
			this->LibUUID_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LibUUID_LLBL->Location = System::Drawing::Point(271, 104);
			this->LibUUID_LLBL->Name = L"LibUUID_LLBL";
			this->LibUUID_LLBL->Size = System::Drawing::Size(40, 16);
			this->LibUUID_LLBL->TabIndex = 10;
			this->LibUUID_LLBL->TabStop = true;
			this->LibUUID_LLBL->Text = L"UUID";
			this->LibUUID_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibUUID_LLBL_LinkClicked);
			// 
			// Version_LBL
			// 
			this->Version_LBL->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->Version_LBL->AutoSize = true;
			this->Version_LBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->Version_LBL->Location = System::Drawing::Point(9, 88);
			this->Version_LBL->Name = L"Version_LBL";
			this->Version_LBL->Size = System::Drawing::Size(86, 16);
			this->Version_LBL->TabIndex = 11;
			this->Version_LBL->Text = L"Version: 1.0.2";
			// 
			// LibGLM_LLBL
			// 
			this->LibGLM_LLBL->AutoSize = true;
			this->LibGLM_LLBL->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->LibGLM_LLBL->Location = System::Drawing::Point(317, 104);
			this->LibGLM_LLBL->Name = L"LibGLM_LLBL";
			this->LibGLM_LLBL->Size = System::Drawing::Size(35, 16);
			this->LibGLM_LLBL->TabIndex = 12;
			this->LibGLM_LLBL->TabStop = true;
			this->LibGLM_LLBL->Text = L"GLM";
			this->LibGLM_LLBL->LinkClicked += gcnew System::Windows::Forms::LinkLabelLinkClickedEventHandler(this, &AboutGui::LibGLM_LLBL_LinkClicked);
			// 
			// AboutGui
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(399, 161);
			this->Controls->Add(this->LibGLM_LLBL);
			this->Controls->Add(this->Version_LBL);
			this->Controls->Add(this->LibUUID_LLBL);
			this->Controls->Add(this->LibPerlinNoise_LLBL);
			this->Controls->Add(this->LibNlohmannJson_LLBL);
			this->Controls->Add(this->LibLZ4_LLBL);
			this->Controls->Add(this->LibLodePNG_LLBL);
			this->Controls->Add(this->LibAssimp_LLBL);
			this->Controls->Add(this->CreatorHardcoded_LLBL);
			this->Controls->Add(this->Creators_LBL);
			this->Controls->Add(this->CreatorQM_LLBL);
			this->Controls->Add(this->AppDesc_TB);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"AboutGui";
			this->ShowIcon = false;
			this->Text = L"About";
			this->Load += gcnew System::EventHandler(this, &AboutGui::AboutGui_Load);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private:
		System::Void AboutGui_Load(System::Object^ sender, System::EventArgs^ e);
		System::Void CreatorQM_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void CreatorHardcoded_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibPerlinNoise_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibLodePNG_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibLZ4_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibUUID_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibAssimp_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibNlohmannJson_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void LibGLM_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e);
		System::Void MouseKillFocus(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e);
	};
}
