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

	void Orbit(float elapsedSec, ThreeBlade orbitPoint);

	void RotateLookAt(ThreeBlade orbitPoint);

	void SetStartedRotating(bool isRotating);

	void Snap();

	std::vector<Point2f>& GetCarPositions();

	void CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos);

	void Reflect();


private:
	ThreeBlade m_Position;
	TwoBlade m_ForwardTwoBlade;
	TwoBlade m_LineToOrbitAround;
	float m_Speed;

	float m_Width;
	float m_Height;
	std::vector<Point2f> m_CarPoints;
	Color4f m_Color;



	bool m_StartedRotating = false;
};

