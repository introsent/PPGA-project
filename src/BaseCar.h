#pragma once
#include "FlyFish.h"
#include "utils.h"
#include "Camera.h"
#include "Border.h"
class BaseCar
{
public:
	BaseCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed);

	BaseCar(const BaseCar&) = delete;
	BaseCar& operator=(const BaseCar&) = delete;
	BaseCar(BaseCar&&) = delete;
	BaseCar& operator=(BaseCar&&) = delete;

	virtual void UpdateForwardForce(float elapsedSec);

	virtual void RotateLookAt() = 0;

	virtual void CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray) = 0;

	virtual void Draw() const;

	void UpdateCarPointsLocalSpace(const Camera* cameraPtr);

	ThreeBlade GetCarLocationLocalSpace() const;
	ThreeBlade GetCarLocationWorldSpace() const;

	float Remap(float value, float fromLow, float fromHigh, float toLow, float toHigh) const {
		return toLow + (value - fromLow) * (toHigh - toLow) / (fromHigh - fromLow);
	}

	bool IsPointInRange(float px, float py,
		float x1, float y1, float x2, float y2,
		float x3, float y3, float x4, float y4) const;

protected:
	TwoBlade m_ForwardTwoBlade;

	float m_Width;
	float m_Height;
	Color4f m_Color;

	std::vector<ThreeBlade> m_CarPointsWorldSpace;
	std::vector<ThreeBlade> m_CarPointsLocalSpace;

private:
	std::vector<Point2f> ConvertThreeBladeArrayToPoint2fArray(const std::vector<ThreeBlade> pointsThreeBlade) const;
};

