#include "Car.h"
#include "utils.h"
#include <numbers>
#include <iostream>
#include <algorithm>
Car::Car(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed) : m_Position(startPos), m_ForwardTwoBlade(forwardTwoBlade), m_Speed(speed)
{
	m_Width  = 15.f;
	m_Height = 30.f;
	m_Color = Color4f(0.f, Remap(m_Speed, 0.f, 300.f, 0.2f, 1.f), 0.f, 1.f);

	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1]));
	m_CarPoints.push_back(Point2f(m_Position[0], m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1] + m_Height));
	m_CarPoints.push_back(Point2f(m_Position[0] + m_Width, m_Position[1]));
}

void Car::Update(float elapsedSec)
{
	UpdateSideForce(elapsedSec);
	UpdateForwardForce(elapsedSec);
}

void Car::UpdateSideForce(float elapsedSec)
{
	if (m_TimeBouncing > 0.f)
	{
		Motor sideTranslator = Motor::Translation(m_Speed * elapsedSec, m_SideForceTwoBlade);

		for (Point2f& pos : m_CarPoints)
		{
			ThreeBlade position = (sideTranslator * ThreeBlade(pos.x, pos.y, 0.f) * ~sideTranslator).Grade3();
			pos = Point2f(position[0], position[1]);
		}

		m_TimeBouncing -= elapsedSec;
	}
}

void Car::UpdateForwardForce(float elapsedSec)
{
	if (m_TimeBouncing <= 0.f)
	{
		Motor forwardTranslator = Motor::Translation(m_Speed * elapsedSec, m_ForwardTwoBlade);

		for (Point2f& pos : m_CarPoints)
		{
			ThreeBlade position = (forwardTranslator * ThreeBlade(pos.x, pos.y, 0.f) * ~forwardTranslator).Grade3();
			pos = Point2f(position[0], position[1]);
		}
	}
}

void Car::UpdateCarPointsLocalSpace(const Camera* cameraPtr)
{
	m_CarPointsLocalSpace.clear();
	for (const Point2f& carPoint : m_CarPoints)
	{
		m_CarPointsLocalSpace.push_back(cameraPtr->GetAppliedTransform(carPoint));
	}

}

void Car::Draw()
{
	utils::SetColor(m_Color);

	utils::FillPolygon(m_CarPointsLocalSpace);

	utils::SetColor(Color4f(1.f, 1.f, 0.f, 1.f));
	utils::DrawLine(Point2f(m_CarPointsLocalSpace[1].x,
							m_CarPointsLocalSpace[1].y),
					Point2f(m_CarPointsLocalSpace[1].x + m_ForwardTwoBlade[0] * 10.f,
						    m_CarPointsLocalSpace[1].y + m_ForwardTwoBlade[1] * 10.f), 3.f);

	utils::DrawLine(Point2f(m_CarPointsLocalSpace[2].x,
							m_CarPointsLocalSpace[2].y),
					Point2f(m_CarPointsLocalSpace[2].x + m_ForwardTwoBlade[0] * 10.f,
							m_CarPointsLocalSpace[2].y + m_ForwardTwoBlade[1] * 10.f), 3.f);

}

void Car::IncreaseSpeed()
{
	if (m_Speed + 50 < 300.f)
	{
		m_Speed += 50.f;
	}
	m_Color = Color4f(0.f, Remap(m_Speed, 0.f, 300.f, 0.2f, 1.f), 0.f, 1.f);
}

void Car::DecreaseSpeed()
{
	m_Speed -= 50.f;
	m_Color = Color4f(0.f, Remap(m_Speed, 0.f, 300.f, 0.2f, 1.f), 0.f, 1.f);
}

float Car::DetermineAngularVelocity(TwoBlade& radiusTwoBlade)
{
	float radius = radiusTwoBlade.VNorm();
	return m_Speed / radius;
}

void Car::Orbit(ThreeBlade orbitPoint, const std::vector<Border>& bordersArray)
{
	if (m_TimeBouncing <= 0.f)
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
	
	
}

void Car::RotateLookAt()
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



void Car::CheckIntersectionWithMapBorders(const TwoBlade& border, const ThreeBlade& startPos, const ThreeBlade& endPos)
{
	for (int i = 0; i < m_CarPoints.size(); ++i)
	{
		ThreeBlade point1 = ThreeBlade(m_CarPoints[i % m_CarPoints.size()].x, m_CarPoints[i % m_CarPoints.size()].y, 0.f);
		ThreeBlade point2 = ThreeBlade(m_CarPoints[(i + 1) % m_CarPoints.size()].x, m_CarPoints[(i + 1) % m_CarPoints.size()].y, 0.f);

		TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);

		TwoBlade curTwoBladeNoVanishing = ThreeBlade(curTwoBlade[3], curTwoBlade[4], curTwoBlade[5])  & ThreeBlade(0.f, 0.f, 0.f);
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
			(std::min(point1[0], point2[0]) <= point[0] && point[0] <= std::max(point1[0], point2[0]) &&
			std::min(point1[1], point2[1]) <= point[1] && point[1] <= std::max(point1[1], point2[1]))
			&&
			(std::min(startPos[0], endPos[0]) <= point[0] && point[0] <= std::max(startPos[0], endPos[0]) &&
			std::min(startPos[1], endPos[1]) <= point[1] && point[1] <= std::max(startPos[1], endPos[1]))

			)
		{
			Bounce(point, border);
			break;
		}
	}
}

void Car::Bounce(const ThreeBlade& hitPos, const TwoBlade& borderVector)
{
	if (m_Speed > 50)
	{
		DecreaseSpeed();
	}
	RotateLookAt();

	float minDistance = std::numeric_limits<float>::infinity();
	int indexOfClosestPoint = 0;

	for (int curIndex{ 0 }; curIndex < m_CarPoints.size(); ++curIndex)
	{
		ThreeBlade position = ThreeBlade(m_CarPoints[curIndex].x, m_CarPoints[curIndex].y, 0.f);
		float distance = (position.Normalized() & hitPos.Normalized()).Norm();
		if (distance < minDistance)
		{
			minDistance = distance;
			indexOfClosestPoint = curIndex;
		}
	}
	minDistance *= 1.5f;
	
	m_SideForceTwoBlade = m_ForwardTwoBlade;

	if (m_Speed > 0.f && (indexOfClosestPoint == 0 || indexOfClosestPoint == 3))
	{
		m_SideForceTwoBlade = m_ForwardTwoBlade;
	}
	else
	{
		auto reflector = borderVector.Normalized() * m_SideForceTwoBlade * ~(borderVector.Normalized());
		m_SideForceTwoBlade = reflector.Grade2();
	}

	Motor translator = Motor::Translation(minDistance, m_SideForceTwoBlade);
	for (Point2f& pos : m_CarPoints)
	{
		ThreeBlade position = (translator * ThreeBlade(pos.x, pos.y, 0.f) * ~translator).Grade3();
		pos = Point2f(position[0], position[1]);
	}

	m_TimeBouncing = 0.5f;
}

ThreeBlade Car::GetCarWorldLocation() const
{
	return ThreeBlade(m_CarPoints[0].x, m_CarPoints[0].y, 0.f);
}

