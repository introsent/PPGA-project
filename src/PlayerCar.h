#pragma once
#include "FlyFish.h"
#include "utils.h"
#include "Camera.h"
#include "Border.h"
#include "BaseCar.h"
class PlayerCar final : public BaseCar
{
public:
	PlayerCar(ThreeBlade startPos, TwoBlade forwardTwoBlade, float speed);

	PlayerCar(const PlayerCar&) = delete;
	PlayerCar& operator=(const PlayerCar&) = delete;
	PlayerCar(PlayerCar&&) = delete;
	PlayerCar& operator=(PlayerCar&&) = delete;

	void UpdateSideForce(float elapsedSec);
	void UpdateForwardForce(float elapsedSec) override;

	void CheckIntersectionWithMapBorders(const std::vector<Border>& bordersArray) override;
	void Draw() const override;

	void IncreaseSpeed();
	void DecreaseSpeed();

	float DetermineAngularVelocity(TwoBlade& radiusTwoBlade);

	void Orbit(ThreeBlade orbitPoint, const std::vector<Border>& bordersArray);

	void RotateLookAt() override;

	void SetStartedRotating(bool isRotating);

	void Snap();

	void Bounce(const ThreeBlade& hitPos, const TwoBlade& borderVector);
private:
	TwoBlade m_SideForceTwoBlade;
	TwoBlade m_LineToOrbitAround;

	float m_TimeBouncing = 0.f;

	bool m_StartedRotating = false;
};

