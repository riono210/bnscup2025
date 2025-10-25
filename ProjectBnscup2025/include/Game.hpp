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
	// 3D シーンを描く、マルチサンプリング対応レンダーテクスチャ
	const MSRenderTexture rt{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
	// 背景色 (removeSRGBCurve() で sRGB カーブを除去)
	Color bgColor;

	// デバッグカメラ
	DebugCamera3D camera;

	// シミュレーション空間の定義 (重力 Vec3{ 0, -9.8, 0 })
	DynamicsWorld world;

	// 床
	// 質量 0.0 で { 0, 0, 0 } に { 100, 0.1, 100 } の大きさの箱
	BulletBody ground;

	// 箱の配列
	Array<BulletBody> boxes;
};