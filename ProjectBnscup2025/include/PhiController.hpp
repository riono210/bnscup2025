# pragma once
#include <Siv3D.hpp>

class PhiController
{
private:
	Vec3 m_cameraPosition;
	Vec3 m_focusPosition;
	double m_phi = 0.0;

public:
	PhiController(const Vec3& eye, const Vec3& focus);

	void updatePhi();

	void rotate(
		double deltaDegrees,
		double deltaTime,
		double thumb
	);

	double getPhi() const;

	void setCameraPosition(const Vec3& eye);

	void setFocusPosition(const Vec3& focus);
};
