# include "ModelCheck.hpp"

ModelCheck::ModelCheck(const InitData& init)
	: IScene{ init }
{

}

void ModelCheck::update()
{
	if (KeyEnter.down())
	{
		changeScene(State::Title);
	}
}

void ModelCheck::draw() const
{

}