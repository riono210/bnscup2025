# include "Game.hpp"

// テーブル用のテクスチャを手続き的に生成する関数
Image CreateTableImage()
{
    PerlinNoise noise;
    return Image::Generate(Size{ 1024, 1024 }, [&](Point p)
    {
        const double x = Fraction(noise.octave2D0_1(p * Vec2{ 0.03, 0.0005 }, 2) * 25) * 0.3 + 0.55;
        return ColorF{ x, 0.85 * x, 0.7 * x }.removeSRGBCurve();
    }).gaussianBlurred(3);
}

// ブロック用のテクスチャを手続き的に生成する関数
Image CreateBlockImage()
{
    PerlinNoise noise;
    return Image::Generate(Size{ 256, 256 }, [&](Point p)
    {
        const double x = Fraction(noise.octave2D0_1(p * Vec2{ 0.05, 0.0005 }, 2) * 25) * 0.15 + 0.85;
        return ColorF{ x }.removeSRGBCurve();
    }).gaussianBlurred(2);
}

// テーブルを描画する関数
void DrawTable(const Texture& tableTexture)
{
    Plane{ Vec3{ 0, -0.4, 0 }, 24 }.draw(tableTexture);
}

// 盤を描画する関数
void DrawBoard(const Mesh& mesh)
{
    const ColorF BoardColor = ColorF{ 0.9, 0.85, 0.75 }.removeSRGBCurve();
    const ColorF LineColor = ColorF{ 0.3, 0.2, 0.0 }.removeSRGBCurve();

    mesh.draw(BoardColor);

    // 盤上の線
    for (int32 i = -4; i <= 4; ++i)
    {
        Line3D{ Vec3{ -4, 0.01, i }, Vec3{ 4, 0.01, i } }.draw(LineColor);
        Line3D{ Vec3{ i, 0.01, -4 }, Vec3{ i, 0.01, 4 } }.draw(LineColor);
    }
    Line3D{ Vec3{ -4.1, 0.01, -4.1 }, Vec3{ 4.1, 0.01, -4.1 } }.draw(LineColor);
    Line3D{ Vec3{ -4.1, 0.01, 4.1 }, Vec3{ 4.1, 0.01, 4.1 } }.draw(LineColor);
    Line3D{ Vec3{ -4.1, 0.01, 4.1 }, Vec3{ -4.1, 0.01, -4.1 } }.draw(LineColor);
    Line3D{ Vec3{ 4.1, 0.01, 4.1 }, Vec3{ 4.1, 0.01, -4.1 } }.draw(LineColor);
}

// 盤上のインデックス (x, y, z) から Box を作成する関数
Box MakeBox(int32 x, int32 y, int32 z)
{
    return Box::FromPoints(Vec3{ (x - 4), (y + 1), (4 - z) }, Vec3{ (x - 3), y, (3 - z) });
}

// ブロックを描く関数
void DrawBlock(int32 x, int32 y, int32 z, const ColorF& color, double scale = 1.0)
{
    MakeBox(x, y, z).scaled(scale).draw(color);
}

// ブロックを描く関数
void DrawBlock(int32 x, int32 y, int32 z, const ColorF& color, const Texture& blockTexture)
{
    MakeBox(x, y, z).draw(blockTexture, color);
}


// ゲームの状態に基づいてブロックを描く関数
void DrawGame(const GameState& gameState, const Texture& blockTexture)
{
    const ColorF BlockColor1 = ColorF{ 1.0, 0.85, 0.6 }.removeSRGBCurve();
    const ColorF BlockColor2 = ColorF{ 0.4, 0.15, 0.15 }.removeSRGBCurve();

    for (int32 y = 0; y <= GameState::MaxY; ++y)
    {
        for (int32 x = 0; x < 8; ++x)
        {
            for (int32 z = 0; z < 8; ++z)
            {
                const int32 s = gameState.s[x][y][z];

                if (s == 1)
                {
                    DrawBlock(x, y, z, BlockColor1, blockTexture);
                }
                else if (s == 2)
                {
                    DrawBlock(x, y, z, BlockColor2, blockTexture);
                }
            }
        }
    }
}


Game::Game(const InitData& init)
    : IScene{ init }
{
    // 環境光を設定する
    Graphics3D::SetGlobalAmbientColor(ColorF{ 0.75, 0.75, 0.75 });

    // 太陽光を設定する
    Graphics3D::SetSunColor(ColorF{ 0.5, 0.5, 0.5 });

    // 太陽の方向を設定する
    Graphics3D::SetSunDirection(Vec3{ 0, 1, -0.3 }.normalized());

    // ゲームの状態
    gameState.s[0][0][0] = gameState.s[1][0][1] = 1;
    gameState.s[4][0][4] = gameState.s[5][0][4] = 2;

    // アクティブなボクセル
    Point activeVoxelXZ{ -1, -1 };
    
    AudioAsset(U"BGM").play();
    AudioAsset(U"bonfire").play();
}

