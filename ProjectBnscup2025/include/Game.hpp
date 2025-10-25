# pragma once
# include "Common.hpp"

Image CreateTableImage();
Image CreateBlockImage();

// ゲームの状態
struct GameState
{
    static constexpr int32 MaxY = 15;

    int32 s[8][(MaxY + 1)][8] = {};

    int32 getHeight(int32 x, int32 z) const
    {
        for (int32 y = MaxY; 0 <= y; --y)
        {
            if (s[x][y][z])
            {
                return (y + 1);
            }
        }

        return 0;
    }
};

// カメラの制御クラス
class CameraController
{
public:

    explicit CameraController(const Size& sceneSize)
        : m_camera{ sceneSize, m_verticalFOV, m_eyePosition, m_focusPosition } {}

    void update()
    {
        const Ray ray = getMouseRay();

        // 盤のまわり部分
        const std::array<Box, 4> boxes =
        {
            Box::FromPoints(Vec3{ -5, 0.0, 5 }, Vec3{ 5, -0.4, 4 }),
            Box::FromPoints(Vec3{ 4, 0.0, 5 }, Vec3{ 5, -0.4, -5 }),
            Box::FromPoints(Vec3{ -5, 0.0, -4 }, Vec3{ 5, -0.4, -5 }),
            Box::FromPoints(Vec3{ -5, 0.0, 5 }, Vec3{ -4, -0.4, -5 })
        };

        if (MouseL.up())
        {
            m_grabbed = false;
        }

        if (m_grabbed)
        {
            const double before = (m_cursorPos - Scene::Center()).getAngle();
            const double after = (Cursor::Pos() - Scene::Center()).getAngle();
            m_phi -= (after - before);
            m_cursorPos = Cursor::Pos();
        }

        for (const auto& box : boxes)
        {
            if (box.intersects(ray))
            {
                // マウスカーソルを手のアイコンにする
                Cursor::RequestStyle(CursorStyle::Hand);

                if ((not m_grabbed) && MouseL.down())
                {
                    m_grabbed = true;
                    m_cursorPos = Cursor::Pos();
                }
            }
        }

        // 視点を球面座標系で計算する
        m_eyePosition = Spherical{ 24, m_theta, (270_deg - m_phi) };

        // カメラを更新する
        m_camera.setView(m_eyePosition, m_focusPosition);

        // シーンにカメラを設定する
        Graphics3D::SetCameraTransform(m_camera);
    }

    Ray getMouseRay() const
    {
        return m_camera.screenToRay(Cursor::PosF());
    }

    bool isGrabbing() const
    {
        return m_grabbed;
    }

private:

    // 縦方向の視野角（ラジアン）
    double m_verticalFOV = 25_deg;

    // カメラの視点の位置
    Vec3 m_eyePosition{ 16, 16, -16 };

    // カメラの注視点の位置
    Vec3 m_focusPosition{ 0, 0, 0 };

    double m_phi = -20_deg;

    double m_theta = 50_deg;

    // カメラ
    BasicCamera3D m_camera;

    bool m_grabbed = false;

    Vec2 m_cursorPos = Scene::Center();
};



// ゲームシーン
class Game : public App::Scene
{
public:

    Game(const InitData& init);

    void update() override;

    void draw() const override;

private:
    // 背景色 (3D 用の色は .removeSRGBCurve() で sRGB カーブを除去）
    const ColorF backgroundColor = ColorF{ 0.4, 0.6, 0.8 }.removeSRGBCurve();

    // 3D シーンを描く、マルチサンプリング対応レンダーテクスチャ
    const MSRenderTexture renderTexture{ Scene::Size(), TextureFormat::R8G8B8A8_Unorm_SRGB, HasDepth::Yes };

    // テーブル用のテクスチャ
    const Texture tableTexture{ CreateTableImage(), TextureDesc::MippedSRGB };

    // ブロック用のテクスチャ
    const Texture blockTexture{ CreateBlockImage(), TextureDesc::MippedSRGB };

    // 盤用の 3D メッシュ
    const Mesh meshBoard{ MeshData::RoundedBox(0.1, Vec3{ 10, 0.4, 10 }, 5).translate(0, -0.2, 0) };
    
    // ゲームの状態
    GameState gameState;
    
    // カメラ制御
    CameraController cameraController{ renderTexture.size() };

    // アクティブなボクセル
    Point activeVoxelXZ;
    
    RoundRect resultButton{ Arg::center(1100, 100), 300, 60, 8 };
    Transition resultTransition{ 0.4s, 0.2s };

    RoundRect walk45SecLoopButton{ Arg::center(1100, 200), 300, 60, 8 };
    Transition walk45SecLoopTransition{ 0.4s, 0.2s };
    
    RoundRect doorOpenButton{ Arg::center(1100, 300), 300, 60, 8 };
    Transition doorOpenTransition{ 0.4s, 0.2s };
    
    RoundRect doorCloseButton{ Arg::center(1100, 400), 300, 60, 8 };
    Transition doorCloseTransition{ 0.4s, 0.2s };
    
};
