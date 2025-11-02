# include "ToBeContinued.hpp"

ToBeContinued::ToBeContinued(const InitData& init)
    : IScene{ init }
{

}

void ToBeContinued::update()
{
    // ボタンのクリック処理
    if (MouseL.down())
    {
        changeScene(State::Title);
    }

    // 指定したプレイヤーインデックスの XInput コントローラを取得
    size_t playerIndex = 0;
    auto xboxController = XInput(playerIndex);
    xboxController.setLeftTriggerDeadZone();
    xboxController.setRightTriggerDeadZone();
    xboxController.setLeftThumbDeadZone();
    xboxController.setRightThumbDeadZone();

    if (xboxController.buttonA.down() || xboxController.buttonStart.down())
    {
        changeScene(State::Title);
    }
}

void ToBeContinued::draw() const
{
    Scene::SetBackground(ColorF{ 0, 0, 0 });

    const Font& boldFont = FontAsset(U"Bold");
 //   boldFont(U"To Be Continued...").drawAt(36, SCENE_WIDTH - 200, SCENE_HEIGHT - 36 - 10, ColorF{ 0.7 });
    boldFont(U"To Be Continued...").drawAt(30, SCENE_WIDTH - 180, SCENE_HEIGHT - 30 - 10, ColorF{ 0.7 });

}
