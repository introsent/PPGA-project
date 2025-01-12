#pragma once
#include "FlyFish.h"
#include "utils.h"
#include "Camera.h"
#include "Border.h"
#include "BaseCar.h"
class RivalCar final : public BaseCar
{
public:
	RivalCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed, Color4f color);

	void Draw() const override;
	void RotateLookAt() override;
	void CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray) override;

	void CalculatePossibleDirections(float startAngle);

	bool IsPossibleDirectionVectorContiguous(const std::vector<int>& vec);

private:
	std::vector<TwoBlade> m_PossibleDirections;

	float m_DirectionTwoBladeSize = 90.f;
	int m_AmountOfPossibleDirection = 50;
	float m_SegmentLength = 360.f / m_AmountOfPossibleDirection;

};

