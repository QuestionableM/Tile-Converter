#include "AboutGui.h"

#include "Utils/WinInclude.hpp"

namespace TileConverter
{
	AboutGui::AboutGui()
	{
		this->InitializeComponent();
	}

	AboutGui::~AboutGui()
	{
		if (components) delete components;
	}

	void AboutGui::AboutGui_Load(System::Object^ sender, System::EventArgs^ e)
	{
	#if _WIN64
		this->Version_LBL->Text += " (x64)";
	#else
		this->Version_LBL->Text += " (x86)";
	#endif

	#if _DEBUG
			this->Version_LBL->Text += " DEBUG";
	#endif
	}

	void AboutGui::CreatorQM_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/QuestionableM");
	}

	void AboutGui::CreatorHardcoded_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/Kariaro");
	}

	void AboutGui::LibPerlinNoise_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/Reputeless/PerlinNoise");
	}

	void AboutGui::LibLodePNG_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/lvandeve/lodepng");
	}

	void AboutGui::LibLZ4_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/lz4/lz4");
	}

	void AboutGui::LibUUID_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/mariusbancila/stduuid");
	}

	void AboutGui::LibAssimp_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/assimp/assimp");
	}

	void AboutGui::LibNlohmannJson_LLBL_LinkClicked(System::Object^ sender, System::Windows::Forms::LinkLabelLinkClickedEventArgs^ e)
	{
		System::Diagnostics::Process::Start("https://github.com/nlohmann/json");
	}

	void AboutGui::MouseKillFocus(System::Object^ sender, System::Windows::Forms::MouseEventArgs^ e)
	{
		this->AppDesc_TB->SelectionLength = 0;
		SendMessage(
			static_cast<HWND>(this->AppDesc_TB->Handle.ToPointer()),
			WM_KILLFOCUS,
			0,
			NULL
		);
	}
}