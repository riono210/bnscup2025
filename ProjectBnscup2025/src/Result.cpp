# include "Result.hpp"

Result::Result(const InitData& init)
    : IScene{ init }
{

}

void Result::update()
{
    // ボタンの更新
    {
        titleTransition.update(titleButton.mouseOver());

        if (titleButton.mouseOver())
        {
            Cursor::RequestStyle(CursorStyle::Hand);
        }
    }

    // ボタンのクリック処理
    if (titleButton.leftClicked()) // リザルトへ
    {
        changeScene(State::Title);
    }
}

void Result::draw() const
{
    Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

    // ボタン描画
    {
        titleButton.draw(ColorF{ 1.0, titleTransition.value() }).drawFrame(2);
        
        const Font& boldFont = FontAsset(U"Bold");
        boldFont(U"TITLE").drawAt(36, titleButton.center(), ColorF{ 0.1 });
    }
}
