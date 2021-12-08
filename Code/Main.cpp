#define NOMINMAX
#include <Windows.h>

#include "Console.hpp"
#include "SMReaders/Tile/TileReader.hpp"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	CreateDebugConsole(L"World Converter Debug Console");

	Tile* test = TileReader::ReadTile(L"./Tests/FOREST256_01.tile");

	std::ofstream output_model("./model_output.obj");
	if (output_model.is_open())
	{
		const std::vector<float> pHeightArray = test->GetVertexHeight();

		const int tWidth = test->GetWidth() * 32 + 1;
		const int tHeight = test->GetHeight() * 32 + 1;

		for (int y = 0; y < tHeight; y++)
		{
			for (int x = 0; x < tWidth; x++)
			{
				const int index = x + y * tWidth;
				const float& height = pHeightArray[index];

				output_model << "v " << x << " " << height << " " << y << "\n";
			}
		}

		for (int y = 0; y < tHeight - 1; y++)
		{
			for (int x = 0; x < tWidth - 1; x++)
			{
				const int h00 = (x    ) + (y    ) * tWidth + 1;
				const int h01 = (x    ) + (y + 1) * tWidth + 1;
				const int h10 = (x + 1) + (y    ) * tWidth + 1;
				const int h11 = (x + 1) + (y + 1) * tWidth + 1;

				output_model << "f " << h00 << " " << h01 << " " << h10 << "\n";
				output_model << "f " << h01 << " " << h11 << " " << h10 << "\n";
			}
		}

		output_model.close();
	}

	while (true) {};

	return 0;
}