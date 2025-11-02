# pragma once
# include "Common.hpp"

// ランキングシーン
class ModelView : public App::Scene
{
public:
    
    ModelView(const InitData& init);
    
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
    ///
    
    /// モデル配置
    // モデルの読み込み（マテリアル参照を無視）
    const Model model{ U"assets/models/Room/EV_Room01.obj" };
    
    const Texture floorTexture{ U"assets/models/Room/T_EV_Floor01_D.png", TextureDesc::Mipped };
    const Texture wallTexture{ U"assets/models/Room/T_EV_Wall01_D.png", TextureDesc::Mipped };
    
    // 各オブジェクトの位置
    const Vec3 roomPos{ 0, 0, 0 };
    // スケーリングの倍率
    const Vec3 roomScale{ 0.01, 0.01, 0.01 };
    ///

    
   
    
    
    
    // カメラ系
//    // カメラの位置
//    Vec3 cameraPos = { 0.0, 1.6, 0.0 }; // 少し高め（身長の高さ）
//    
//    // カメラの向き（Yaw：左右回転, Pitch：上下回転）
//    double yaw = 0.0;
//    double pitch = 0.0;
//    
//    // カメラの移動スピード
//    const double moveSpeed = 5.0;
//    const double mouseSensitivity = 0.1;
};
