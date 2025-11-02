#include "AnimeDraw.hpp"

// ’g˜F‚Ì‰Š
//animeDraw::animeDraw(const Texture& texture, const int32& x, const int32& y) :
//	_effectTime(1000),
//	_baseTime(Time::GetMillisec())
//{
//	assert(x >= 0 && y >= 0 && !texture.isEmpty());
//
//	for (int32 ny = 0; ny < y; ny++) {
//		for (int32 nx = 0; nx < x; nx++) {
//			_animeTexture.push_back(texture.uv(1.0 / x * nx, 1.0 / y * ny, 1.0 / x, 1.0 / y));
//		}
//	}
//}

void animeDraw::startDraw(
	const uint64& effecttime,
	const Texture& texture, 
	const int32& x,
	const int32& y
) {
	_effectTime = effecttime;
	_baseTime = Time::GetMillisec();
	_color = 0;

	assert(x >= 0 && y >= 0 && !texture.isEmpty());

	for (int32 ny = 0; ny < y; ny++) {
		for (int32 nx = 0; nx < x; nx++) {
			_animeTexture.push_back(texture.uv(1.0 / x * nx, 1.0 / y * ny, 1.0 / x, 1.0 / y));
		}
	}
}

void animeDraw::draw(BasicCamera3D camera, Vec3 pos, float scale, float color) {

	size_t index = 0;
	for (size_t i = 0; i < _animeTexture.size(); i++) {
		if ((Time::GetMillisec() - _baseTime) >= _effectTime / _animeTexture.size() * i) {
			index = i;
		}
	}

	//_color = Math::Sin(Time::GetMillisec());
	_color = Periodic::Sine0_1(2s) * 0.2;	// TODO ’g˜F‚ÌŒõ‚Æ‡‚í‚¹‚é

	billboard.draw(
		camera.billboard(pos, scale),
		_animeTexture[index],
		ColorF{ color + _color }
	);

	if ((Time::GetMillisec() - _baseTime) > _effectTime)
	{
		// ‰Šú‰»
		_effectTime = 1000;
		_baseTime = Time::GetMillisec();
	}
	return;
}
