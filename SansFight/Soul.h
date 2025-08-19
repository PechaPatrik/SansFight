#pragma once
#include "utils.h"
#include "SoundEffect.h"
#include "Texture.h"
#include "Platform.h"

class Soul
{
public:
	Soul();
	~Soul();

	Soul(const Soul& other) = delete;
	Soul& operator=(const Soul& other) = delete;
	Soul(Soul&& other) = delete;
	Soul& operator=(Soul&& other) = delete;

	void Draw() const;
	void Move(float elapsedSec, const Rectf& bounds, const std::vector<Platform*>& platforms = {});
	void OnKeyDown(const SDL_KeyboardEvent& e);
	void OnKeyUp(const SDL_KeyboardEvent& e);
	void SetMode(bool newMode);
	void SetGravityDirection(const Vector2f& newGravityDirection);
	void SetBoundSnapping(bool newSnapping);
	void Teleport(const Vector2f& newMiddlePos);
	void Slam(const Vector2f& direction, float speed = 100.f);
	void DisableInput(bool newSetting);
	void SetOnPlatform(bool newOnPlatform) { m_isOnPlatform = newOnPlatform; }
	void SetFinalAttackX(float newFinalX) { m_finalAttackX = newFinalX; }

	Rectf GetHitbox() const;
	Vector2f GetVelocity() const { return m_velocity; }
private:
	Vector2f m_pos{};
	Vector2f m_gravityDirection{};
	Vector2f m_gravityVelocity{};
	Vector2f m_velocity{};
	Platform* m_pCurrentPlatform{};
	Texture* m_pSoulTexture{};
	SoundEffect m_slamSound{ "Audio/Slam.ogg" };
	SoundEffect m_dingSound{ "Audio/Ding.ogg" };
	const float m_size{};
	const float m_speed{};
	const float m_gravity{};
	bool m_mode{};
	bool m_isFalling{};
	bool m_isGrounded{};
	bool m_disableInput{};
	bool m_jumpKeyPressedWhileFalling{};
	bool m_boundSnapping{ 1 };
	bool m_isOnPlatform{ 0 };
	float m_finalAttackX{ 0 };

	void OnKeyUpHandling(const Vector2f& dir);
	void BoundsSnapping(const Rectf& bounds);
};

