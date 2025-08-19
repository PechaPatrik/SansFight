#pragma once
#include "Weapon.h"

class Platform : public Weapon
{
public:
    Platform(const Vector2f& pos, const Rectf& viewport, float size, int direction = 0, float speed = 0);
    void Draw() const override;
    Rectf GetHitbox() const override;
    float GetSpeed() const { return m_direction * m_speed; }
    int GetDirection() const { return m_direction; }
    void SetDirection(int newDirection) { m_direction = newDirection; }
    void Update(float elapsedSec) override;

private:
    int m_direction;
    float m_speed;
    Rectf m_viewport;
    Rectf m_Hitbox;
};