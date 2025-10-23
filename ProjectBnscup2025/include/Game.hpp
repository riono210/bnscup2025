# pragma once
# include "Common.hpp"
# include "SivBullet.hpp"
// Game scene
class Game : public App::Scene
{
public:

	Game(const InitData& init);

	void update() override;

	void draw() const override;

private:

	// // Block size
	// static constexpr Size BrickSize{ 40, 20 };

	// // Ball speed
	// static constexpr double BallSpeed = 480.0;

	// // Ball velocity
	// Vec2 m_ballVelocity{ 0, -BallSpeed };

	// // Ball
	// Circle m_ball{ 400, 400, 8 };

	// // Block array
	// Array<Rect> m_bricks;

	// // Current game score
	// int32 m_score = 0;

	// // Sound effect when breaking blocks
	// Audio m_brickSound{ GMInstrument::Woodblock, PianoKey::C5, 0.2s, 0.1s };

	// Rect getPaddle() const;

	// 3D シーンを描く、マルチサンプリング対応レンダーテクスチャ
	const MSRenderTexture rt{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	// 背景色 (removeSRGBCurve() で sRGB カーブを除去)
	Color bgColor = Palette::Skyblue.removeSRGBCurve();

	// デバッグカメラ
	DebugCamera3D camera{ rt.size(), 60_deg, Vec3{0, 30, -100}, Vec3{0, 0, 0} };

	// シミュレーション空間の定義 (重力 Vec3{ 0, -9.8, 0 })
	DynamicsWorld world{ Vec3{ 0, -9.8, 0 } };

	// 床
	// 質量 0.0 で { 0, 0, 0 } に { 100, 0.1, 100 } の大きさの箱
	BulletBody ground = world.createBox(Vec3{ 0, 0, 0 }, Vec3{ 100, 0.1, 100 }, 0.0);

	// 箱の配列
	Array<BulletBody> boxes;
};