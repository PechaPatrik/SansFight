#pragma once
#include "utils.h"

class Weapon
{
public:
    Weapon(const Vector2f& pos);
    virtual ~Weapon() = default;

    virtual void Draw() const = 0;
    virtual Rectf GetHitbox() const = 0;
    virtual void Update(float elapsedSec) = 0;

    bool ShouldDestroy() const { return m_ShouldDestroy; }
    Vector2f GetPosition() const { return m_Pos; }

protected:
    Vector2f m_Pos;
    bool m_ShouldDestroy{ false };
};

