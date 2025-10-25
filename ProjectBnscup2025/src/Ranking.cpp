# include "Ranking.hpp"

Ranking::Ranking(const InitData& init)
	: IScene{ init }
{
	auto& data = getData();

	if (data.lastScore)
	{
		// ランキングを再構成
		data.highScores << data.lastScore;
		data.highScores.rsort();
		data.highScores.resize(RankingCount);

		// ランクインしていたら m_rank に順位をセット
		for (int32 i = 0; i < RankingCount; ++i)
		{
			if (data.highScores[i] == data.lastScore)
			{
				m_rank = i;
				break;
			}
		}

		data.lastScore = 0;
	}
}

void Ranking::update()
{
	if (MouseL.down())
	{
		// タイトルシーンへ
		changeScene(State::Title);
	}
}

void Ranking::draw() const
{
	Scene::SetBackground(ColorF{ 0.4, 0.6, 0.9 });
	const Font& boldFont = FontAsset(U"Bold");
	const auto& data = getData();

	boldFont(U"RANKING").drawAt(400, 60);

	// ランキングを表示
	for (int32 i = 0; i < RankingCount; ++i)
	{
		const RectF rect{ 100, (120 + i * 80), 600, 80 };

		rect.draw(ColorF{ 1.0, (1.0 - i * 0.2) });

		boldFont(data.highScores[i]).drawAt(rect.center(), ColorF{ 0.1 });

		// ランクインしていたら
		if (i == m_rank)
		{
			rect.drawFrame(2, 10, ColorF{ 1.0, 0.8, 0.2 });
		}
	}
}