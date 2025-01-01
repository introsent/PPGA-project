#include "Car.h"
#include "utils.h"
#include <numbers>
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
		rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, -1.f);
	}
	else if (perpDot < 0)
	{
		rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	}

	float rotationAngle = acosf(maxDot) * 180.f / std::numbers::pi;
	Motor rotation = Motor::Rotation(rotationAngle, rotationLine);

	TwoBlade originToRotationPoint = TwoBlade(m_CarPoints[0].x, m_CarPoints[0].y, 0.f, 0.f, 0.f, 0.f);
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

