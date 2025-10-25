# pragma once
# include "Common.hpp"

// ランキングシーン
class Stage : public App::Scene
{
public:
    
    Stage(const InitData& init);
    
    void update() override;
    
    void draw() const override;
    
private:
    
    /// ライティング
    // 牢屋の雰囲気に合う暗い背景色
    const ColorF backgroundColor = ColorF{ 0.05, 0.08, 0.1 }.removeSRGBCurve();
    // 3D シーンを描く、マルチサンプリング対応レンダーテクスチャ
    const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };
    // カメラの設定
    DebugCamera3D camera{ renderTexture.size(), 30_deg, Vec3{ 10, 10, 10 } };
    
    /// モデル配置
    // モデルの読み込み（マテリアル参照を無視）
    const Model model{ U"assets/models/Room/EV_Room01.obj" };
    
    const Texture floorTexture{ U"assets/models/Room/T_EV_Floor01_D.png", TextureDesc::Mipped };
    const Texture wallTexture{ U"assets/models/Room/T_EV_Wall01_D.png", TextureDesc::Mipped };
    
    // 各オブジェクトの位置
    const Vec3 roomPos{ 0, 0, 0 };
    // スケーリングの倍率
    const Vec3 roomScale{ 0.01, 0.01, 0.01 };
};
