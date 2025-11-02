# pragma once
# include "Common.hpp"

// リザルトシーン
class ToBeContinued : public App::Scene
{
public:

    ToBeContinued(const InitData& init);

    void update() override;

    void draw() const override;

private:

    RoundRect titleButton{ Arg::center(640, 300), 300, 60, 8 };

    Transition titleTransition{ 0.4s, 0.2s };
};
