#pragma once
#include <stack>
#include "FlyFish.h"
#include "utils.h"
class Camera
{
public:
	explicit Camera(float screenWidth, float screenHeight);
	Camera& operator=(const Camera& rhs) = default;
	Camera(const Camera& other) = default;

	ThreeBlade GetAppliedTransform(const ThreeBlade& pointToTransform) const;
	ThreeBlade GetWorldLocation(const ThreeBlade& pointInLocalSpace) const;
	//const Motor& GetCurrentTransformation() const;


	void Aim(float levelW, float levelH, ThreeBlade trackCenter);
	void Reset();

private:
	//std::stack<Motor> m_TransformationStack;
	Motor m_FinalTransform;

	float m_ScreenWidth;
	float m_ScreenHeight;
};

