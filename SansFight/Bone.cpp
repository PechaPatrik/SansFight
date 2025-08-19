#include "pch.h"
#include "Bone.h"
#include <iostream>

Bone::Bone(const Vector2f& pos, const Rectf& battleBox, Type type, const Vector2f& direction, const Vector2f& size, float speed) :
    Weapon(pos)
    , m_speed(speed)
    , m_type(type)
    , m_size(size)
    , m_direction(direction)
    , m_BattleBound(battleBox)
	, m_warningTimer(50.f / speed)
    , m_pTexture(nullptr)
    , m_Hitbox()
    , m_MenuFirstMoveDistance(std::max(size.x, size.y))
{
    switch (m_type) {
    case Type::NORMAL:
        if (size.x != 0.f) {
            m_pTexture = new Texture("Weapons/bone_endsH.png");
            m_Hitbox = Rectf{ pos.x, pos.y, size.x, m_pTexture->GetHeight() };
        }
        else {
            m_pTexture = new Texture("Weapons/bone_endsV.png");
            m_Hitbox = Rectf{ pos.x, pos.y, m_pTexture->GetWidth(), size.y};
        }
        break;
    case Type::BLUE:
        if (size.x != 0.f) {
            m_pTexture = new Texture("Weapons/bone_endsH_blue.png");
            m_Hitbox = Rectf{ pos.x, pos.y, size.x, m_pTexture->GetHeight()};
        }
        else {
            m_pTexture = new Texture("Weapons/bone_endsV_blue.png");
            m_Hitbox = Rectf{ pos.x, pos.y, m_pTexture->GetWidth(), size.y};
        }

        break;
    case Type::MENU:
        m_pTexture = new Texture("Weapons/bone_menu.png");
        break;
    case Type::SPREAD:
        m_warningSound.Play();
        if (direction.x != 0.f) {
            m_pTexture = new Texture("Weapons/bone_spreadV.png");
        }
        else {
            m_pTexture = new Texture("Weapons/bone_spreadH.png");
        }
        break;
    }
}

Bone::~Bone() {
    if (m_pTexture) {
        delete m_pTexture;
        m_pTexture = nullptr;
    }
}