void Game::update()
{
    if (resultButton.leftClicked()) // リザルトへ
    {
        AudioAsset(U"BGM").stop();
        AudioAsset(U"bonfire").stop();
        changeScene(State::Result);
    }
    if (walk45SecLoopButton.leftClicked()) // リザルトへ
    {
        if (AudioAsset(U"足音45秒のループ").isPlaying()) {
            AudioAsset(U"足音45秒のループ").stop();
        } else {
            AudioAsset(U"足音45秒のループ").play();
        }
    }
    if (doorOpenButton.leftClicked()) // リザルトへ
    {
        if (AudioAsset(U"牢屋の扉を開ける").isPlaying()) {
            AudioAsset(U"牢屋の扉を開ける").stop();
        } else {
            AudioAsset(U"牢屋の扉を開ける").play();
        }
    }
    if (doorCloseButton.leftClicked()) // リザルトへ
    {
        if (AudioAsset(U"牢屋の扉を閉める").isPlaying()) {
            AudioAsset(U"牢屋の扉を閉める").stop();
        } else {
            AudioAsset(U"牢屋の扉を閉める").play();
        }
    }
    
    ////////////////////////////////
    //
    //    状態の更新
    //
    ////////////////////////////////
    {
        if (not cameraController.isGrabbing())
        {
            const Ray ray = cameraController.getMouseRay();
            float minDistance = 99999.9f;

            for (int32 x = 0; x < 8; ++x)
            {
                for (int32 z = 0; z < 8; ++z)
                {
                    const int32 height = gameState.getHeight(x, z);

                    const Box box = MakeBox(x, height, z);

                    if (Optional<float> distacne = box.intersects(ray))
                    {
                        if (*distacne < minDistance)
                        {
                            minDistance = *distacne;
                            activeVoxelXZ.set(x, z);
                        }
                    }
                }
            }

            if (activeVoxelXZ != Point{ -1, -1 })
            {
                auto& voxel = gameState.s[activeVoxelXZ.x][gameState.getHeight(activeVoxelXZ.x, activeVoxelXZ.y)][activeVoxelXZ.y];

                if (MouseL.down())
                {
                    voxel = 1;
                }
                else if (MouseR.down())
                {
                    voxel = 2;
                }
            }
        }

        cameraController.update();
    }
}

void Game::draw() const
{
    ////////////////////////////////
    //
    //    3D 描画
    //
    ////////////////////////////////
    {
        {
            // renderTexture を背景色で塗りつぶし、3D 描画のレンダーターゲットに
            const ScopedRenderTarget3D target{ renderTexture.clear(backgroundColor) };

            DrawTable(tableTexture);
            DrawBoard(meshBoard);
            DrawGame(gameState, blockTexture);

            {
                // 半透明を有効に
                const ScopedRenderStates3D blend{ BlendState::OpaqueAlphaToCoverage };

                for (int32 x = 0; x < 8; ++x)
                {
                    for (int32 z = 0; z < 8; ++z)
                    {
                        const int32 height = gameState.getHeight(x, z);
                        DrawBlock(x, height, z, ColorF{ 0.2, 0.8, 0.8, 0.5 }, ((activeVoxelXZ == Point{ x, z }) ? 1.0 : 0.25));
                    }
                }
            }
        }

        Graphics3D::Flush();
        renderTexture.resolve();
        Shader::LinearToScreen(renderTexture);
    }

    ////////////////////////////////
    //
    //    2D 描画
    //
    ////////////////////////////////
    {
        if (SimpleGUI::Button(U"片づける", Vec2{ 1100, 20 }, 160))
        {
            //gameState = GameState{};
        }
        
        const Font& boldFont = FontAsset(U"Bold");

        resultButton.draw(ColorF{ 1.0, resultTransition.value() }).drawFrame(2);
        boldFont(U"リザルトへ").drawAt(36, resultButton.center(), ColorF{ 0.1 });
        
        resultButton.draw(ColorF{ 1.0, walk45SecLoopTransition.value() }).drawFrame(2);
        boldFont(U"足音45秒のループ").drawAt(36, walk45SecLoopButton.center(), ColorF{ 0.1 });
        
        resultButton.draw(ColorF{ 1.0, doorOpenTransition.value() }).drawFrame(2);
        boldFont(U"牢屋を開ける音").drawAt(36, doorOpenButton.center(), ColorF{ 0.1 });
        
        resultButton.draw(ColorF{ 1.0, doorCloseTransition.value() }).drawFrame(2);
        boldFont(U"牢屋を閉める音").drawAt(36, doorCloseButton.center(), ColorF{ 0.1 });
    }
}
