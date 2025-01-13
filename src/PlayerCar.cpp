#include "PlayerCar.h"
#include "utils.h"
#include <numbers>
#include <iostream>
#include <algorithm>
#include <cmath>

PlayerCar::PlayerCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed) : BaseCar(startPos, forwardTwoBlade, speed)
{
	m_Color = Color4f(0.f, Remap(speed, 0.f, 300.f, 0.2f, 1.f), 0.f, 1.f);
}

void PlayerCar::UpdateSideForce(float elapsedSec)
{
	if (m_TimeBouncing > 0.f)
	{
		TwoBlade desireSideTwoBlade = m_SideForceTwoBlade;
		desireSideTwoBlade[2] = 0;
		Motor sideTranslator = Motor::Translation(m_ForwardTwoBlade[2] * elapsedSec, desireSideTwoBlade);

		for (ThreeBlade& worldPos : m_CarPointsWorldSpace)
		{
			worldPos = (sideTranslator * worldPos * ~sideTranslator).Grade3();
		}
		m_TimeBouncing -= elapsedSec;
	}
}

void PlayerCar::UpdateForwardForce(float elapsedSec)
{
	if (m_TimeBouncing <= 0.f)
	{
		BaseCar::UpdateForwardForce(elapsedSec);
	}
}

void PlayerCar::Draw() const
{
	BaseCar::Draw();
}

void PlayerCar::IncreaseSpeed()
{
	if (m_ForwardTwoBlade[2] + 50 < 300.f) m_ForwardTwoBlade[2] += 50.f;
	
	m_Color = Color4f(0.f, Remap(m_ForwardTwoBlade[2], 0.f, 300.f, 0.2f, 1.f), 0.f, 1.f);
}

void PlayerCar::DecreaseSpeed()
{
	m_ForwardTwoBlade[2] -= 50.f;
	m_Color = Color4f(0.f, Remap(m_ForwardTwoBlade[2], 0.f, 300.f, 0.2f, 1.f), 0.f, 1.f);
}

float PlayerCar::DetermineAngularVelocity(TwoBlade& radiusTwoBlade)
{
	float radius = radiusTwoBlade.VNorm();
	return m_ForwardTwoBlade[2] / radius;
}

void PlayerCar::Orbit(ThreeBlade orbitPoint, const std::vector<Border>& bordersArray)
{
	if (m_TimeBouncing <= 0.f)
	{
		TwoBlade originToOrbitPoint = TwoBlade(orbitPoint[0], orbitPoint[1], orbitPoint[2], 0.f, 0.f, 0.f);
		float distance = originToOrbitPoint.VNorm();
		 
		TwoBlade carToOrbitPoint = TwoBlade((m_CarPointsWorldSpace[0][0] + m_CarPointsWorldSpace[2][0]) / 2.f - orbitPoint[0], (m_CarPointsWorldSpace[0][1] + m_CarPointsWorldSpace[2][1]) / 2.f - orbitPoint[1], 0.f, 0.f, 0.f, 0.f);
		float angularVelocity = DetermineAngularVelocity(carToOrbitPoint);
		carToOrbitPoint /= carToOrbitPoint.VNorm();

		TwoBlade planeNormal = TwoBlade(0, 0, 0, 0, 0, 1);
		TwoBlade carSideTwoBlade = (planeNormal * !m_ForwardTwoBlade).Grade2(); //Cross between norm of plane and line to find a line perpendicular to m_ForwardTwoBlade

		float dotProduct = carSideTwoBlade | !carToOrbitPoint;

		if (!m_StartedRotating)
		{
			TwoBlade rotationLine;
			if (dotProduct > 0)
			{
				rotationLine = -planeNormal;
			}
			else if (dotProduct <= 0)
			{
				rotationLine = planeNormal;
			}

			m_LineToOrbitAround = rotationLine;
			m_StartedRotating = true;
		}

		Motor rotation = Motor::Rotation(angularVelocity, m_LineToOrbitAround);

		Motor translator = Motor::Translation(distance, originToOrbitPoint);

		Motor rotor = (translator * rotation * ~translator);

		for (ThreeBlade& worldPos : m_CarPointsWorldSpace)
		{
			worldPos = (rotor * worldPos * ~rotor).Grade3();
		}
	}
}
	

void PlayerCar::RotateLookAt()
{
	float currentSpeed = m_ForwardTwoBlade[2];

	m_ForwardTwoBlade = TwoBlade(m_CarPointsWorldSpace[1][0] - m_CarPointsWorldSpace[0][0], m_CarPointsWorldSpace[1][1] - m_CarPointsWorldSpace[0][1], 0.f, 0.f, 0.f, 0.f);
	m_ForwardTwoBlade /= m_ForwardTwoBlade.VNorm();

	m_ForwardTwoBlade[2] = currentSpeed;
}

void PlayerCar::SetStartedRotating(bool isRotating)
{
	m_StartedRotating = isRotating;
}
	