void Bone::Draw() const
{
    switch (m_type) {
    case Type::NORMAL:
    case Type::BLUE:
        if (m_type == Type::NORMAL) {
            utils::SetColor(Color4f{ 1, 1, 1, 1 });
        }
        else {
            utils::SetColor(Color4f{ 0, 1, 1, 1 });
        }
        if (m_size.x != 0) {
            m_pTexture->Draw(m_Pos, Rectf{0, 0, m_pTexture->GetWidth() / 2.f, m_pTexture->GetHeight()});
            utils::FillRect(Rectf{m_Pos.x + m_pTexture->GetWidth() / 2.f, m_Pos.y + 2.f, m_size.x - m_pTexture->GetWidth(), 6.f});
            m_pTexture->Draw(Vector2f{ m_Pos.x + m_size.x - m_pTexture->GetWidth() / 2.f, m_Pos.y }, Rectf{ m_pTexture->GetWidth() / 2.f, 0, m_pTexture->GetWidth() / 2.f, m_pTexture->GetHeight() });
        }
        else if (m_size.y != 0) {
			float maxY = m_BattleBound.bottom + m_BattleBound.height;
			float minY = m_BattleBound.bottom;
			float rectHeight = m_size.y - m_pTexture->GetHeight();
			if (m_Pos.y <= minY) {
				rectHeight = m_Pos.y + m_size.y - minY - m_pTexture->GetHeight() / 2.f;
			}
			else if (m_Pos.y + m_size.y >= maxY) {
				rectHeight = maxY - m_Pos.y - m_pTexture->GetHeight() / 2.f;
			}
            utils::FillRect(Rectf{ 
				m_Pos.x + 2.f, 
				(m_Pos.y > minY) ? (m_Pos.y + m_pTexture->GetHeight() / 2.f) : (minY), 
				6.f, 
				rectHeight
				});
			if (m_Pos.y + m_size.y < maxY) {
				m_pTexture->Draw(Vector2f{ m_Pos.x, m_Pos.y + m_size.y - m_pTexture->GetHeight() / 2.f }, Rectf{ 0, 0, m_pTexture->GetWidth(), m_pTexture->GetHeight() / 2.f });
			}
			if (m_Pos.y > minY) {
				m_pTexture->Draw(m_Pos, Rectf{ 0, m_pTexture->GetHeight() / 2.f, m_pTexture->GetWidth(), m_pTexture->GetHeight() / 2.f });
			}
        }
        break;
    case Type::MENU:
        m_pTexture->Draw(m_Pos);
        break;
    case Type::SPREAD:
        float texWidth = static_cast<float>(m_pTexture->GetWidth());
        float texHeight = static_cast<float>(m_pTexture->GetHeight());
        Rectf srcRect;
        Rectf destRect;

        if (m_direction.x == -1) {
            destRect = Rectf(m_BattleBound.left, m_BattleBound.bottom, m_size.x, texHeight);
            srcRect = Rectf(texWidth - m_size.x, 0, m_size.x, texHeight);
        }
        else if (m_direction.x == 1) {
            destRect = Rectf(m_BattleBound.left + m_BattleBound.width - m_size.x, m_BattleBound.bottom, m_size.x, texHeight);
            srcRect = Rectf(0, 0, m_size.x, texHeight);
        }
        else if (m_direction.y == -1) {
            destRect = Rectf(m_BattleBound.left, m_BattleBound.bottom, texWidth, m_size.y);
            srcRect = Rectf(0, 0, texWidth, m_size.y);
        }
        else if (m_direction.y == 1) {
            destRect = Rectf(m_BattleBound.left, m_BattleBound.bottom + m_BattleBound.height - m_size.y, texWidth, m_size.y);
            srcRect = Rectf(0, texHeight - m_size.y, texWidth, m_size.y);
        }

        if (m_warningTimer > 0.f) {
            utils::SetColor(Color4f{ 1, 0, 0, 1 });
            utils::DrawRect(destRect);
        }
        else {
            m_pTexture->Draw(destRect, srcRect);
        }

        break;
    }
}

Rectf Bone::GetHitbox() const
{
    return m_Hitbox;
}

bool Bone::GetBlue() const
{
    return m_type == Type::BLUE;
}

