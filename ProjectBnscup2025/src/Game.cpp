// # include "Game.hpp"

// Game::Game(const InitData& init)
// 	: IScene{ init }
// {
// 	for (int32 y = 0; y < 5; ++y)
// 	{
// 		for (int32 x = 0; x < (800 / BrickSize.x); ++x)
// 		{
// 			m_bricks << Rect{ (x * BrickSize.x), (60 + y * BrickSize.y), BrickSize };
// 		}
// 	}
// }

// void Game::update()
// {
// 	// Move ball
// 	m_ball.moveBy(m_ballVelocity * Scene::DeltaTime());

// 	// Check blocks in order
// 	for (auto it = m_bricks.begin(); it != m_bricks.end(); ++it)
// 	{
// 		// If block and ball intersect
// 		if (it->intersects(m_ball))
// 		{
// 			// If intersecting with top or bottom edge of block
// 			if (it->bottom().intersects(m_ball) || it->top().intersects(m_ball))
// 			{
// 				m_ballVelocity.y *= -1;
// 			}
// 			else // If intersecting with left or right edge of block
// 			{
// 				m_ballVelocity.x *= -1;
// 			}

// 			// Remove block from array (iterator becomes invalid)
// 			m_bricks.erase(it);

// 			m_brickSound.playOneShot(0.5);

// 			++m_score;

// 			break;
// 		}
// 	}

// 	// If hit ceiling
// 	if ((m_ball.y < 0) && (m_ballVelocity.y < 0))
// 	{
// 		m_ballVelocity.y *= -1;
// 	}

// 	// If hit left or right walls
// 	if (((m_ball.x < 0) && (m_ballVelocity.x < 0))
// 		|| ((800 < m_ball.x) && (0 < m_ballVelocity.x)))
// 	{
// 		m_ballVelocity.x *= -1;
// 	}

// 	// Bounce off paddle
// 	if (const Rect paddle = getPaddle();
// 		(0 < m_ballVelocity.y) && paddle.intersects(m_ball))
// 	{
// 		// Change bounce direction based on distance from paddle center
// 		m_ballVelocity = Vec2{ (m_ball.x - paddle.center().x) * 10, -m_ballVelocity.y }.setLength(BallSpeed);
// 	}

// 	// If goes off screen or no blocks left
// 	if ((600 < m_ball.y) || m_bricks.isEmpty())
// 	{
// 		// Go to ranking screen
// 		changeScene(State::Title);

// 		getData().lastScore = m_score;
// 	}
// }

// void Game::draw() const
// {
// 	Scene::SetBackground(ColorF{ 0.2 });

// 	// Draw all blocks
// 	for (const auto& brick : m_bricks)
// 	{
// 		brick.stretched(-1).draw(HSV{ brick.y - 40 });
// 	}

// 	// Draw ball
// 	m_ball.draw();

// 	// Draw paddle
// 	getPaddle().rounded(3).draw();

// 	// Hide mouse cursor
// 	Cursor::RequestStyle(CursorStyle::Hidden);

// 	// Draw score
// 	FontAsset(U"Bold")(m_score).draw(24, Vec2{ 400, 16 });
// }

// Rect Game::getPaddle() const
// {
// 	return{ Arg::center(Cursor::Pos().x, 500), 60, 10 };
// }

// #include <Siv3D.hpp> // OpenSiv3D v0.6.16
// #include "SivBullet.hpp"
#include "Game.hpp"
//#include "BulletBody.hpp"

Game::Game(const InitData& init)
	: IScene{ init }
{
	// 背景色 (removeSRGBCurve() で sRGB カーブを除去)
	bgColor = Palette::Skyblue.removeSRGBCurve();

	// デバッグカメラ
	camera = DebugCamera3D{ rt.size(), 60_deg, Vec3{0, 30, -100}, Vec3{0, 0, 0} };

	// シミュレーション空間の定義 (重力 Vec3{ 0, -9.8, 0 })
	world = { Vec3{ 0, -9.8, 0 } };

	// 床
	// 質量 0.0 で { 0, 0, 0 } に { 100, 0.1, 100 } の大きさの箱
	ground = world.createBox(Vec3{ 0, 0, 0 }, Vec3{ 100, 0.1, 100 }, 0.0);
}

void Game::update() 
{
	// 空間の更新
	world.update(Scene::DeltaTime());

	// Enterキーで箱を追加
	if (KeyEnter.down()) {
		boxes << world.createBox(Vec3{ Random(-50, 50), 50, Random(-50, 50)}, Vec3{1, 1, 1}, 1.0);
	}

	// カメラの更新
	camera.update(50.0f);

	// カメラを3D空間に設定
	Graphics3D::SetCameraTransform(camera);
}

void Game::draw() const
	{
		// [3D 描画]
		{
			const ScopedRenderTarget3D target{ rt.clear(bgColor) };

			// 床の描画
			ground.draw();
			// 箱の描画
			for (const auto& box : boxes) {
				box.draw();
			}
		}

		// 3D シーンの描画
		Graphics3D::Flush();
		rt.resolve();
		Shader::LinearToScreen(rt);
	}