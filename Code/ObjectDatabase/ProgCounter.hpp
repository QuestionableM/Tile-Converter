#pragma once

#include <string>

enum class ProgState : std::size_t
{
	None = 0,
	LoadingVanilla = 1,
	LoadingModded = 2,

	ReadingTile = 3,

	ConvertSuccess = 4,
	ConvertFailure = 5
};

class ProgCounter
{
public:
	static ProgState State;
	static std::size_t ProgressMax;
	static std::size_t ProgressValue;

	static void SetState(const ProgState& nState);
	static void SetState(const ProgState& nState, const std::size_t& new_max);

	static std::wstring GetStateString();
	static bool StateHasNumbers();
};