void Bone::Update(float elapsedSec)
{
	switch (m_type)
	{
	case Type::NORMAL:
	case Type::BLUE:
		m_Pos.x += m_direction.x * m_speed * elapsedSec;
		m_Pos.y += m_direction.y * m_speed * elapsedSec;

		if (!utils::IsOverlapping(m_Hitbox, Rectf{ 
			m_BattleBound.left + m_pTexture->GetWidth() / 2.f,
			m_BattleBound.bottom + m_pTexture->GetHeight() / 2.f, 
			m_BattleBound.width - m_pTexture->GetWidth(),
			m_BattleBound.height - m_pTexture->GetHeight() 
			}))
		{
			m_ShouldDestroy = true;
		}

		if (m_size.x != 0.f)
		{
			m_Hitbox = Rectf{
				m_Pos.x,
				m_Pos.y,
				m_size.x,
				static_cast<float>(m_pTexture->GetHeight())

			};
		}
		else
		{
			m_Hitbox = Rectf{
				m_Pos.x,
				m_Pos.y,
				static_cast<float>(m_pTexture->GetWidth()),
				m_size.y
			};
		}
		break;
	case Type::MENU:
		if (m_direction.Equals(Vector2f{ -1, 0 })) {
			switch (m_MenuMoveState) {
			case MenuMovementState::FIRST_MOVE:
				m_Pos.x += m_speed * elapsedSec;
				m_MenuMoveProgress += m_speed * elapsedSec;
				if (m_MenuMoveProgress >= m_MenuFirstMoveDistance) {
					m_MenuMoveState = MenuMovementState::FINAL_MOVE;
				}
				break;

			case MenuMovementState::FINAL_MOVE:
				m_Pos.x -= m_speed * elapsedSec;
				if (!utils::IsOverlapping(m_Hitbox, m_BattleBound)) {
					m_ShouldDestroy = true;
				}
				break;
			}
		}
		else if (m_direction.Equals(Vector2f{ 0, -1 })) {
			switch (m_MenuMoveState) {
			case MenuMovementState::FIRST_MOVE:
				m_Pos.y += m_speed * elapsedSec;
				m_MenuMoveProgress += m_speed * elapsedSec;
				if (m_MenuMoveProgress >= m_MenuFirstMoveDistance) {
					m_MenuMoveState = MenuMovementState::SECOND_MOVE;
					m_MenuMoveProgress = 0.f;
				}
				break;

			case MenuMovementState::SECOND_MOVE:
				m_Pos.x -= m_speed * elapsedSec;
				m_MenuMoveProgress += m_speed * elapsedSec;
				if (m_MenuMoveProgress >= m_MenuSecondMoveDistance) {
					m_MenuMoveState = MenuMovementState::FINAL_MOVE;
				}
				break;

			case MenuMovementState::FINAL_MOVE:
				m_Pos.y -= m_speed * elapsedSec;
				if (!utils::IsOverlapping(m_Hitbox, m_BattleBound)) {
					m_ShouldDestroy = true;
				}
				break;
			}
		}
		m_Hitbox = Rectf{ m_Pos.x, m_Pos.y, m_pTexture->GetWidth(), m_pTexture->GetHeight() };
		break;
	case Type::SPREAD:
		if (m_warningTimer > 0.f) {
			float previousTimer = m_warningTimer;
			m_warningTimer -= elapsedSec;
			// Check if we just crossed from positive to <= 0
			if (previousTimer > 0.f && m_warningTimer <= 0.f && !m_hasPlayedStabSound) {
				m_stabSound.Play();
				m_hasPlayedStabSound = true;
			}
			return;
		}
		if (!m_IsRetracting)
		{
			if (m_direction.x != 0) {
				m_size.x += m_speed * 2.f * elapsedSec;
				if (m_direction.x < 0) {
					m_Pos.x = m_BattleBound.left - m_size.x;
				}

				if (m_size.x >= m_MaxSize) {
					m_IsRetracting = true;
				}
			}
			else {
				m_size.y += m_speed * 2.f * elapsedSec;
				if (m_direction.y < 0) {
					m_Pos.y = m_BattleBound.bottom - m_size.y;
				}

				if (m_size.y >= m_MaxSize) {
					m_IsRetracting = true;
				}
			}
		}
		else
		{
			if (m_direction.x != 0) {
				m_size.x -= m_speed * 2.f * elapsedSec;
				if (m_direction.x < 0) {
					m_Pos.x = m_BattleBound.left - m_size.x;
				}

				if (m_size.x <= 0) {
					m_ShouldDestroy = true;
				}
			}
			else {
				m_size.y -= m_speed * 2.f * elapsedSec;
				if (m_direction.y < 0) {
					m_Pos.y = m_BattleBound.bottom - m_size.y;
				}

				if (m_size.y <= 0) {
					m_ShouldDestroy = true;
				}
			}
		}

		Rectf destRect{};
		float texWidth = static_cast<float>(m_pTexture->GetWidth());
		float texHeight = static_cast<float>(m_pTexture->GetHeight());
		if (m_direction.x == -1) {
			destRect = Rectf(m_BattleBound.left, m_BattleBound.bottom, m_size.x, texHeight);
		}
		else if (m_direction.x == 1) {
			destRect = Rectf(m_BattleBound.left + m_BattleBound.width - m_size.x, m_BattleBound.bottom, m_size.x, texHeight);
		}
		else if (m_direction.y == -1) {
			destRect = Rectf(m_BattleBound.left, m_BattleBound.bottom, texWidth, m_size.y);
		}
		else if (m_direction.y == 1) {
			destRect = Rectf(m_BattleBound.left, m_BattleBound.bottom + m_BattleBound.height - m_size.y, texWidth, m_size.y);
		}
		m_Hitbox = destRect;
		break;
	}
}