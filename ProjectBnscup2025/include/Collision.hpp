# pragma once
#include "Common.hpp"

struct Vec3_ {	// TODO Vec3を継承する
	double x, y, z;

	Vec3_ operator-(const Vec3_& v) const { return { x - v.x, y - v.y, z - v.z }; }
	Vec3_ operator+(const Vec3_& v) const { return { x + v.x, y + v.y, z + v.z }; }
	Vec3_ operator*(double s) const { return { x * s, y * s, z * s }; }

	static Vec3_ cross(const Vec3_& a, const Vec3_& b) {
		return {
			a.y * b.z - a.z * b.y,
			a.z * b.x - a.x * b.z,
			a.x * b.y - a.y * b.x
		};
	}

	static double dot(const Vec3_& a, const Vec3_& b) {
		return a.x * b.x + a.y * b.y + a.z * b.z;
	}

	double dot(const Vec3_& rhs) const { return x * rhs.x + y * rhs.y + z * rhs.z; }

	double lengthSq() const { return x * x + y * y + z * z; }
};

// レイ（三角形との交差を調べる）
struct Ray_ {	// TODO Rayを継承してみる
	Vec3_ origin;
	Vec3_ direction;
};

// 交差点の情報
struct HitResult {
	double t;       // レイのパラメータ（距離）
	double u, v;    // バリセントリック座標
	Vec3_ point;    // 交差点
};

struct Segment {
	Vec3_ start;
	Vec3_ end;
};

// 三角形（頂点3つ）
using Triangle_ = std::array<Vec3_, 3>;

// ベクトルの外積（2D）
double cross(
	const Vec2& a,
	const Vec2& b
);

// 点Cが線分ABのどちら側にあるか
double direction(
	const Vec2& a,
	const Vec2& b,
	const Vec2& c
);

// 線分同士が交差しているかを判定
bool isIntersecting(
	const Vec2& a,
	const Vec2& b,
	const Vec2& c,
	const Vec2& d
);

// 2Dの線分同士の交差判定
bool isCollision(
	const Vec2& a,
	const Vec2& b,
	double* collisionList
);

// 3D空間上の線分と三角形が交差しているかどうかを判定する関数
bool IntersectSegmentTriangle(
	const Segment& segment,
	const Triangle_& tri,
	HitResult& outResult
);

// 最近点を三角形上に求める（Möllerのアルゴリズム系）
Vec3_ ClosestPointOnTriangle(
	const Vec3_& p,
	const Vec3_& a,
	const Vec3_& b,
	const Vec3_& c
);

// 球と三角形の交差判定
bool IsSphereIntersectingTriangle(
	const Vec3_& sphereCenter,
	double radius,
	const Vec3_& a,
	const Vec3_& b,
	const Vec3_& c
);
