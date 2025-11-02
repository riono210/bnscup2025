# pragma once
#include <Siv3D.hpp>

// シーンのステート
enum class State
{
	Title,
	Game,
	Ranking,
	CameraTest,
    ModelView,
    Result,
    Stage,
	ToBeContinued,
};

// 共有するデータ
struct GameData
{
	// 直前のゲームのスコア
	int32 lastScore = 0;

	// ハイスコア
	Array<int32> highScores = { 10, 8, 6, 4, 2 };
};

using App = SceneManager<State, GameData>;

// Windows で Direct3D の代わりに OpenGL を使用するモードに切り替える
SIV3D_SET(EngineOption::Renderer::OpenGL);

// ウィンドウサイズ
// TODO オプションで変えられるようにする
// TODO 変えてもズレないようにする（UIスケーリング対応）
//const int WINDOW_WIDTH = 800;
//const int WINDOW_HEIGHT = 600;
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;
//const int WINDOW_WIDTH = 1920;
//const int WINDOW_HEIGHT = 1080;

// シーンサイズ
const int SCENE_WIDTH = 1280;
const int SCENE_HEIGHT = 720;
//const int SCENE_WIDTH = 1920;
//const int SCENE_HEIGHT = 1080;

// フォーカスエリア
const int focusWidth = 200;
const int focusHeight = 200;
const float focusDistance = 1.5;
