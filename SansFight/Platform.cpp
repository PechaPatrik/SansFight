#include "pch.h"
#include "Platform.h"

Platform::Platform(const Vector2f& pos, const Rectf& viewport, float size, int direction, float speed) :
    Weapon(pos),
    m_speed(speed),
    m_direction(direction),
    m_viewport(viewport),
    m_Hitbox(Rectf{ pos.x - size / 2.f, pos.y, size, 5.f })
{
}

void Platform::Draw() const
{
    utils::SetColor(Color4f{ 1, 1, 1, 1 });
    utils::DrawRect(m_Hitbox);
    utils::SetColor(Color4f{ 0, 0.5f, 0, 1 });
    utils::DrawRect(Rectf{ m_Hitbox.left, m_Hitbox.bottom + 3.f, m_Hitbox.width, m_Hitbox.height });
}

Rectf Platform::GetHitbox() const
{
    return m_Hitbox;
}

void Platform::Update(float elapsedSec)
{
    if (m_direction == 0) {
        return;
    }
    m_Hitbox.left += m_direction * m_speed * elapsedSec;

    if ((m_direction > 0 && m_Hitbox.left > m_viewport.left + m_viewport.width) ||
        (m_direction < 0 && m_Hitbox.left + m_Hitbox.width < m_viewport.left))
    {
        m_ShouldDestroy = true;
    }
}