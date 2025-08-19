#include "pch.h"
#include "Soul.h"
#include <iostream>

Soul::Soul() :
	m_pos(310, 230)
	, m_gravityDirection()
	, m_gravityVelocity()
	, m_size(16.f)
	, m_speed(140.f)
	, m_gravity(35.f)
	, m_mode(0)
	, m_isFalling(0)
	, m_isGrounded(0)
	, m_disableInput(0)
	, m_jumpKeyPressedWhileFalling(0)
	, m_pSoulTexture(new Texture("Soul.png"))
{

}

Soul::~Soul() {
	if (m_pSoulTexture) {
		delete m_pSoulTexture;
		m_pSoulTexture = nullptr;
	}
}

void Soul::Draw() const {
	if (m_mode) {
		//I figured out this formula to avoid using if statements
		m_pSoulTexture->Draw(m_pos, Rectf((1 + std::abs(1 + m_gravityDirection.y - 2.f * m_gravityDirection.x)) * (m_size + 1) + 1, 1, m_size, m_size));
	}
	else {
		m_pSoulTexture->Draw(m_pos, Rectf(1, 1, m_size, m_size));
	}
}

void Soul::Move(float elapsedSec, const Rectf& bounds, const std::vector<Platform*>& platforms) {

	//movement functionality itself
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	Vector2f direction{};

	if (!m_disableInput) {
		if ((pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]) && !m_gravityDirection.Equals(Vector2f{ 0, 1 })) { 
			direction.y += 1;
			if (m_isGrounded && !m_jumpKeyPressedWhileFalling && m_gravityDirection.Equals(Vector2f{ 0, -1 })) {
				m_isGrounded = 0;
			}
			if (m_isOnPlatform && !m_jumpKeyPressedWhileFalling && m_gravityDirection.Equals(Vector2f{ 0, -1 })) {
				m_isOnPlatform = 0;
			}
		}
		if ((pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]) && !m_gravityDirection.Equals(Vector2f{ 0, -1 })) { 
			direction.y -= 1; 
			if (m_isGrounded && !m_jumpKeyPressedWhileFalling && m_gravityDirection.Equals(Vector2f{ 0, 1 })) {
				m_isGrounded = 0;
			}
		}
		if ((pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A]) && !m_gravityDirection.Equals(Vector2f{ -1, 0 })) {
			direction.x -= 1; 
			if (m_isGrounded && !m_jumpKeyPressedWhileFalling && m_gravityDirection.Equals(Vector2f{ 1, 0 })) {
				m_isGrounded = 0;
			}
		}
		if ((pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]) && !m_gravityDirection.Equals(Vector2f{ 1, 0 })) { 
			direction.x += 1; 
			if (m_isGrounded && !m_jumpKeyPressedWhileFalling && m_gravityDirection.Equals(Vector2f{ -1, 0 })) {
				m_isGrounded = 0;
			}
		}
	}

	//direction isn't normalized on purpose, because that's how it is in the original game

	//makes sure gravity works when you press the jump button frame perfect
	if (m_mode && m_isGrounded && m_isFalling) {
		m_isGrounded = 0;
	}

	//disables movement along gravity axis while falling
	//also prevents the player from jumping immediately upon landing if they hold the jump key while falling
	if (m_mode && (m_isFalling || m_jumpKeyPressedWhileFalling)) {
		direction.x *= 1 - std::abs(m_gravityDirection.x);
		direction.y *= 1 - std::abs(m_gravityDirection.y);
	}

	if (m_mode && !m_isGrounded) {
		//applies double the gravity while falling and normal gravity while jumping
		const float gravMult{ m_isFalling ? 2.f : 1.f };
		m_gravityVelocity += m_gravityDirection * (m_gravity * gravMult * elapsedSec);
		direction += m_gravityVelocity * elapsedSec;
	}

	//once the player reaches the apex of the jump, falling starts automatically
	//once again, to prevent an immediate jump upon landing
	const float delta{ 0.05f };
	if (!m_isFalling && !m_isGrounded && m_mode &&
		((m_gravityDirection.y != 0 && std::abs(direction.y) < delta) ||
			(m_gravityDirection.x != 0 && std::abs(direction.x) < delta))) {
		m_jumpKeyPressedWhileFalling = 1;
		m_gravityVelocity = {};
		m_isFalling = 1;
	}

	//make the soul move with the platform
	if (m_isOnPlatform && m_pCurrentPlatform) {
		m_pos.x += m_pCurrentPlatform->GetSpeed() * elapsedSec;
		direction.x *= 0.7f;
	}

	m_pos += direction * (m_speed * elapsedSec);

	//velocity check for blue attacks
	if (!m_isGrounded && m_mode || !m_mode) {
		m_velocity = direction * m_speed;
	}
	else if (m_isGrounded) {
		m_velocity = direction * m_speed;
	}

	//check if the bottom quarter of the soul is overlapping with any platform
	for (Platform* platform : platforms) {
		if (utils::IsOverlapping(Rectf{ GetHitbox().left, GetHitbox().bottom, GetHitbox().width, GetHitbox().height / 4.f },
			Rectf{ platform->GetHitbox()})
			&& m_isFalling) {
			m_isGrounded = 1;
			m_isOnPlatform = 1;
			m_pCurrentPlatform = platform;
		}
	}

	//starts falling immediately if the soul leaves the platform on the x axis
	if (m_isOnPlatform && m_pCurrentPlatform && !(GetHitbox().left < m_pCurrentPlatform->GetHitbox().left + m_pCurrentPlatform->GetHitbox().width
		&& GetHitbox().left + GetHitbox().width > m_pCurrentPlatform->GetHitbox().left)) {
		m_isGrounded = 0;
		m_isFalling = 1;
		m_isOnPlatform = 0;
	}

	if (m_boundSnapping) {
		if (m_isOnPlatform && m_pCurrentPlatform) {
			BoundsSnapping(Rectf{ bounds.left, m_pCurrentPlatform->GetHitbox().bottom + m_pCurrentPlatform->GetHitbox().height, bounds.width, bounds.bottom + bounds.height - (m_pCurrentPlatform->GetHitbox().bottom + m_pCurrentPlatform->GetHitbox().height) });
            
		}
		else {
			BoundsSnapping(bounds);
		}
	}

	if (m_finalAttackX != 0.f) {
		m_pos.x = m_finalAttackX;
	}
}

