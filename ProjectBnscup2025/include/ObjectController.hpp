# pragma once
#include "Common.hpp"

class ObjectController
{
public:
	ObjectController();

	std::tuple<bool, bool, int, bool> update(
		const Vec3& objPos,
		const BasicCamera3D& camera,
		const Vec3& eyePos,
	//	Ray ray,
		Vec3& MarkPosition,
		int bgmNo,
		bool bHave
	);

private:

	bool m_isHave;

	void playSE(String SE);

};
