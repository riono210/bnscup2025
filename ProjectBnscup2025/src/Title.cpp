# include "Title.hpp"

Title::Title(const InitData& init)
	: IScene{ init }
{

}

void Title::update()
{
	// Button updates
	{
		m_startTransition.update(m_startButton.mouseOver());
		m_modelCheckTransition.update(m_modelCheckButton.mouseOver());
		m_exitTransition.update(m_exitButton.mouseOver());

		if (m_startButton.mouseOver() || m_exitButton.mouseOver())
		{
			Cursor::RequestStyle(CursorStyle::Hand);
		}
	}

	// Button click handling
	if (m_startButton.leftClicked())
	{
		changeScene(State::Game);
	}
	else if (m_modelCheckButton.leftClicked())
	{
		changeScene(State::ModelCheck);
	}
	else if (m_exitButton.leftClicked())
	{
		System::Exit();
	}
}

void Title::draw() const
{
	Scene::SetBackground(ColorF{ 0.2, 0.8, 0.4 });

	// Title drawing
	FontAsset(U"TitleFont")(U"BREAKOUT")
		.drawAt(TextStyle::OutlineShadow(0.2, ColorF{ 0.2, 0.6, 0.2 }, Vec2{ 3, 3 }, ColorF{ 0.0, 0.5 }), 100, Vec2{ 400, 100 });

	// Button drawing
	{
		m_startButton.draw(ColorF{ 1.0, m_startTransition.value() }).drawFrame(2);
		m_modelCheckButton.draw(ColorF{ 1.0, m_modelCheckTransition.value() }).drawFrame(2);
		m_exitButton.draw(ColorF{ 1.0, m_exitTransition.value() }).drawFrame(2);

		const Font& boldFont = FontAsset(U"Bold");
		boldFont(U"PLAY").drawAt(36, m_startButton.center(), ColorF{ 0.1 });
		boldFont(U"MODEL CHECK").drawAt(36, m_modelCheckButton.center(), ColorF{ 0.1 });
		boldFont(U"EXIT").drawAt(36, m_exitButton.center(), ColorF{ 0.1 });
	}
}