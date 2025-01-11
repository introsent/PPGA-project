#pragma once
#include "FlyFish.h"
#include "utils.h"
#include "Camera.h"
#include "Border.h"
class Car
{
public:
	Car(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed);

	void Update(float elapsedSec);

	void UpdateSideForce(float elapsedSec);
	void UpdateForwardForce(float elapsedSec);
	void UpdateCarPointsLocalSpace(const Camera* cameraPtr);

	void Draw();

	void IncreaseSpeed();
	void DecreaseSpeed();
	float DetermineAngularVelocity(TwoBlade& radiusTwoBlade);

	void Orbit(ThreeBlade orbitPoint, const std::vector<Border>& bordersArray);

	void RotateLookAt();

	void SetStartedRotating(bool isRotating);

	void Snap();

	void CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos);

	void Bounce(const ThreeBlade& hitPos, const TwoBlade& borderVector);

	Point2f GetCarLocation() const;

	ThreeBlade GetCarWorldLocation() const;

	float Remap(float value, float fromLow, float fromHigh, float toLow, float toHigh) {
		return toLow + (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
	}

	bool IsPointInRange(float px, float py,
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4);

private:
	ThreeBlade m_Position;
	TwoBlade m_ForwardTwoBlade;
	TwoBlade m_SideForceTwoBlade;

	TwoBlade m_LineToOrbitAround;
	float m_Speed;

	float m_Width;
	float m_Height;
	std::vector<Point2f> m_CarPoints;
	std::vector<Point2f> m_CarPointsLocalSpace;
	Color4f m_Color;

	float m_TimeBouncing = 0.f;

	bool m_StartedRotating = false;
};

