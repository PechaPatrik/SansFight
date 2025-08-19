#include "pch.h"
#include "Sans.h"
#include <iostream>

Sans::Sans(const Rectf& viewport) :
	m_totalMovedX(0)
	, m_accumulatedFrames(0)
	, m_pHeadTexture(new Texture("Sans/sans_head_idle.png"))
	, m_pBodyTexture(new Texture("Sans/sans_body_idle.png"))
	, m_pLegsTexture(new Texture("Sans/sans_legs.png"))
	, m_baseLegsPos(viewport.width / 2.f - m_pLegsTexture->GetWidth(), 250.f)
	, m_baseBodyPos(viewport.width / 2.f - m_pBodyTexture->GetWidth(), 250.f + m_pLegsTexture->GetHeight() * 2.f)
	, m_baseHeadPos(viewport.width / 2.f - m_pHeadTexture->GetWidth(), 250.f + m_pLegsTexture->GetHeight() * 2.f + m_pBodyTexture->GetHeight() * 3.f / 2.f)
{
	m_legsPos = m_baseLegsPos;
	m_bodyPos = m_baseBodyPos;
	m_headPos = m_baseHeadPos;

	for (int i{}; i < 3; i++) {
		m_pSweatTextures.push_back(new Texture("Sans/sans_sweat" + std::to_string(i) + ".png"));
	}
}

Sans::~Sans()
{
	if (m_pHeadTexture != nullptr) {
		delete m_pHeadTexture;
		m_pHeadTexture = nullptr;
	}
	if (m_pBodyTexture != nullptr) {
		delete m_pBodyTexture;
		m_pBodyTexture = nullptr;
	}
	if (m_pLegsTexture != nullptr) {
		delete m_pLegsTexture;
		m_pLegsTexture = nullptr;
	}
	for (Texture* texture : m_pSweatTextures) {
		delete texture;
		texture = nullptr;
	}
}

void Sans::Draw(BattleUI* battleUI) const
{
	if (m_isIdle) {
		if (m_pLegsTexture) {
			m_pLegsTexture->Draw(Rectf{ m_legsPos.x, m_legsPos.y, m_pLegsTexture->GetWidth() * 2.f, m_pLegsTexture->GetHeight() * 2.f });
		}
	}
	if (m_pBodyTexture) {
		m_pBodyTexture->Draw(Rectf{ m_bodyPos.x, m_bodyPos.y, m_pBodyTexture->GetWidth() * 2.f, m_pBodyTexture->GetHeight() * 2.f });
	}
	if (m_pHeadTexture) {
		m_pHeadTexture->Draw(Rectf{ m_headPos.x, m_headPos.y, m_pHeadTexture->GetWidth() * 2.f, m_pHeadTexture->GetHeight() * 2.f });
	}
	if (battleUI && battleUI->GetCurrentPattern() == 20 && m_pHeadTexture) {
		m_pSweatTextures[0]->Draw(Rectf{ m_headPos.x,
			m_headPos.y + m_pHeadTexture->GetHeight() * 2.f - m_pSweatTextures[0]->GetHeight() * 2.f + 2.f,
			m_pHeadTexture->GetWidth() * 2.f,
			m_pSweatTextures[0]->GetHeight() * 2.f },
			Rectf{
				1,
				1,
				m_pSweatTextures[0]->GetWidth() - 2,
				m_pSweatTextures[0]->GetHeight() - 2
			});
	}
	else if (battleUI && battleUI->GetCurrentPattern() >= 13 && battleUI->GetCurrentPattern() <= 23 && m_pHeadTexture) {
		m_pSweatTextures[1]->Draw(Rectf{ m_headPos.x, 
			m_headPos.y + m_pHeadTexture->GetHeight() * 2.f - m_pSweatTextures[1]->GetHeight() * 2.f + 2.f,
			m_pHeadTexture->GetWidth() * 2.f, 
			m_pSweatTextures[1]->GetHeight() * 2.f },
			Rectf{
				1,
				1,
				m_pSweatTextures[1]->GetWidth() - 2,
				m_pSweatTextures[1]->GetHeight() - 2
			});
	}
	else if (battleUI && battleUI->GetCurrentPattern() > 23 && m_pHeadTexture) {
		m_pSweatTextures[2]->Draw(Rectf{ m_headPos.x,
			m_headPos.y + m_pHeadTexture->GetHeight() * 2.f - m_pSweatTextures[2]->GetHeight() * 2.f + 2.f,
			m_pHeadTexture->GetWidth() * 2.f,
			m_pSweatTextures[2]->GetHeight() * 2.f },
			Rectf{
				1,
				1,
				m_pSweatTextures[2]->GetWidth() - 2,
				m_pSweatTextures[2]->GetHeight() - 2
			});
	}
}