void Soul::BoundsSnapping(const Rectf& bounds) {
	if (m_pos.x <= bounds.left) {
		m_pos.x = bounds.left;
		if (m_gravityDirection.Equals(Vector2f{ -1, 0 }) && m_mode) {
			m_gravityVelocity = {};
			m_isFalling = 0;
			m_isGrounded = 1;
			if (m_disableInput) {
				m_slamSound.Play(0);
				m_disableInput = 0;
			}
		}
	}

	if (m_pos.x >= bounds.left + bounds.width - m_size) {
		m_pos.x = bounds.left + bounds.width - m_size;
		if (m_gravityDirection.Equals(Vector2f{ 1, 0 }) && m_mode) {
			m_gravityVelocity = {};
			m_isFalling = 0;
			m_isGrounded = 1;
			if (m_disableInput) {
				m_slamSound.Play(0);
				m_disableInput = 0;
			}
		}
	}

	if (m_pos.y <= bounds.bottom) {
		m_pos.y = bounds.bottom;
		if (m_gravityDirection.Equals(Vector2f{ 0, -1 }) && m_mode) {
			m_gravityVelocity = {};
			m_isFalling = 0;
			m_isGrounded = 1;
			if (m_disableInput) {
				m_slamSound.Play(0);
				m_disableInput = 0;
			}
		}
	}

	if (m_pos.y >= bounds.bottom + bounds.height - m_size) {
		m_pos.y = bounds.bottom + bounds.height - m_size;
		if (m_gravityDirection.Equals(Vector2f{ 0, 1 }) && m_mode) {
			m_gravityVelocity = {};
			m_isFalling = 0;
			m_isGrounded = 1;
			if (m_disableInput) {
				m_slamSound.Play(0);
				m_disableInput = 0;
			}
		}
		else if (m_gravityDirection.Equals(Vector2f{ 0, -1 }) && m_mode) {
			m_isFalling = 1;
			m_jumpKeyPressedWhileFalling = 1;
			m_gravityVelocity = {};
		}
	}
}

