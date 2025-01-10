#include "RivalCar.h"
#include "utils.h"
#include <iostream>
#include <algorithm> 

RivalCar::RivalCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed) : m_Position(startPos), m_ForwardTwoBlade(forwardTwoBlade), m_Speed(speed)
{
	m_Width = 15.f;
	m_Height = 30.f;
	m_Color = Color4f(0.f, 0.f, 1.f, 1.f);

	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1]));
	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1]));

	CalculatePossibleDirections(0.f);
}

void RivalCar::UpdateForwardForce(float elapsedSec)
{
	Motor forwardTranslator = Motor::Translation(m_Speed * elapsedSec, m_ForwardTwoBlade);

	for (Point2f& pos : m_CarPoints)
	{
		ThreeBlade position = (forwardTranslator * ThreeBlade(pos.x, pos.y, 0.f) * ~forwardTranslator).Grade3();
		pos = Point2f(position[0], position[1]);
	}
}

void RivalCar::Draw() const
{
	utils::SetColor(m_Color);

	utils::FillPolygon(m_CarPointsLocalSpace);

	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));

	Point2f startLinePos = Point2f((m_CarPointsLocalSpace[0].x + m_CarPointsLocalSpace[2].x) / 2.f, (m_CarPointsLocalSpace[0].y + m_CarPointsLocalSpace[2].y) / 2.f);
	for (const auto& direction : m_PossibleDirections)
	{
		Point2f endLinePos = Point2f(startLinePos.x + direction[0], startLinePos.y + direction[1]);
		utils::DrawLine(startLinePos, endLinePos, 1.f);
	}
}

void RivalCar::RotateLookAt()
{

	TwoBlade normalizedForwardTwoBlade = m_ForwardTwoBlade / m_ForwardTwoBlade.VNorm();
	m_ForwardTwoBlade = normalizedForwardTwoBlade;

	TwoBlade carDirection = TwoBlade::LineFromPoints(m_CarPoints[0].x, m_CarPoints[0].y, 0.f, m_CarPoints[1].x, m_CarPoints[1].y, 0.f).Normalized();
	TwoBlade forwardDirection = TwoBlade(m_ForwardTwoBlade[3], m_ForwardTwoBlade[4], m_ForwardTwoBlade[5], m_ForwardTwoBlade[0], m_ForwardTwoBlade[1], m_ForwardTwoBlade[2]).Normalized();

	float angleBetween = acosf(std::clamp(-forwardDirection | carDirection, -1.0f, 1.0f));

	if (angleBetween > utils::g_Pi / 4.f)
	{
		angleBetween = utils::g_Pi / 2.f - angleBetween;
	}

	ThreeBlade carMiddlePoint = ThreeBlade((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f, 0.f);
	TwoBlade originToOrbitPoint = TwoBlade(carMiddlePoint[0], carMiddlePoint[1], carMiddlePoint[2], 0.f, 0.f, 0.f);
	float distance = originToOrbitPoint.VNorm();


	auto perpDot = forwardDirection[3] * carDirection[4] - forwardDirection[4] * carDirection[3];

	TwoBlade rotationLine;
	if (perpDot > 0)
	{
		rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	}
	else if (perpDot <= 0)
	{
		rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, -1.f);
	}

	//TwoBlade rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);

	Motor rotation = Motor::Rotation(-angleBetween * 180.f / utils::g_Pi, rotationLine);

	Motor translator = Motor::Translation(distance, originToOrbitPoint);

	Motor rotor = (translator * rotation * ~translator);

	for (Point2f& pos : m_CarPoints)
	{
		ThreeBlade position = (rotor * ThreeBlade(pos.x, pos.y, 0.f) * ~rotor).Grade3();
		pos = Point2f(position[0], position[1]);
	}

	float startAngleRad = atan2f(-m_ForwardTwoBlade[0], m_ForwardTwoBlade[1]);
	float startAngleDeg = startAngleRad * (180.f / utils::g_Pi);
	CalculatePossibleDirections(startAngleDeg);
}

void RivalCar::CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray)
{
	std::vector<int> indicesOfPossibleDirectionArray;

	ThreeBlade point1 = ThreeBlade((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f, 0.f);
	int currentInteration = 0;
	for (const auto& direction : m_PossibleDirections)
	{
		ThreeBlade point2 = ThreeBlade(point1[0] + direction[0], point1[1] + direction[1], 0.f);

		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);
		TwoBlade curTwoBladeNoVanishing = ThreeBlade(curTwoBlade[3], curTwoBlade[4], curTwoBlade[5]) & ThreeBlade(0.f, 0.f, 0.f);


		bool isPossible = true;
		for (const auto& border : bordersArray)
		{
			TwoBlade borderTwoBladeNoVanishing = ThreeBlade(border.borderDirection[3], border.borderDirection[4], border.borderDirection[5]) & ThreeBlade(0.f, 0.f, 0.f);
			ThreeBlade skewTest = (curTwoBladeNoVanishing ^ borderTwoBladeNoVanishing).Grade3();

			ThreeBlade point;

			if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
			{
				TwoBlade commonNormal = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
				ThreeBlade NPoint = (commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f));
				OneBlade commonPlane = NPoint & curTwoBlade.Normalized();

				point = (commonPlane ^ border.borderDirection).Normalized();


				

				if (
					(std::min(point1[0], point2[0]) <= point[0] && point[0] <= std::max(point1[0], point2[0]) &&
						std::min(point1[1], point2[1]) <= point[1] && point[1] <= std::max(point1[1], point2[1]))
					&&
					(std::min(border.startPosition[0], border.endPosition[0]) <= point[0] && point[0] <= std::max(border.startPosition[0], border.endPosition[0]) &&
						std::min(border.startPosition[1], border.endPosition[1]) <= point[1] && point[1] <= std::max(border.startPosition[1], border.endPosition[1]))

					)
				{
					TwoBlade carToIntersectionPoint = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point[0], point[1], point[2]);
					float distance = carToIntersectionPoint.Norm();

					if (distance <= curTwoBlade.Norm())
					{
						isPossible = false;
						break;

					}
				}
			}
		}

		if (isPossible)
		{
			indicesOfPossibleDirectionArray.push_back(currentInteration);
		}
		currentInteration += 1;
	}

	// Find the minimum value

	if (IsPossibleDirectionVectorContiguous(indicesOfPossibleDirectionArray))
	{
		auto minIt = std::min_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
		int minValue = (minIt != indicesOfPossibleDirectionArray.end()) ? *minIt : 0;

		// Find the maximum value
		auto maxIt = std::max_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
		int maxValue = (maxIt != indicesOfPossibleDirectionArray.end()) ? *maxIt : 0;

		int desiredDirectionIndex = (minValue + maxValue) / 2; //integer division on purpose

		m_ForwardTwoBlade = m_PossibleDirections[desiredDirectionIndex];

		RotateLookAt();
	}
	
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

void RivalCar::UpdateCarPointsLocalSpace(const Camera* cameraPtr)
{
	m_CarPointsLocalSpace.clear();
	for (const Point2f& carPoint : m_CarPoints)
	{
		m_CarPointsLocalSpace.push_back(cameraPtr->GetAppliedTransform(carPoint));
	}
}

bool RivalCar::IsPossibleDirectionVectorContiguous(const std::vector<int>& vec) {
	if (vec.size() < 2) return true;
	auto it = std::adjacent_find(vec.begin(), vec.end(), [](int a, int b) {
		return b != a + 1;
		});

	return it == vec.end(); 
}
