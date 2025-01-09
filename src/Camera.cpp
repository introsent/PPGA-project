#include "Camera.h"
#include <stack>
Camera::Camera(float screenWidth, float screenHeight)
{
	m_ScreenWidth = screenWidth;
	m_ScreenHeight = screenHeight;

	m_FinalTransform = Motor(0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f);
}

//void Camera::PushMatrix()
//{
//	m_TransformationStack.push(m_FinalTransform);
//}
//
//void Camera::PopMatrix()
//{
//	if (!m_TransformationStack.empty()) {
//		m_FinalTransform = m_TransformationStack.top();
//		m_TransformationStack.pop();
//	}
//}

//void Camera::ApplyTransform(const Motor& transformation)
//{
//	m_FinalTransform = transformation * m_FinalTransform;
//}

Point2f Camera::GetAppliedTransform(const Point2f& pointToTransform) const
{
	ThreeBlade newPoint = (m_FinalTransform * ThreeBlade(pointToTransform.x, pointToTransform.y, 0.f) * ~m_FinalTransform).Grade3();

	float x, y;

	if (!std::isnan(newPoint[0]))
	{
		x = newPoint[0];
	}
	else
	{
		x = pointToTransform.x;
	}

	if (!std::isnan(newPoint[1]))
	{
		y = newPoint[1];
	}
	else
	{
		y = pointToTransform.y;
	}

	return Point2f(x, y);
	//return pointToTransform;
}

Point2f Camera::GetWorldLocation(const Point2f& pointInLocalSpace) const
{
	//ThreeBlade newPoint = (-m_FinalTransform * ThreeBlade(pointInLocalSpace.x, pointInLocalSpace.y, 0.f) * ~(-m_FinalTransform)).Grade3();
	float transformX, transformY;

	if (!std::isnan(m_FinalTransform[1])) //m_FinalTransform[1] != m_FinalTransform[1]))
	{
		transformX = m_FinalTransform[1];
	}
	else
	{
		transformX = 0.f;
	}

	if (!std::isnan(m_FinalTransform[2]))
	{
		transformY = m_FinalTransform[2];
	}
	else
	{
		transformY = 0.f;
	}

	Motor translation = Motor(m_FinalTransform[0], -transformX, -transformY, m_FinalTransform[3], m_FinalTransform[4], m_FinalTransform[5], m_FinalTransform[6], m_FinalTransform[7]);

	ThreeBlade result = (translation * ThreeBlade(pointInLocalSpace.x, pointInLocalSpace.y, 0.f) * ~translation).Grade3();


	float x, y;


	if (!std::isnan(result[0])) //m_FinalTransform[1] != m_FinalTransform[1]))
	{
		x = result[0];
	}
	else
	{
		x = pointInLocalSpace.x;
	}

	if (!std::isnan(result[1]))
	{
		y = result[1];
	}
	else
	{
		y = pointInLocalSpace.y;
	}
	return Point2f(x, y);
}

//Zconst Motor& Camera::GetCurrentTransformation() const
//Z{
//Z	return m_FinalTransform;
//Z}

void Camera::Aim(float levelW, float levelH, ThreeBlade trackCenter)
{
	float cameraY{ trackCenter[1] - m_ScreenHeight / 2.f};
	float cameraX{ trackCenter[0] - m_ScreenWidth / 2.f};

	if (cameraY < 0) cameraY = 0;
	else if (cameraY > levelH - m_ScreenHeight) cameraY = levelH - m_ScreenHeight;

	if (cameraX < 0) cameraX = 0;
	else if (cameraX > levelW - m_ScreenWidth) cameraX = levelW - m_ScreenWidth;

	//glPushMatrix();
	//glTranslatef(0.f, -cameraY, 0.0f);
	//

	TwoBlade cameraTranslationVector = TwoBlade(-cameraX, -cameraY, 0.f, 0.f, 0.f, 0.f);
	m_FinalTransform = Motor::Translation(cameraTranslationVector.VNorm(), cameraTranslationVector);


}

void Camera::Reset()
{
	m_FinalTransform = Motor();
}
