# include "Common.hpp"
# include "Title.hpp"
# include "Game.hpp"
# include "Ranking.hpp"
# include "CameraTest.hpp"
# include "ModelView.hpp"
# include "Result.hpp"
# include "Stage.hpp"
# include "ToBeContinued.hpp"
# include "ModelCheck.hpp"

void Main()
{
	// マウスでサイズ変更可能にする
	Window::SetStyle(WindowStyle::Sizable);

	// シーンサイズ：ゲーム中はキープしておく（変更するとズレてしまうため）
	Scene::SetResizeMode(ResizeMode::Keep);

    // ウインドウとシーンをリサイズする
    Window::Resize(WINDOW_WIDTH, WINDOW_HEIGHT);
	Scene::Resize(SCENE_WIDTH, SCENE_HEIGHT);

	// フルスクリーンモード
//	Window::SetFullscreen(true);

	// フォント
//	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
//	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"assets/font/SoukouMincho-Font/SoukouMincho.ttf");
//	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"assets/font/07鉄瓶ゴシック.otf");
//	FontAsset(U"TitleFont").setBufferThickness(4);
//	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);
//	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, U"assets/font/SoukouMincho-Font/SoukouMincho.ttf");
	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, U"assets/font/07鉄瓶ゴシック.otf");

	// SE 足音
//	AudioAsset::Register(U"足音45秒のループ", U"assets/sound/se/足音45秒のループ.wav", true);
	AudioAsset::Register(U"footsteps1", U"assets/sound/se/se_footsteps_concrete-01.wav");
	AudioAsset::Register(U"footsteps2", U"assets/sound/se/se_footsteps_concrete-02.wav");

	// SE 引き出し
	AudioAsset::Register(U"drawer_open", U"assets/sound/se/drawer_open.mp3");
	AudioAsset::Register(U"drawer_close", U"assets/sound/se/drawer_close.mp3");

	// SE 牢屋の扉
//	AudioAsset::Register(U"牢屋の扉を閉める", U"assets/sound/se/牢屋の扉を閉める.wav");
	AudioAsset::Register(U"牢屋の扉を開ける", U"assets/sound/se/牢屋の扉を開ける.wav");
//	AudioAsset::Register(U"Cancellation", U"assets/sound/se/SE_Cancellation01.mp3");
	AudioAsset::Register(U"IronDoor_Close", U"assets/sound/se/SE_IronDoor_Close01.mp3");
	
	// SE 木製の扉
	AudioAsset::Register(U"WoodDoor_Close", U"assets/sound/se/SE_WoodDoor_Close01.mp3");
	AudioAsset::Register(U"WoodDoor_open01", U"assets/sound/se/SE_WoodDoor_open01.mp3");

	// SE 石板
//	AudioAsset::Register(U"Stone", U"assets/sound/se/SE_Stone01.mp3");
	AudioAsset::Register(U"Stone", U"assets/sound/se/SE_Stone02.mp3");

	// SE アイテム
	AudioAsset::Register(U"GET", U"assets/sound/se/GET.wav");
	AudioAsset::Register(U"Item", U"assets/sound/se/SE_Item01.mp3");
	AudioAsset::Register(U"Paper", U"assets/sound/se/SE_Paper01.mp3");
	AudioAsset::Register(U"Title", U"assets/sound/se/SE_Title01.mp3");
	AudioAsset::Register(U"Water", U"assets/sound/se/SE_Water01.mp3");
	AudioAsset::Register(U"Inventory", U"assets/sound/se/SE_Inventory_Open01.mp3");

	// SE 火かき棒で鉄製のカギを入手
	AudioAsset::Register(U"KeyGimmick01", U"assets/sound/se/SE_KeyGimmick01.mp3");

	// SE 石板、引き出しのギミックを説いた後のSE
	AudioAsset::Register(U"lock_wood_door1", U"assets/sound/se/lock_wood_door1.mp3");

	// SE はぁはぁ
	AudioAsset::Register(U"breath_pant", U"assets/sound/se/se_breath_pant.wav");
	AudioAsset::Register(U"breath_deep", U"assets/sound/se/se_breath_deep.wav");

	// BGM メイン
//	AudioAsset::Register(U"BGM", U"assets/sound/bgm/BGM.mp3", Loop::Yes);
	AudioAsset::Register(U"BGM", U"assets/sound/bgm/BGM_Map01.mp3", Loop::Yes);

	// BGM 暖炉
	AudioAsset::Register(U"bonfire", U"assets/sound/bgm/bonfire.mp3", Loop::Yes);

    App manager;
	manager.add<Title>(State::Title);
//	manager.add<Game>(State::Game);
//	manager.add<Ranking>(State::Ranking);
	manager.add<CameraTest>(State::CameraTest);
//    manager.add<ModelView>(State::ModelView);
//    manager.add<Result>(State::Result);
//	manager.add<Stage>(State::Stage);
	manager.add<ToBeContinued>(State::ToBeContinued);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}
