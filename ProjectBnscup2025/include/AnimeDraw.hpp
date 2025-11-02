# pragma once
#include "Common.hpp"

class animeDraw {
private:
	Array<TextureRegion> _animeTexture;
	uint64 _baseTime;
	uint64 _effectTime;
	const Mesh billboard{ MeshData::Billboard() };
	float _color;

public:

//	animeDraw(const Texture& texture, const int32& x, const int32& y);

	void startDraw(
		const uint64& effecttime,
		const Texture& texture,
		const int32& x,
		const int32& y
	);

	void draw(BasicCamera3D camera, Vec3 pos, float scale, float color);

};

//extern animeDraw Fire;
 