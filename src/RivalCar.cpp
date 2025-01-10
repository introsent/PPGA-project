#include "RivalCar.h"
#include "utils.h"
#include <iostream>

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

	Point2f startLinePos = Point2f((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f);
	for (const auto& direction : m_PossibleDirections)
	{
		Point2f endLinePos = Point2f(startLinePos.x + direction[0], startLinePos.y + direction[1]);
		utils::DrawLine(startLinePos, endLinePos, 1.f);
	}
}

void RivalCar::RotateLookAt()
{
	m_CarPoints[0] = Point2f(m_CarPoints[0].x, m_CarPoints[0].y);

	TwoBlade normalizedForwardTwoBlade = m_ForwardTwoBlade / m_ForwardTwoBlade.VNorm();
	m_ForwardTwoBlade = normalizedForwardTwoBlade;

	TwoBlade carDirection = TwoBlade::LineFromPoints(m_CarPoints[0].x, m_CarPoints[0].y, 0.f, m_CarPoints[1].x, m_CarPoints[1].y, 0.f).Normalized();
	TwoBlade forwardDirection = TwoBlade(m_ForwardTwoBlade[3], m_ForwardTwoBlade[4], m_ForwardTwoBlade[5], m_ForwardTwoBlade[0], m_ForwardTwoBlade[1], m_ForwardTwoBlade[2]).Normalized();

	float angleBetween = acosf(-carDirection | forwardDirection);


	ThreeBlade carMiddlePoint = ThreeBlade((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f, 0.f);
	TwoBlade originToOrbitPoint = TwoBlade(carMiddlePoint[0], carMiddlePoint[1], carMiddlePoint[2], 0.f, 0.f, 0.f);
	float distance = originToOrbitPoint.VNorm();
	TwoBlade rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	
	Motor rotation = Motor::Rotation(angleBetween * 180.f / utils::g_Pi, rotationLine);
	
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
	auto minIt = std::min_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
	int minValue = (minIt != indicesOfPossibleDirectionArray.end()) ? *minIt : 0;

	// Find the maximum value
	auto maxIt = std::max_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
	int maxValue = (maxIt != indicesOfPossibleDirectionArray.end()) ? *maxIt : 0;

	int desiredDirectionIndex = (minValue + maxValue) / 2; //integer division on purpose

	m_ForwardTwoBlade = m_PossibleDirections[desiredDirectionIndex];

	RotateLookAt();

}

//void RivalCar::CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos)
//{
//	std::vector<int> indicesOfPossibleDirectionArray;
//
//	ThreeBlade point1 = ThreeBlade((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f, 0.f);
//	int currentInteration = 0;
//	for (const auto& direction : m_PossibleDirections)
//	{
//		//ThreeBlade point1 = ThreeBlade(m_CarPoints[i % m_CarPoints.size()].x, m_CarPoints[i % m_CarPoints.size()].y, 0.f);
//		ThreeBlade point2 = ThreeBlade(point1[0] + direction[0], point1[1] + direction[1], 0.f);
//
//		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);
//
//		TwoBlade curTwoBladeNoVanishing = ThreeBlade(curTwoBlade[3], curTwoBlade[4], curTwoBlade[5]) & ThreeBlade(0.f, 0.f, 0.f);
//		TwoBlade borderTwoBladeNoVanishing = ThreeBlade(border[3], border[4], border[5]) & ThreeBlade(0.f, 0.f, 0.f);
//
//		ThreeBlade skewTest = (curTwoBladeNoVanishing ^ borderTwoBladeNoVanishing).Grade3();
//
//		ThreeBlade point;
//
//		if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
//		{
//			TwoBlade commonNormal = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
//			ThreeBlade NPoint = commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f);
//			OneBlade commonPlane = NPoint & curTwoBlade;
//
//			point = (commonPlane ^ border).Normalize();
//		}
//
//		TwoBlade carToIntersectionPoint = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point[0], point[1], point[2]);
//		float distance = carToIntersectionPoint.VNorm();
//
//		if (distance > curTwoBlade.VNorm())
//		{
//			indicesOfPossibleDirectionArray.push_back(currentInteration);
//		}
//		currentInteration += 1;
//	}
//
//	// Find the minimum value
//	auto minIt = std::min_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
//	int minValue = (minIt != indicesOfPossibleDirectionArray.end()) ? *minIt : 0;
//
//	// Find the maximum value
//	auto maxIt = std::max_element(indicesOfPossibleDirectionArray.begin(), indicesOfPossibleDirectionArray.end());
//	int maxValue = (maxIt != indicesOfPossibleDirectionArray.end()) ? *maxIt : 0;
//
//	int desiredDirectionIndex = (minValue + maxValue) / 2; //integer division on purpose
//
//	m_ForwardTwoBlade = m_PossibleDirections[desiredDirectionIndex];
//
//	RotateLookAt();
//	
//	//std::cout << "gheello.Ha llo goeden dag meneer ik kom met de request vcan vandaag om eer voor te zorgen dat we de std library vermoorden dat wat vind ik erg fijn want die shit is fakking bull shit " << std::begin;
//}

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
