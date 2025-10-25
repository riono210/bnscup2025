# include "Stage.hpp"

Stage::Stage(const InitData& init)
    : IScene{ init }
{
    // モデルに付随するテクスチャをアセット管理に登録
    Model::RegisterDiffuseTextures(model, TextureDesc::MippedSRGB);
    
    // おまじない
    // 牢屋のような薄暗い雰囲気の設定
    Graphics3D::SetGlobalAmbientColor(ColorF{ 0.2, 0.2, 0.25 }); // ほぼ暗闇
    Graphics3D::SetSunColor(ColorF{ 0.2, 0.2, 0.25 }); // 光源を弱める
    Graphics3D::SetSunDirection(Vec3{ 0, -1, -0.5 }.normalized()); // 影を強調
    
    AudioAsset(U"BGM").play();
}

void Stage::update()
{
    if (MouseL.down())
    {
        // タイトルシーンへ
        AudioAsset(U"BGM").stop();
        changeScene(State::Title);
    }
    
    // カメラの更新
    camera.update(15.0);
}

void Stage::draw() const
{
    // renderTexture を背景色で塗りつぶし、3D 描画のレンダーターゲットに
    const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };
    
    // 3D 描画
    {
        Graphics3D::SetCameraTransform(camera);
        // スケーリングを適用してモデルを描画
        Transformer3D t{ Mat4x4::RotateY(45_deg).scaled(roomScale).translated(roomPos) };
        // モデルを描画
        model.draw();
    }
    
    // 描画のおまじない
    {
        Graphics3D::Flush();
        renderTexture.resolve();
        Shader::LinearToScreen(renderTexture);
    }
}
