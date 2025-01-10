#include "RivalCar.h"
#include "utils.h"

RivalCar::RivalCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed) : m_Position(startPos), m_ForwardTwoBlade(forwardTwoBlade), m_Speed(speed)
{
	m_Width = 15.f;
	m_Height = 30.f;
	m_Color = Color4f(0.f, 0.f, 1.f, 1.f);

	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1]));
	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1]));

	CalculatePossibleDirections();
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
	m_CarPoints[1] = Point2f(m_CarPoints[0].x + m_Width * normalizedForwardTwoBlade[0], m_CarPoints[0].y + m_Height * normalizedForwardTwoBlade[1]);
	m_CarPoints[2] = Point2f(m_CarPoints[1].x + m_Width * normalizedForwardTwoBlade[1], m_CarPoints[1].y + m_Height * (-normalizedForwardTwoBlade[0]));
	m_CarPoints[3] = Point2f(m_CarPoints[0].x + m_Width * normalizedForwardTwoBlade[1], m_CarPoints[0].y + m_Height * (-normalizedForwardTwoBlade[0]));
}

void RivalCar::CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos)
{
	std::vector<int> indicesOfPossibleDirectionArray;

	ThreeBlade point1 = ThreeBlade((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f, 0.f);
	int currentInteration = 0;
	for (const auto& direction : m_PossibleDirections)
	{
		//ThreeBlade point1 = ThreeBlade(m_CarPoints[i % m_CarPoints.size()].x, m_CarPoints[i % m_CarPoints.size()].y, 0.f);
		ThreeBlade point2 = ThreeBlade(point1[0] + direction[0], point1[1] + direction[1], 0.f);

		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);

		TwoBlade curTwoBladeNoVanishing = ThreeBlade(curTwoBlade[3], curTwoBlade[4], curTwoBlade[5]) & ThreeBlade(0.f, 0.f, 0.f);
		TwoBlade borderTwoBladeNoVanishing = ThreeBlade(border[3], border[4], border[5]) & ThreeBlade(0.f, 0.f, 0.f);

		ThreeBlade skewTest = (curTwoBladeNoVanishing ^ borderTwoBladeNoVanishing).Grade3();

		ThreeBlade point;

		if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
		{
			TwoBlade commonNormal = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
			ThreeBlade NPoint = commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f);
			OneBlade commonPlane = NPoint & curTwoBlade;

			point = (commonPlane ^ border).Normalize();
		}

		if (
			(std::min(point1[0], point2[0]) > point[0] && point[0] > std::max(point1[0], point2[0]) &&
				std::min(point1[1], point2[1]) > point[1] && point[1] && std::max(point1[1], point2[1]))
			&&
			(std::min(startPos[0], endPos[0]) > point[0] && point[0] > std::max(startPos[0], endPos[0]) &&
				std::min(startPos[1], endPos[1]) > point[1] && point[1] > std::max(startPos[1], endPos[1]))
		
			)
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

	m_ForwardTwoBlade = m_PossibleDirections[desiredDirectionIndex] / m_PossibleDirections[desiredDirectionIndex].VNorm();

	RotateLookAt();
	
}

void RivalCar::CalculatePossibleDirections()
{
	float directionTwoBladeSize = 90.f;

	int amountOfPossibleDirection = 20;
	float segmentLength = 360.f / amountOfPossibleDirection; //45

	for (float angle{ 0.f }; angle <= 180.f; )
	{
		m_PossibleDirections.push_back(TwoBlade(directionTwoBladeSize * cosf(angle * utils::g_Pi / 180.f), directionTwoBladeSize * sinf(angle * utils::g_Pi /180.f), 0.f, 0.f, 0.f, 0.f));

		angle += segmentLength; 
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
