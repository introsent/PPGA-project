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

void RivalCar::Update(float elapsedSec)
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
	utils::SetColor(Color4f(1.f, 0.f, 0.f, 1.f));

	Point2f startLinePos = Point2f(m_CarPoints[0].x + m_CarPoints[2].x / 2.f, m_CarPoints[0].y + m_CarPoints[2].y);
	for (const auto& direction : m_PossibleDirections)
	{
		Point2f endLinePos = Point2f(startLinePos.x + direction[0], startLinePos.y + direction[1]);
		utils::DrawLine(startLinePos, endLinePos, 3.f);
	}
}

void RivalCar::RotateLookAt()
{
}

void RivalCar::CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos)
{

}

void RivalCar::CalculatePossibleDirections()
{
	float directionTwoBladeSize = 30.f;

	int amountOfPossibleDirection = 8;
	float segmentLength = 360.f / amountOfPossibleDirection; //45

	for (int angle{ 0 }; angle < 360.f; ++segmentLength)
	{
		m_PossibleDirections.push_back(TwoBlade(directionTwoBladeSize * cos(angle), directionTwoBladeSize * sin(angle), 0.f, 0.f, 0.f, 0.f));
	}
}
