#pragma once
#include "FlyFish.h"
#include "utils.h"
#include "Camera.h"
#include "Border.h"
class RivalCar
{
public:
	RivalCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed);

	//void Update(float elapsedSec);

	//void UpdateSideForce(float elapsedSec);
	void UpdateForwardForce(float elapsedSec);
	//void UpdateCarPointsLocalSpace(const Camera* cameraPtr);

	void Draw() const;

	//void IncreaseSpeed();
	//void DecreaseSpeed();
	//float DetermineAngularVelocity(TwoBlade& radiusTwoBlade);
	//
	//void Orbit(ThreeBlade orbitPoint);
	//
	void RotateLookAt();

	//void SetStartedRotating(bool isRotating);
	//
	//void Snap();
	//
	void CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray);
	//
	//void Bounce(const ThreeBlade& hitPos, const TwoBlade& borderVector);
	//
	//ThreeBlade GetCarWorldLocation() const;

	void CalculatePossibleDirections(float startAngle);
	void UpdateCarPointsLocalSpace(const Camera* cameraPtr);


private:
	ThreeBlade m_Position;
	TwoBlade m_ForwardTwoBlade;
	//TwoBlade m_SideForceTwoBlade;
	//
	//TwoBlade m_LineToOrbitAround;
	float m_Speed;

	float m_Width;
	float m_Height;
	std::vector<Point2f> m_CarPoints;
	std::vector<Point2f> m_CarPointsLocalSpace;

	std::vector<TwoBlade> m_PossibleDirections;
	Color4f m_Color;


	float m_DirectionTwoBladeSize = 90.f;

	int m_AmountOfPossibleDirection = 20;

	float m_SegmentLength = 360.f / m_AmountOfPossibleDirection;
	//
	//float m_TimeBouncing = 0.f;
	//
	//	bool m_StartedRotating = false;

};

