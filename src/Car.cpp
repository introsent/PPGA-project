#include "Car.h"
#include "utils.h"

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

void Car::Orbit(float elapsedSec, ThreeBlade orbitPoint)
{
	TwoBlade originToOrbitPoint = TwoBlade(orbitPoint[0], orbitPoint[1], orbitPoint[2], 0.f, 0.f, 0.f);
	float distance = originToOrbitPoint.VNorm();

	TwoBlade carToOrbitPoint = TwoBlade(m_CarPoints[0].x - orbitPoint[0], m_CarPoints[0].y - orbitPoint[0], 0.f, 0.f, 0.f, 0.f);
	carToOrbitPoint /= carToOrbitPoint.VNorm();

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
	

	//TwoBlade rotationLine = TwoBlade(0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
	Motor rotation = Motor::Rotation(2.f, m_LineToOrbitAround);

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

