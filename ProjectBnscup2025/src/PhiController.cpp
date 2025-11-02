#include "PhiController.hpp"

PhiController::PhiController(const Vec3& eye, const Vec3& focus)
	: m_cameraPosition(eye), m_focusPosition(focus)
{
	updatePhi();
}

void PhiController::updatePhi()
{
	m_phi = std::atan2(
		(m_focusPosition.z - m_cameraPosition.z),
		(m_focusPosition.x - m_cameraPosition.x)
	);
}

void PhiController::rotate(
	double deltaDegrees,
	double deltaTime,
	double thumb
)
{
	m_phi += deltaDegrees * deltaTime * thumb;

	// 正規化
	if (m_phi < 0.0)
	{
		m_phi += 360_deg;
	}
	else if (m_phi >= 360.0)
	{
		m_phi -= 360_deg;
	}
}

double PhiController::getPhi() const
{
	return m_phi;
}

// 座標
void PhiController::setCameraPosition(const Vec3& cameraPos)
{
	m_cameraPosition = cameraPos;
	updatePhi();
}

// 視点の向き
void PhiController::setFocusPosition(const Vec3& focus)
{
	m_focusPosition = focus;
	updatePhi();
}
