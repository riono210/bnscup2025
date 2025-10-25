# include "Common.hpp"
# include "Title.hpp"
# include "Game.hpp"
# include "ModelCheck.hpp"
//# include "Ranking.hpp"

void Main()
{
	FontAsset::Register(U"TitleFont", FontMethod::MSDF, 48, U"example/font/RocknRoll/RocknRollOne-Regular.ttf");
	FontAsset(U"TitleFont").setBufferThickness(4);

	FontAsset::Register(U"Bold", FontMethod::MSDF, 48, Typeface::Bold);

	// シーン追加
	App manager;
	manager.add<Title>(State::Title);
	manager.add<Game>(State::Game);
	manager.add<ModelCheck>(State::ModelCheck);
	//manager.add<Ranking>(State::Ranking);

	while (System::Update())
	{
		if (not manager.update())
		{
			break;
		}
	}
}