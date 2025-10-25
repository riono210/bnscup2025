# pragma once
# include <Siv3D.hpp>

// Scene states
enum class State
{
	Title,
	Game,
	ModelCheck,
};

// Shared data
struct GameData
{
	// Last game score
	int32 lastScore = 0;

	// High scores
	Array<int32> highScores = { 10, 8, 6, 4, 2 };
};

using App = SceneManager<State, GameData>;