void PlayerCar::Snap()
{
	TwoBlade up =	 TwoBlade(0.f, 1.f, 0.f, 0.f, 0.f, 0.f);
	TwoBlade right = TwoBlade(1.f, 0.f, 0.f, 0.f, 0.f, 0.f);
	TwoBlade down =  TwoBlade(0.f, -1.f, 0.f, 0.f, 0.f, 0.f);
	TwoBlade left = TwoBlade(-1.f, 0.f, 0.f, 0.f, 0.f, 0.f);

	std::vector<TwoBlade> directions = { up, right, down, left };

	float maxDot = -std::numeric_limits<float>::infinity();
	TwoBlade closestDirection = directions[0];

	for (const auto& direction : directions) {
		float dotProduct = (!direction | !m_ForwardTwoBlade);
		if (dotProduct > maxDot) {
			maxDot = dotProduct;
			closestDirection = direction;
		}
	}

	TwoBlade planeNormal = TwoBlade(0, 0, 0, 0, 0, 1);
	TwoBlade carSideTwoBlade = (!m_ForwardTwoBlade * planeNormal).Grade2(); //Cross between norm of plane and line to find a line perpendicular to m_ForwardTwoBlade

	float dotProduct = carSideTwoBlade | !closestDirection;

	TwoBlade rotationLine;
	if (dotProduct > 0)
	{
		rotationLine = planeNormal;
	}
	else if (dotProduct <= 0)
	{
		rotationLine = -planeNormal;
	}

	float rotationAngle = acosf(maxDot) * 180.f / float(std::numbers::pi);
	Motor rotation = Motor::Rotation(rotationAngle, rotationLine);

	TwoBlade originToRotationPoint = TwoBlade((m_CarPointsWorldSpace[0][0] + m_CarPointsWorldSpace[2][0]) / 2.f, (m_CarPointsWorldSpace[0][1] + m_CarPointsWorldSpace[2][1]) / 2.f, 0.f, 0.f, 0.f, 0.f);
	float distance = originToRotationPoint.VNorm();
	
	Motor translator = Motor::Translation(distance, originToRotationPoint);

	Motor rotor = (translator * rotation * ~translator);

	for (ThreeBlade& worldPos : m_CarPointsWorldSpace)
	{
		worldPos = (rotor * worldPos * ~rotor).Grade3();
	}

	float currentSpeed = m_ForwardTwoBlade[2];
	m_ForwardTwoBlade = -closestDirection;
	m_ForwardTwoBlade[2] = currentSpeed;
}



void PlayerCar::CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray)
{
	for (const auto& border : bordersArray)
	{
		for (int i = 0; i < m_CarPointsWorldSpace.size(); ++i)
		{ 
			ThreeBlade point1 = m_CarPointsWorldSpace[i % m_CarPointsWorldSpace.size()];
			ThreeBlade point2 = m_CarPointsWorldSpace[(i + 1) % m_CarPointsWorldSpace.size()];

			TwoBlade curTwoBlade = TwoBlade::LineFromPoints(point1[0], point1[1], point1[2], point2[0], point2[1], point2[2]);

			TwoBlade curTwoBladeNoVanishing  = !curTwoBlade;
			TwoBlade borderTwoBladeNoVanishing = !border.borderDirection;

			ThreeBlade skewTest = (curTwoBladeNoVanishing ^ borderTwoBladeNoVanishing).Grade3();

			ThreeBlade point;

			if (skewTest == ThreeBlade(0.f, 0.f, 0.f, 0.f))
			{
				TwoBlade commonNormal = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
				ThreeBlade NPoint = commonNormal ^ OneBlade::OneBlade(1.f, 0.f, 0.f, 0.f);
				OneBlade commonPlane = NPoint & curTwoBlade;

				point = (commonPlane ^ border.borderDirection).Normalize();
			}

			if (IsPointInRange(point[0], point[1],
				point1[0], point1[1],
				point2[0], point2[1],
				border.startPosition[0], border.startPosition[1],
				border.endPosition[0], border.endPosition[1])
				)
			{
				Bounce(point, border.borderDirection);
				break;
			}
		}
	}
}

void PlayerCar::Bounce(const ThreeBlade& hitPos, const TwoBlade& borderVector)
{
	if (m_ForwardTwoBlade[2] > 50)
	{
		DecreaseSpeed();
	}
	RotateLookAt();

	float minDistance = std::numeric_limits<float>::infinity();
	int indexOfClosestPoint = 0;

	for (int curIndex{ 0 }; curIndex < m_CarPointsWorldSpace.size(); ++curIndex)
	{
		//ThreeBlade position = ThreeBlade(m_CarPointsWorldSpace.[curIndex].x, m_CarPoints[curIndex].y, 0.f);
		ThreeBlade position = m_CarPointsWorldSpace[curIndex];
		float distance = (position.Normalized() & hitPos.Normalized()).Norm();
		if (distance < minDistance)
		{
			minDistance = distance;
			indexOfClosestPoint = curIndex;
		}
	}
	minDistance *= 1.5f;
	
	m_SideForceTwoBlade = m_ForwardTwoBlade;

	if (m_ForwardTwoBlade[2] >= 0.f && (indexOfClosestPoint == 0 || indexOfClosestPoint == 3))
	{
		m_SideForceTwoBlade = m_ForwardTwoBlade;
	}
	else
	{
		auto reflector = borderVector.Normalized() * m_SideForceTwoBlade * ~(borderVector.Normalized());
		m_SideForceTwoBlade = reflector.Grade2();
	}

	Motor translator = Motor::Translation(minDistance, m_SideForceTwoBlade);
	for (ThreeBlade& worldPos : m_CarPointsWorldSpace)
	{
		worldPos = (translator * worldPos * ~translator).Grade3();
	}

	m_TimeBouncing = 0.5f;
}

