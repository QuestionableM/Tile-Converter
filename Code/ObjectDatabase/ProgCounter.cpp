#include "ProgCounter.hpp"

ProgState ProgCounter::State = ProgState::None;
std::size_t ProgCounter::ProgressMax   = 0;
std::size_t ProgCounter::ProgressValue = 0;

void ProgCounter::SetState(const ProgState& nState)
{
	ProgCounter::State = nState;
}

void ProgCounter::SetState(const ProgState& nState, const std::size_t& new_max)
{
	ProgCounter::State = nState;

	ProgCounter::ProgressValue = 0;
	ProgCounter::ProgressMax = new_max;
}

static const std::wstring StateStrings[] =
{
	L"Loading Vanilla Objects... ",
	L"Loading Modded Objects... ",
	L"Reading Tile...",
	L"Success!",
	L"Convert Failed"
};

static const bool StateNumberData[] =
{
	true,
	true,
	false,
	false,
	false
};
/*
None = 0,
LoadingVanilla = 1,
LoadingModded = 2,

ReadingTile = 3,

ConvertSuccess = 4,
ConvertFailure = 5
*/

std::wstring ProgCounter::GetStateString()
{
	const std::size_t state_idx = static_cast<std::size_t>(ProgCounter::State);
	if (state_idx > 0)
	{
		return StateStrings[state_idx - 1];
	}

	return L"NO_STATE";
}

bool ProgCounter::StateHasNumbers()
{
	const std::size_t state_idx = static_cast<std::size_t>(ProgCounter::State);
	if (state_idx > 0)
	{
		return StateNumberData[state_idx - 1];
	}

	return false;
}