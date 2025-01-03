#include "Car.h"
#include "utils.h"
#include <numbers>
#include <iostream>
Car::Car(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed) : m_Position(startPos), m_ForwardTwoBlade(forwardTwoBlade), m_Speed(speed)
{
	m_Width  = 15.f;
	m_Height = 30.f;
	m_Color = Color4f(1.f, 1.f, 1.f, 1.f);

	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1]));
	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1]));
}

void Car::Update(float elapsedSec)
{
	Motor translator = Motor::Translation(m_Speed * elapsedSec, m_ForwardTwoBlade);

	for (Point2f& pos : m_CarPoints)
	{
		ThreeBlade position = (translator * ThreeBlade(pos.x, pos.y, 0.f) * ~translator).Grade3();
		pos = Point2f(position[0], position[1]);
	}
	//m_Position = (translator * m_Position * ~translator).Grade3();
}

void Car::Draw()
{
	utils::SetColor(m_Color);

	utils::FillPolygon(m_CarPoints);

	utils::SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
	utils::DrawLine(Point2f(m_CarPoints[1].x,
							m_CarPoints[1].y),
					Point2f(m_CarPoints[1].x + m_ForwardTwoBlade[0] * 10.f,
							m_CarPoints[1].y + m_ForwardTwoBlade[1] * 10.f), 3.f);

	utils::DrawLine(Point2f(m_CarPoints[2].x,
							m_CarPoints[2].y),
							Point2f(m_CarPoints[2].x + m_ForwardTwoBlade[0] * 10.f,
							m_CarPoints[2].y + m_ForwardTwoBlade[1] * 10.f), 3.f);

}

void Car::IncreaseSpeed()
{
	m_Speed += 50.f;
}

void Car::DecreaseSpeed()
{
	m_Speed -= 50.f;
}

float Car::DetermineAngularVelocity(TwoBlade& radiusTwoBlade)
{
	float radius = radiusTwoBlade.VNorm();
	return m_Speed / radius;
}

void Car::Orbit(float elapsedSec, ThreeBlade orbitPoint)
{
	TwoBlade originToOrbitPoint = TwoBlade(orbitPoint[0], orbitPoint[1], orbitPoint[2], 0.f, 0.f, 0.f);
	float distance = originToOrbitPoint.VNorm();

	TwoBlade carToOrbitPoint = TwoBlade(m_CarPoints[0].x - orbitPoint[0], m_CarPoints[0].y - orbitPoint[1], 0.f, 0.f, 0.f, 0.f);
	float angularVelocity = DetermineAngularVelocity(carToOrbitPoint);
	carToOrbitPoint /= carToOrbitPoint.VNorm();

	//Perpendicular dot
	auto perpDot = m_ForwardTwoBlade[0] * carToOrbitPoint[1] - m_ForwardTwoBlade[1] * carToOrbitPoint[0];


	if (!m_StartedRotating)
	{
		TwoBlade rotationLine;
		if (perpDot > 0)
		{
			rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, -1.f);
		}
		else if (perpDot < 0)
		{
			rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
		}

		m_LineToOrbitAround = rotationLine;
		m_StartedRotating = true;
	}
	

	
	Motor rotation = Motor::Rotation(angularVelocity, m_LineToOrbitAround);

	Motor translator = Motor::Translation(distance, originToOrbitPoint);

	Motor rotor = (translator * rotation * ~translator);

	for (Point2f& pos : m_CarPoints)
	{
		ThreeBlade position = (rotor * ThreeBlade(pos.x, pos.y, 0.f) * ~rotor).Grade3();
		pos = Point2f(position[0], position[1]);
	}
	
}

void Car::RotateLookAt(ThreeBlade orbitPoint)
{
	m_ForwardTwoBlade = TwoBlade(m_CarPoints[1].x - m_CarPoints[0].x, m_CarPoints[1].y - m_CarPoints[0].y, 0.f, 0.f, 0.f, 0.f);
	m_ForwardTwoBlade /= m_ForwardTwoBlade.VNorm();
}

void Car::SetStartedRotating(bool isRotating)
{
	m_StartedRotating = isRotating;
}