void Soul::OnKeyDown(const SDL_KeyboardEvent& e) {
	if ((e.keysym.sym == SDLK_w && m_gravityDirection.Equals(Vector2f{ 0, -1 })) ||
		(e.keysym.sym == SDLK_UP && m_gravityDirection.Equals(Vector2f{ 0, -1 })) ||
		(e.keysym.sym == SDLK_s && m_gravityDirection.Equals(Vector2f{ 0, 1 })) ||
		(e.keysym.sym == SDLK_DOWN && m_gravityDirection.Equals(Vector2f{ 0, 1 })) ||
		(e.keysym.sym == SDLK_a && m_gravityDirection.Equals(Vector2f{ 1, 0 })) ||
		(e.keysym.sym == SDLK_LEFT && m_gravityDirection.Equals(Vector2f{ 1, 0 })) ||
		(e.keysym.sym == SDLK_d && m_gravityDirection.Equals(Vector2f{ -1, 0 })) ||
		(e.keysym.sym == SDLK_RIGHT && m_gravityDirection.Equals(Vector2f{ -1, 0 }))) {
		if (m_isFalling) {
			m_jumpKeyPressedWhileFalling = 1;
		}
		else if (m_isGrounded) {
			m_isGrounded = 0;
		}
	}
}

void Soul::OnKeyUp(const SDL_KeyboardEvent& e) {
	switch (e.keysym.sym) {
	case SDLK_w:
	case SDLK_UP:
		OnKeyUpHandling(Vector2f{ 0, -1 });
		break;
	case SDLK_s:
	case SDLK_DOWN:
		OnKeyUpHandling(Vector2f{ 0, 1 });
		break;
	case SDLK_a:
	case SDLK_LEFT:
		OnKeyUpHandling(Vector2f{ 1, 0 });
		break;
	case SDLK_d:
	case SDLK_RIGHT:
		OnKeyUpHandling(Vector2f{ -1, 0 });
		break;
	}
}

void Soul::OnKeyUpHandling(const Vector2f& dir) {
	if (m_gravityDirection.Equals(dir) && m_mode) {
		if (!m_isFalling) {
			m_gravityVelocity = {};
			m_isFalling = 1;
		}
		if (m_jumpKeyPressedWhileFalling) {
			m_jumpKeyPressedWhileFalling = 0;
		}
	}
}

void Soul::SetMode(bool newMode) {
	if (m_mode == newMode) {
		return;
	}
	m_dingSound.Play(0);
	m_mode = newMode;
	if (m_mode) {
		m_gravityDirection = Vector2f{ 0, -1 };
		m_isGrounded = 0;
		m_isFalling = 1;
		m_jumpKeyPressedWhileFalling = 0;
		m_gravityVelocity = {};
	}
	else {
		m_isOnPlatform = 0;
		m_gravityDirection = {};
		m_isFalling = 0;
	}
}

void Soul::SetGravityDirection(const Vector2f& newGravityDirection) {
	m_gravityDirection = newGravityDirection;
	m_isFalling = 1;
}

void Soul::Teleport(const Vector2f& newMiddlePos) {
	m_pos = Vector2f{newMiddlePos.x - GetHitbox().width, newMiddlePos.y - GetHitbox().height};
	if (m_mode) {
		m_isFalling = 1;
		m_isGrounded = 0;
		m_jumpKeyPressedWhileFalling = 0;
		m_gravityVelocity = {};
	}
}

Rectf Soul::GetHitbox() const{
	return Rectf{ m_pos.x + m_size / 4.f, m_pos.y + m_size / 4.f, m_size / 2.f, m_size / 2.f };
}

void Soul::DisableInput(bool newSetting)
{
	m_disableInput = newSetting;
}

void Soul::SetBoundSnapping(bool newSnapping)
{
	m_boundSnapping = newSnapping;
}

void Soul::Slam(const Vector2f& direction, float speed) {
	m_gravityDirection = direction;
	m_gravityVelocity = direction * speed;
	m_mode = 1;
	m_disableInput = 1;
	m_isFalling = 1;
	m_isGrounded = 0;
}