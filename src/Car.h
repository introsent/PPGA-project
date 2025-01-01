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

	void Orbit(float elapsedSec, ThreeBlade orbitPoint);

	void RotateLookAt(ThreeBlade orbitPoint);

	//void SetRotating(bool isRotating);

private:
	ThreeBlade m_Position;
	TwoBlade m_ForwardTwoBlade;
	float m_Speed;

	float m_Width;
	float m_Height;
	std::vector<Point2f> m_CarPoints;
	Color4f m_Color;

	bool m_IsRotating = false;
};

