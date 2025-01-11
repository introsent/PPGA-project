#pragma once
#include "FlyFish.h"
#include "utils.h"
#include "Camera.h"
#include "Border.h"
class RivalCar
{
public:
	RivalCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed, Color4f color);
	void UpdateForwardForce(float elapsedSec);


	void Draw() const;
	void RotateLookAt();
	void CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray);

	void CalculatePossibleDirections(float startAngle);
	void UpdateCarPointsLocalSpace(const Camera* cameraPtr);

	bool IsPossibleDirectionVectorContiguous(const std::vector<int>& vec);

	//bool CheckFinished(float elapsedSec);
	bool IsPointInRange(float px, float py,
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4);

private:
	ThreeBlade m_Position;
	TwoBlade m_ForwardTwoBlade;

	float m_Speed;

	float m_Width;
	float m_Height;
	std::vector<Point2f> m_CarPoints;
	std::vector<Point2f> m_CarPointsLocalSpace;

	std::vector<TwoBlade> m_PossibleDirections;
	Color4f m_Color;


	float m_DirectionTwoBladeSize = 90.f;

	int m_AmountOfPossibleDirection = 50;

	float m_SegmentLength = 360.f / m_AmountOfPossibleDirection;

};

