# pragma once
# include "Common.hpp"

// タイトルシーン
class Title : public App::Scene
{
public:

	Title(const InitData& init);

	void update() override;

	void draw() const override;

private:

	// マウスオーバー処理
	float mouseOver(float color, String text, RoundRect::position_type button);

	const float buttonWidth = 300;
	const float buttonHeight = 60;

	// ゲームスタートボタンの座標
	RoundRect m_cameraTestButton{ Arg::center(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 270), buttonWidth, buttonHeight, 8 };

	// ゲーム終了ボタンの座標
	RoundRect m_exitButton{ Arg::center(WINDOW_WIDTH / 2, WINDOW_HEIGHT - 160), buttonWidth, buttonHeight, 8 };

	Transition m_startTransition{ 0.4s, 0.2s };
	Transition m_rankingTransition{ 0.4s, 0.2s };
	Transition m_exitTransition{ 0.4s, 0.2s };
	Transition m_cameraTestTransition{ 0.4s, 0.2s };
    Transition m_modelViewTransition{ 0.4s, 0.2s };
    Transition m_stageTransition{ 0.4s, 0.2s };

	// タイトル画面
	const Texture BG{ U"assets/sprites/BG_Title01.png" };

	// フォント
	const Font& boldFont = FontAsset(U"Bold");

	// フォントサイズ
	const float fontSize = 48;

	// 文字の色
	const float minColor = 0.3;
	const float maxColor = 1.0;
	float gamestartColor = minColor;
	float exitColor = minColor;

	// カーソルの位置
	int cursorIndex = -1;

	bool KeyMode = false;

	Vec2 lastCursorPos;

	bool ButtonDown = false;
};
