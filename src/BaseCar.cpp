#include "BaseCar.h"
#include "utils.h"
#include <numbers>
#include <iostream>
#include <algorithm>
#include <cmath>

constexpr float EPSILON = float(1e-3);
BaseCar::BaseCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed) : m_ForwardTwoBlade(forwardTwoBlade)
{
	m_ForwardTwoBlade[2] = speed;

	m_Width  = 15.f;
	m_Height = 30.f;

	m_CarPointsWorldSpace.push_back({ startPos[0], startPos[1], 0.f });
	m_CarPointsWorldSpace.push_back({ startPos[0], startPos[1] + m_Height, 0.f });
	m_CarPointsWorldSpace.push_back({ startPos[0] + m_Width, startPos[1] + m_Height, 0.f });
	m_CarPointsWorldSpace.push_back({ startPos[0] + m_Width, startPos[1], 0.f });
}

void BaseCar::UpdateForwardForce(float elapsedSec)
{
	TwoBlade desiredForwardTwoBlade = m_ForwardTwoBlade;
	desiredForwardTwoBlade[2] = 0;

	Motor forwardTranslator = Motor::Translation(m_ForwardTwoBlade[2] * elapsedSec, desiredForwardTwoBlade);

	for (ThreeBlade& worldPos : m_CarPointsWorldSpace)
	{
		worldPos = (forwardTranslator * worldPos * ~forwardTranslator).Grade3();
	}
}

void BaseCar::Draw() const
{
	//Draw car
	utils::SetColor(m_Color);
	utils::FillPolygon(ConvertThreeBladeArrayToPoint2fArray(m_CarPointsLocalSpace));

	//Draw lights
	utils::SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
	utils::DrawLine(Point2f(m_CarPointsLocalSpace[1][0],
							m_CarPointsLocalSpace[1][1]),
					Point2f(m_CarPointsLocalSpace[1][0] + m_ForwardTwoBlade[0] * 10.f,
						    m_CarPointsLocalSpace[1][1] + m_ForwardTwoBlade[1] * 10.f), 3.f);

	utils::DrawLine(Point2f(m_CarPointsLocalSpace[2][0],
							m_CarPointsLocalSpace[2][1]),
					Point2f(m_CarPointsLocalSpace[2][0] + m_ForwardTwoBlade[0] * 10.f,
							m_CarPointsLocalSpace[2][1] + m_ForwardTwoBlade[1] * 10.f), 3.f);

}

void BaseCar::UpdateCarPointsLocalSpace(const Camera* cameraPtr)
{
	m_CarPointsLocalSpace.clear();
	for (const ThreeBlade& carPointWorldSpace : m_CarPointsWorldSpace)
	{
		m_CarPointsLocalSpace.push_back(cameraPtr->GetAppliedTransform(carPointWorldSpace));
	}
}

bool BaseCar::IsPointInRange(float px, float py, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4) const
{
	// Check if the point is within the range of the first TwoBlade
	bool inRangeVec1 =
		((x1 - EPSILON <= px && px <= x2 + EPSILON) ||
			(x2 - EPSILON <= px && px <= x1 + EPSILON)) &&
		((y1 - EPSILON <= py && py <= y2 + EPSILON) ||
			(y2 - EPSILON <= py && py <= y1 + EPSILON));

	// Check if the point is within the range of the second vector
	bool inRangeVec2 =
		((x3 - EPSILON <= px && px <= x4 + EPSILON) ||
			(x4 - EPSILON <= px && px <= x3 + EPSILON)) &&
		((y3 - EPSILON <= py && py <= y4 + EPSILON) ||
			(y4 - EPSILON <= py && py <= y3 + EPSILON));

	// The point must satisfy both ranges
	return inRangeVec1 && inRangeVec2;
}

std::vector<Point2f> BaseCar::ConvertThreeBladeArrayToPoint2fArray(const std::vector<ThreeBlade>& pointsThreeBlade) const
{
	return [&pointsThreeBlade]
		{
			std::vector<Point2f> pointsPoint2f;
			for (const auto& point : pointsThreeBlade)
			{
				pointsPoint2f.push_back(Point2f{ point[0], point[1]});
			}
			return pointsPoint2f;
		}();
}

void BaseCar::CheckForTheCompletion(const Border& lastBorder, float completionTime)
{
	for (int i = 0; i < m_CarPointsWorldSpace.size(); ++i)
	{
		ThreeBlade point1 = m_CarPointsWorldSpace[i % m_CarPointsWorldSpace.size()];
		ThreeBlade point2 = m_CarPointsWorldSpace[(i + 1) % m_CarPointsWorldSpace.size()];

		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);

		TwoBlade curTwoBladeNoVanishing = !curTwoBlade;
		TwoBlade borderTwoBladeNoVanishing = !lastBorder.borderDirection;

		ThreeBlade skewTest = (curTwoBladeNoVanishing ^ borderTwoBladeNoVanishing).Grade3();

		ThreeBlade point;

		if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
		{
			TwoBlade commonNormal = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
			ThreeBlade NPoint = commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f);
			OneBlade commonPlane = NPoint & curTwoBlade;

			point = (commonPlane ^ lastBorder.borderDirection).Normalize();
		}

		if (IsPointInRange(point[0], point[1],
			point1[0], point1[1],
			point2[0], point2[1],
			lastBorder.startPosition[0], lastBorder.startPosition[1],
			lastBorder.endPosition[0], lastBorder.endPosition[1])
			)
		{
			m_IsRaceCompleted = true;
			std::cout << "Race completed with time: " << std::to_string(completionTime) << " s\n";
			break;
		}
	}
}

bool BaseCar::GetIsRaceCompleted() const
{
	return m_IsRaceCompleted;
}

ThreeBlade BaseCar::GetCarLocationLocalSpace() const
{
	return { (m_CarPointsLocalSpace[0][0] + m_CarPointsLocalSpace[2][0]) / 2.f,
			 (m_CarPointsLocalSpace[0][1] + m_CarPointsLocalSpace[2][1]) / 2.f, 
			  0.f };
}

ThreeBlade BaseCar::GetCarLocationWorldSpace() const
{
	return { (m_CarPointsWorldSpace[0][0] + m_CarPointsWorldSpace[2][0]) / 2.f,
			 (m_CarPointsWorldSpace[0][1] + m_CarPointsWorldSpace[2][1]) / 2.f, 
			  0.f };
}
