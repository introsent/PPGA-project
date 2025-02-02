#include "RivalCar.h"
#include "utils.h"
#include <iostream>
#include <algorithm> 
#include "GizmosDrawer.h"
#include <cmath>

RivalCar::RivalCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed, Color4f color) : BaseCar(startPos ,forwardTwoBlade, speed)
{
	m_Color = color;

	CalculatePossibleDirections(0.f);
}

void RivalCar::Draw() const
{
	BaseCar::Draw();

	utils::SetColor(Color4f(1.f, 0.f, 0.f, 0.35f));
	Point2f startLinePos = Point2f((m_CarPointsLocalSpace[0][0] + m_CarPointsLocalSpace[2][0]) / 2.f, (m_CarPointsLocalSpace[0][1] + m_CarPointsLocalSpace[2][1]) / 2.f);
	for (const auto& direction : m_PossibleDirections)
	{
		Point2f endLinePos = Point2f(startLinePos.x + direction[0], startLinePos.y + direction[1]);
		utils::DrawLine(startLinePos, endLinePos, 1.f);
	}
}

void RivalCar::RotateLookAt()
{
	float currentSpeed = m_ForwardTwoBlade[2];
	m_ForwardTwoBlade[2] = 0;

	TwoBlade normalizedForwardTwoBlade = m_ForwardTwoBlade / m_ForwardTwoBlade.VNorm();
	m_ForwardTwoBlade = normalizedForwardTwoBlade;


	TwoBlade carDirection = TwoBlade::LineFromPoints(m_CarPointsWorldSpace[0][0], m_CarPointsWorldSpace[0][1], 0.f, m_CarPointsWorldSpace[1][0], m_CarPointsWorldSpace[1][1], 0.f).Normalized();
	TwoBlade forwardDirection = TwoBlade(m_ForwardTwoBlade[3], m_ForwardTwoBlade[4], m_ForwardTwoBlade[5], m_ForwardTwoBlade[0], m_ForwardTwoBlade[1], m_ForwardTwoBlade[2]).Normalized();

	float angleBetween = acosf(std::clamp(-forwardDirection | carDirection, -1.0f, 1.0f));
 
	ThreeBlade carMiddlePoint = ThreeBlade((m_CarPointsWorldSpace[0][0] + m_CarPointsWorldSpace[2][0]) / 2.f, (m_CarPointsWorldSpace[0][1] + m_CarPointsWorldSpace[2][1]) / 2.f, 0.f);
	TwoBlade originToOrbitPoint = TwoBlade(carMiddlePoint[0], carMiddlePoint[1], carMiddlePoint[2], 0.f, 0.f, 0.f);
	float distance = originToOrbitPoint.VNorm();


	TwoBlade planeNormal = TwoBlade(0, 0, 0, 0, 0, 1);
	TwoBlade carSideTwoBlade = (planeNormal * forwardDirection).Grade2(); //Cross between norm of plane and line to find a line perpendicular to m_ForwardTwoBlade

	float dotProduct = carSideTwoBlade | carDirection;

	TwoBlade rotationLine;
	if (dotProduct > 0)
	{
		rotationLine = -planeNormal;
	}
	else if (dotProduct <= 0)
	{
		rotationLine = planeNormal;
	}

	Motor rotation = Motor::Rotation(angleBetween * 180.f / utils::g_Pi, rotationLine);

	Motor translator = Motor::Translation(distance, originToOrbitPoint);

	Motor rotor = (translator * rotation * ~translator);

	for (ThreeBlade& worldPos : m_CarPointsWorldSpace)
	{
		worldPos = (rotor * worldPos * ~rotor).Grade3();
	}

	float startAngleRad = atan2f(-m_ForwardTwoBlade[0], m_ForwardTwoBlade[1]);
	float startAngleDeg = startAngleRad * (180.f / utils::g_Pi);

	m_ForwardTwoBlade[2] = currentSpeed;
	CalculatePossibleDirections(startAngleDeg);
}

void RivalCar::CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray)
{
	std::vector<int> indicesOfPossibleDirectionArray;

	ThreeBlade point1 = ThreeBlade((m_CarPointsWorldSpace[0] + m_CarPointsWorldSpace[2]) / 2.f);

	int currentInteration = 0;
	for (const auto& direction : m_PossibleDirections)
	{
		ThreeBlade point2 = ThreeBlade(point1[0] + direction[0], point1[1] + direction[1], 0.f);

		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);
		TwoBlade curTwoBladeNoVanishing = ThreeBlade(curTwoBlade[3], curTwoBlade[4], curTwoBlade[5]) & ThreeBlade(0.f, 0.f, 0.f);

		bool isPossible = true;
		for (int index{ 0 }; index < bordersArray.size(); ++index)
		{
			if (index == 23) continue;
			Border border = bordersArray[index];

			TwoBlade borderTwoBladeNoVanishing = ThreeBlade(border.borderDirection[3], border.borderDirection[4], border.borderDirection[5]) & ThreeBlade(0.f, 0.f, 0.f);
			ThreeBlade skewTest = (curTwoBladeNoVanishing ^ borderTwoBladeNoVanishing).Grade3();

			ThreeBlade point;

			if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
			{
				TwoBlade commonNormal = (curTwoBlade.Normalized() * border.borderDirection).Grade2();
				ThreeBlade NPoint = (commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f));
				OneBlade commonPlane = NPoint & curTwoBlade.Normalized();

				point = (commonPlane ^ border.borderDirection).Normalize();

				// Check bounds
				if (IsPointInRange(point[0], point[1],
					point1[0], point1[1],
					point2[0], point2[1],
					border.startPosition[0], border.startPosition[1],
					border.endPosition[0], border.endPosition[1]))

				{
					isPossible = false;
					break;
				}
			}
		}

		if (isPossible)
		{
			indicesOfPossibleDirectionArray.push_back(currentInteration);
		}
		currentInteration += 1;
	}

	
	if (IsPossibleDirectionVectorContiguous(indicesOfPossibleDirectionArray))
	{
		// Find the minimum value
		auto minIt = std::min_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
		int minValue = (minIt != indicesOfPossibleDirectionArray.end()) ? *minIt : 0;

		// Find the maximum value
		auto maxIt = std::max_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
		int maxValue = (maxIt != indicesOfPossibleDirectionArray.end()) ? *maxIt : int(indicesOfPossibleDirectionArray.size());

		int desiredDirectionIndex = (minValue + maxValue) / 2; //integer division on purpose

		float currentSpeed = m_ForwardTwoBlade[2];
		m_ForwardTwoBlade = m_PossibleDirections[desiredDirectionIndex];
		m_ForwardTwoBlade[2] = currentSpeed;
		
	}
	RotateLookAt();
}

void RivalCar::CalculatePossibleDirections(float startAngle)
{
	m_PossibleDirections.clear();
	for (float angle{ startAngle }; angle <= 180.f + startAngle; )
	{
		m_PossibleDirections.push_back(TwoBlade(float(int(m_DirectionTwoBladeSize * cosf(angle * utils::g_Pi / 180.f))), float(int(m_DirectionTwoBladeSize * sinf(angle * utils::g_Pi / 180.f))), 0.f, 0.f, 0.f, 0.f));
		angle += m_SegmentLength;
	}
}

bool RivalCar::IsPossibleDirectionVectorContiguous(const std::vector<int>& vec) {
	if (vec.size() < 2) return true;
	auto it = std::adjacent_find(vec.begin(), vec.end(), [](int a, int b) {
		return b != a + 1;
		});

	return it == vec.end(); 
}