void Car::Snap()
{
	TwoBlade up =	 TwoBlade(0.f, 1.f, 0.f, 0.f, 0.f, 0.f);
	TwoBlade right = TwoBlade(1.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	TwoBlade down =  TwoBlade(0.f, -1.f, 0.f, 0.f, 0.f, 0.f);
	TwoBlade left = TwoBlade(-1.f, 0.f, 0.f, 0.f, 0.f, 0.f);

	std::vector<TwoBlade> directions = { up, right, down, left };

	float maxDot = -std::numeric_limits<float>::infinity();
	TwoBlade closestDirection = directions[0];

	for (const auto& direction : directions) {
		float dotProduct = m_ForwardTwoBlade[0] * direction[0] + m_ForwardTwoBlade[1] * direction[1];
		if (dotProduct > maxDot) {
			maxDot = dotProduct;
			closestDirection = direction;
		}
	}

	//Perpendicular dot
	auto perpDot = m_ForwardTwoBlade[0] * closestDirection[1] - m_ForwardTwoBlade[1] * closestDirection[0];

	TwoBlade rotationLine;
	if (perpDot > 0)
	{
		rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	}
	else if (perpDot < 0)
	{
		rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, -1.f);
	}

	float rotationAngle = acosf(maxDot) * 180.f / float(std::numbers::pi);
	Motor rotation = Motor::Rotation(rotationAngle, rotationLine);

	TwoBlade originToRotationPoint = TwoBlade((m_CarPoints[0].x + m_CarPoints[2].x) / 2.f, (m_CarPoints[0].y + m_CarPoints[2].y) / 2.f, 0.f, 0.f, 0.f, 0.f);
	float distance = originToRotationPoint.VNorm();
	
	Motor translator = Motor::Translation(distance, originToRotationPoint);

	Motor rotor = (translator * rotation * ~translator);

	for (Point2f& pos : m_CarPoints)
	{
		ThreeBlade position = (rotor * ThreeBlade(pos.x, pos.y, 0.f) * ~rotor).Grade3();
		pos = Point2f(position[0], position[1]);
	}

	m_ForwardTwoBlade = closestDirection;
}


std::vector<Point2f>& Car::GetCarPositions()
{
	return m_CarPoints;
}

void Car::CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos)
{
	for (int i = 0; i < m_CarPoints.size(); ++i)
	{
		ThreeBlade point1 = ThreeBlade(m_CarPoints[i % m_CarPoints.size()].x, m_CarPoints[i % m_CarPoints.size()].y, 0.f);
		ThreeBlade point2 = ThreeBlade(m_CarPoints[(i + 1) % m_CarPoints.size()].x, m_CarPoints[(i + 1) % m_CarPoints.size()].y, 0.f);

		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);
		//curTwoBlade[2] = 0;

		TwoBlade curTwoBladeVanishingToEuclidean = ThreeBlade(curTwoBlade[3], curTwoBlade[4], curTwoBlade[5])  & ThreeBlade(0.f, 0.f, 0.f);
		TwoBlade borderTwoBladeVanishingToEuclidean = ThreeBlade(border[3], border[4], border[5]) & ThreeBlade(0.f, 0.f, 0.f);

		ThreeBlade skewTest = (curTwoBladeVanishingToEuclidean ^ borderTwoBladeVanishingToEuclidean).Grade3();

		ThreeBlade point;

		if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
		{
			TwoBlade commonNormal = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);					//(curTwoBlade * border).Grade2();
			ThreeBlade NPoint = commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f);
			
			OneBlade commonPlane = NPoint & curTwoBlade;

			point = (commonPlane ^ border).Normalize();
		}

		std::cout << point.toString() << std::endl;

		if (
			(std::min(point1[0], point2[0]) <= point[0] && point[0] <= std::max(point1[0], point2[0]) &&
			std::min(point1[1], point2[1]) <= point[1] && point[1] <= std::max(point1[1], point2[1]))
			&&
			(std::min(startPos[0], endPos[0]) <= point[0] && point[0] <= std::max(startPos[0], endPos[0]) &&
			std::min(startPos[1], endPos[1]) <= point[1] && point[1] <= std::max(startPos[1], endPos[1]))

			)
		{
			Reflect();
		}
	}
}

void Car::Reflect()
{
	//reflection logic
}

