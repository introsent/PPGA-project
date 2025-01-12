#include "Camera.h"
#include <stack>
#include "FlyFish.h"
Camera::Camera(float screenWidth, float screenHeight)
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_FinalTransform = Motor(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
}

ThreeBlade Camera::GetAppliedTransform(const ThreeBlade& pointToTransform) const
{
	ThreeBlade transformedPoint = (m_FinalTransform * pointToTransform * ~m_FinalTransform).Grade3();

	float x, y;
	x = std::isnan(transformedPoint[0]) ? pointToTransform[0] : transformedPoint[0];
	y = std::isnan(transformedPoint[1]) ? pointToTransform[1] : transformedPoint[1];

	return ThreeBlade(x, y, 0.f);
}

ThreeBlade Camera::GetWorldLocation(const ThreeBlade& pointInLocalSpace) const
{
	float transformX, transformY;
	transformX = std::isnan(m_FinalTransform[1]) ? 0.f : m_FinalTransform[1];
	transformY = std::isnan(m_FinalTransform[2]) ? 0.f : m_FinalTransform[2];

	Motor translation = Motor(m_FinalTransform[0], -transformX, -transformY, m_FinalTransform[3], m_FinalTransform[4], m_FinalTransform[5], m_FinalTransform[6], m_FinalTransform[7]);
	ThreeBlade result = (translation * pointInLocalSpace * ~translation).Grade3();

	float x, y;
	x = std::isnan(result[0]) ? pointInLocalSpace[0] : result[0];
	y = std::isnan(result[1]) ? pointInLocalSpace[1] : result[1];

	return ThreeBlade(x, y, 0.f);
}

void Camera::Aim(float levelW, float levelH, ThreeBlade trackCenter)
{
	float cameraY{ trackCenter[1] - m_ScreenHeight / 2.f};
	float cameraX{ trackCenter[0] - m_ScreenWidth / 2.f};

	if (cameraY < 0) cameraY = 0;
	else if (cameraY > levelH - m_ScreenHeight) cameraY = levelH - m_ScreenHeight;

	if (cameraX < 0) cameraX = 0;
	else if (cameraX > levelW - m_ScreenWidth) cameraX = levelW - m_ScreenWidth;

	TwoBlade cameraTranslationVector = TwoBlade(-cameraX, -cameraY, 0.f, 0.f, 0.f, 0.f);
	m_FinalTransform = Motor::Translation(cameraTranslationVector.VNorm(), cameraTranslationVector);
}

void Camera::Reset()
{
	m_FinalTransform = Motor();
}
