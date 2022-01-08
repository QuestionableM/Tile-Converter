#include "MainGui.h"
#include "Console.hpp"

#include "ObjectDatabase/ObjectDatabase.hpp"

namespace TileConverter
{
	void MainGui::MainGui_Load(System::Object^ sender, System::EventArgs^ e)
	{
		DebugOutL("MainGui has been loaded!");

		DebugOutL("Initializing database...");
		DatabaseLoader::InitializeDatabase();

		this->DatabaseLoader_BW->RunWorkerAsync();
	}

	void MainGui::DatabaseLoader_BW_DoWork(System::Object^ sender, System::ComponentModel::DoWorkEventArgs^ e)
	{
		DebugOutL("Loading object database from another thread...");
		DatabaseLoader::LoadDatabase();
	}

	void MainGui::DatabaseLoader_BW_RunWorkerCompleted(System::Object^ sender, System::ComponentModel::RunWorkerCompletedEventArgs^ e)
	{
		DebugOutL("Loading object database has been successfully finished!");
	}
}