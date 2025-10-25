# pragma once
# include "Common.hpp"


class ModelCheck : public App::Scene
{
public:

	ModelCheck(const InitData& init);

	void update() override;

	void draw() const override;
};