void Sans::Update(float elapsedSec, const std::string& bodyAnimation, bool blueEye)
{
	if (blueEye) {
	Animate("head", "blueEye", 4, 1);
	}

	if (bodyAnimation != "none" && m_noAnimation) {
		m_noAnimation = 0;
	}

	if (m_noAnimation) {
		return;
	}

	if (bodyAnimation == "none")
	{
		m_isIdle = 1;
		m_accumulatedFrames = 0;
		m_noAnimation = 1;
	}

	else if (bodyAnimation == "idle")
	{
		m_isIdle = 1;
		m_accumulatedFrames++;
		if (m_accumulatedFrames >= 5)
		{
			static const int cycle[] = { -2, -1, 0, 1, 2, 1, 0, -1 };

			m_headMovement = cycle[m_idleIndex];
			m_headPos.x += m_headMovement;

			m_bodyMovement.x = static_cast<float>(cycle[m_idleIndex]);
			m_bodyPos.x += m_bodyMovement.x;

			if (static_cast<float>(cycle[m_idleIndex]) != 1 && static_cast<float>(cycle[m_idleIndex]) != -1) {
				m_bodyMovement.y = -m_bodyMovement.y;
				m_bodyPos.y += m_bodyMovement.y;
			}

			m_idleIndex = (m_idleIndex + 1) % 8;
			m_accumulatedFrames = 0;
		}
	}
	else if (bodyAnimation == "slamDown")
	{
		if (!m_bodyPos.Equals(Vector2f{ m_baseBodyPos.x - 8.f, m_baseLegsPos.y + 20.f }) || !m_headPos.Equals(Vector2f{ m_baseHeadPos.x - 3.f, m_baseHeadPos.y + 15.f })) {
			m_bodyPos = Vector2f{ m_baseBodyPos.x - 8.f, m_baseLegsPos.y + 20.f};
			m_headPos = Vector2f{ m_baseHeadPos.x - 3.f, m_baseHeadPos.y + 15.f };
		}
		Animate("body", "down", 4);
	}
	else if (bodyAnimation == "slamLeft")
	{
		if (!m_bodyPos.Equals(Vector2f{ m_baseBodyPos.x - 14.f, m_baseLegsPos.y + 20.f }) || !m_headPos.Equals(Vector2f{ m_baseHeadPos.x - 3.f, m_baseHeadPos.y + 20.f })) {
			m_bodyPos = Vector2f{ m_baseBodyPos.x - 14.f, m_baseLegsPos.y + 20.f };
			m_headPos = Vector2f{ m_baseHeadPos.x - 3.f, m_baseHeadPos.y + 20.f };
		}
		Animate("body", "left", 5);
	}
	else if (bodyAnimation == "slamUp") {
		if (!m_bodyPos.Equals(Vector2f{ m_baseBodyPos.x - 8.f, m_baseLegsPos.y + 20.f }) || !m_headPos.Equals(Vector2f{ m_baseHeadPos.x - 3.f, m_baseHeadPos.y + 20.f })) {
			m_bodyPos = Vector2f{ m_baseBodyPos.x - 8.f, m_baseLegsPos.y + 20.f };
			m_headPos = Vector2f{ m_baseHeadPos.x - 3.f, m_baseHeadPos.y + 20.f };
		}
		Animate("body", "up", 5);
	}
	else if (bodyAnimation == "slamRight") {
		if (!m_bodyPos.Equals(Vector2f{ m_baseBodyPos.x - 14.f, m_baseLegsPos.y - 20.f }) || !m_headPos.Equals(Vector2f{ m_baseHeadPos.x, m_baseHeadPos.y + 20.f })) {
			m_bodyPos = Vector2f{ m_baseBodyPos.x - 14.f, m_baseLegsPos.y + 20.f };
			m_headPos = Vector2f{ m_baseHeadPos.x, m_baseHeadPos.y + 20.f };
		}
		Animate("body", "right", 5);

	}
}

void Sans::Move(const Vector2f& direction)
{
	m_headPos += direction;
	m_bodyPos += direction;
	m_legsPos += direction;
}

void Sans::ChangeTexture(const std::string& bodyPart, const std::string& type) {
	const std::string newPath = "Sans/sans_" + bodyPart + "_" + type + ".png";

	if (bodyPart == "head") {
		if (m_pHeadTexture->GetImagePath() == newPath) {
			return;
		}
		else if (!SafeTextureChange(m_pHeadTexture, newPath)) {
			m_pHeadTexture = nullptr;
		}
	}
	else if (bodyPart == "body") {
		if (m_pBodyTexture->GetImagePath() == newPath) {
			return;
		}
		else if (!SafeTextureChange(m_pBodyTexture, newPath)) {
			m_pBodyTexture = nullptr;
		}
		if (type == "shrug") {
			m_bodyPos.x -= 18.f;
		}
	}
}

void Sans::Animate(const std::string& bodyPart, const std::string& baseName, int frameCount, bool isInfinite, bool allowRepeat)
{
	AnimationState* state{ nullptr };
	if (bodyPart == "head") state = &m_headAnimation;
	else if (bodyPart == "body") state = &m_bodyAnimation;
	else return;

	if (state->currentAnim != baseName || m_isIdle || allowRepeat) {
		state->currentAnim = baseName;
		state->currentFrame = 0;
		state->isPlaying = true;
		m_isIdle = false;
	}

	if (frameCount <= 1 || (!isInfinite && state->currentFrame >= frameCount)) {
		state->isPlaying = false;
		return;
	}

	std::string textureName = baseName + std::to_string(state->currentFrame % frameCount);
	ChangeTexture(bodyPart, textureName);

	if (state->isPlaying) {
		state->currentFrame++;
	}
}

void Sans::ResetSans(bool changeTextures) {

	m_legsPos = m_baseLegsPos;
	m_bodyPos = m_baseBodyPos;
	m_headPos = m_baseHeadPos;

	m_idleIndex = 0;
	m_headMovement = 0;
	m_bodyMovement = Vector2f{ 0.f, 1.f };

	if (changeTextures) {
		ChangeTexture("head", "idle");
		ChangeTexture("body", "idle");
	}
	else if (m_pBodyTexture->GetImagePath() == "Sans/sans_body_shrug.png") {
		m_bodyPos.x -= 18.f;
	}
}

bool Sans::SafeTextureChange(Texture*& existingTex, const std::string& path) {
	Texture* newTexture = new Texture(path);
	if (!newTexture->IsCreationOk()) {
		delete newTexture;
		std::cerr << "Failed to create texture: " << path << "\n";
		return false;
	}
	delete existingTex;
	existingTex = newTexture;
	return true;
}