#pragma once
#include <Siv3D.hpp>
#include <btBulletDynamicsCommon.h>

inline Vec3 fromBTVec3(const btVector3& v) {
	return { v.x(), v.y(), v.z() };
}
inline btVector3 toBTVec3(const Vec3& v) {
	return { v.x, v.y, v.z };
}

inline Quaternion fromBTQuat(const btQuaternion& q) {
	return { q.x(), q.y(), q.z(), q.w()	};
}

inline btQuaternion toBTQuat(const Quaternion& q) {
	return { q.getX(), q.getY(), q.getZ(), q.getW() };
}
