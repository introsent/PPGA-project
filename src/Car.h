#pragma once
#include "FlyFish.h"
#include "utils.h"

class Car
{
public:
	Car(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed);

	void Update(float elapsedSec);

	void Draw();

	void IncreaseSpeed();
	void DecreaseSpeed();
	float DetermineAngularVelocity(TwoBlade& radiusTwoBlade);

	void Orbit(ThreeBlade orbitPoint);

	void RotateLookAt();

	void SetStartedRotating(bool isRotating);

	void Snap();

	std::vector<Point2f>& GetCarPositions();

	void CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos);

	void Bounce(const ThreeBlade& hitPos, const TwoBlade& borderVector);


private:
	ThreeBlade m_Position;
	TwoBlade m_ForwardTwoBlade;
	TwoBlade m_SideForceTwoBlade;

	TwoBlade m_LineToOrbitAround;
	float m_Speed;

	float m_Width;
	float m_Height;
	std::vector<Point2f> m_CarPoints;
	Color4f m_Color;

	float m_TimeBouncing = 0.f;

	bool m_StartedRotating = false;
};

