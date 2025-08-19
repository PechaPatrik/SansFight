#include "pch.h"
#include "Corridor.h"

Corridor::Corridor(const Rectf& viewport) :
	m_pCorridorTexture(new Texture("Corridor.png")),
	m_pFrisk(new Texture("Frisk.png")),
	m_pSansShadow(new Texture("SansShadow.png")),
	m_pSansFace(new Texture("Sans/sans_head_idle.png")),
	m_pPillar(new Texture("CorridorPillar.png")),
	m_pSoul(new Texture("Soul.png")),
	m_pTextBox(new TextBox(TextBox::Font::SANS, Vector2f{ 220.f, 410.f }, 250.f,
		{ "...", "that expression that you're wearing...", "...", "you're really kind of a freak, huh?" })),
	m_viewport(viewport)
{
	m_friskPos = Vector2f{ m_viewport.width / 4.f, m_viewport.height / 2.f - 80.f };
	m_shadowX = m_viewport.width - 80.f;
}

Corridor::~Corridor() {
	if (m_pCorridorTexture) {
		delete m_pCorridorTexture;
		m_pCorridorTexture = nullptr;
	}
	if (m_pFrisk) {
		delete m_pFrisk;
		m_pFrisk = nullptr;
	}
	if (m_pSansShadow) {
		delete m_pSansShadow;
		m_pSansShadow = nullptr;
	}
	if (m_pSansFace) {
		delete m_pSansFace;
		m_pSansFace = nullptr;
	}
	if (m_pPillar) {
		delete m_pPillar;
		m_pPillar = nullptr;
	}
	if (m_pSoul) {
		delete m_pSoul;
		m_pSoul = nullptr;
	}
	if (m_pTextBox) {
		delete m_pTextBox;
		m_pTextBox = nullptr;
	}
}

void Corridor::Draw() const {
	if (m_cutsceneState < CutsceneState::SOUL_APPEARING) {
		DrawCorridor();
		DrawFrisk();
		DrawPillars();
	}

	if (m_drawShadow) {
		m_pSansShadow->Draw(Rectf{ m_shadowX, m_friskPos.y, m_pSansShadow->GetWidth() * 2.f, m_pSansShadow->GetHeight() * 2.f }, Rectf{1, 1, m_pSansShadow->GetWidth() - 2, m_pSansShadow->GetHeight() - 2 });
	}

	if (m_textTime) {
		DrawTextBox();
		if (m_pSansFace) {
			m_pSansFace->Draw(Rectf{ 100.f, 360.f, 70.f, 70.f });
		}
	}

	if (m_cutsceneState >= CutsceneState::SOUL_APPEARING && m_soulVisible) {
		DrawSoul();
	}
}

void Corridor::Update(float elapsedSec) {
	if (m_cutsceneActive) {
		UpdateCutscene(elapsedSec);
	}

	if (m_corridorX >= 660.f && !m_cutsceneActive) {
		StartCutscene();
	}

	const float maxCorridorX = m_pCorridorTexture->GetWidth() - (m_viewport.width / 2.f);
	if (m_corridorX < 0.f) {
		m_corridorX = 0.f;
	}
	else if (m_corridorX > maxCorridorX) {
		m_corridorX = maxCorridorX;
	}

	if (m_textTime) {
		m_pTextBox->Update(elapsedSec);
		if (m_pTextBox->GetCurrentStringIndex() == 1) {
			delete m_pSansFace;
			m_pSansFace = new Texture("Sans/sans_head_lookLeft.png");
		}
		else if (m_pTextBox->GetCurrentStringIndex() == 2) {
			delete m_pSansFace;
			m_pSansFace = new Texture("Sans/sans_head_closedEyes.png");
		}
		else if (m_pTextBox->GetCurrentStringIndex() == 3) {
			delete m_pSansFace;
			m_pSansFace = new Texture("Sans/sans_head_noEyes.png");
		}
	}

	if (m_disableInput) return;

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	bool isMoving = 0;

	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W]) {
		m_lastDirection = 3;
		if (m_friskPos.y <= 190.f) {
			m_friskPos.y += m_cameraSpeed * 2.f * elapsedSec;
			isMoving = 1;
		}
	}
	if (pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]) {
		m_lastDirection = 0;
		if (m_friskPos.y >= m_viewport.bottom + m_viewport.height - m_pCorridorTexture->GetHeight() * 2.f) {
			m_friskPos.y -= m_cameraSpeed * 2.f * elapsedSec;
			isMoving = 1;
		}
	}
	if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A]) {
		m_lastDirection = 1;
		if ((m_corridorX == 0 || m_friskPos.x > m_viewport.left + m_viewport.width / 2.f - 20.f) && m_friskPos.x > 0.f) {
			m_friskPos.x -= m_cameraSpeed * 2.f * elapsedSec;
			isMoving = 1;
		}
		else if (m_friskPos.x <= 0.f) {
			m_friskPos.x = 0.f;
		}
		else {
			m_corridorX -= m_cameraSpeed * elapsedSec;
			isMoving = 1;
		}
	}
	if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]) {
		m_lastDirection = 2;
		if ((m_corridorX == maxCorridorX || m_friskPos.x < m_viewport.left + m_viewport.width / 2.f - 20.f) && m_friskPos.x < m_viewport.width) {
			m_friskPos.x += m_cameraSpeed * 2.f * elapsedSec;
			isMoving = 1;
		}
		else if (m_friskPos.x >= m_viewport.width) {
			m_friskPos.x = m_viewport.width;
		}
		else {
			m_corridorX += m_cameraSpeed * elapsedSec;
			isMoving = 1;
		}
	}

	m_animTimer += elapsedSec;
	if (isMoving && m_animTimer >= m_animSpeed) {
		m_animTimer = 0.f;
		if (m_lastDirection == 3 || m_lastDirection == 0) {
			m_animFrame = (m_animFrame + 1) % 4;
		}
		else {
			m_animFrame = (m_animFrame + 1) % 2;
		}
	}
	else if (!isMoving) {
		m_animFrame = 0;
	}
}

void Corridor::UpdateCutscene(float elapsedSec) {
	m_cutsceneTimer += elapsedSec;

	switch (m_cutsceneState) {
	case CutsceneState::WAIT_BEFORE_MOVE:
		if (m_cutsceneTimer >= 0.5f) {
			m_cutsceneState = CutsceneState::MOVING_CAMERA;
			m_targetCorridorX = m_corridorX + 90.f;
			m_cutsceneTimer = 0.f;
		}
		break;

	case CutsceneState::MOVING_CAMERA:
		m_corridorX += 100.f * elapsedSec;
		m_friskPos.x -= 200.f * elapsedSec;
		m_shadowX -= 200.f * elapsedSec;

		if (m_corridorX >= m_targetCorridorX) {
			m_corridorX = m_targetCorridorX;
			m_cutsceneState = CutsceneState::WAIT_AFTER_MOVE;
			m_cutsceneTimer = 0.f;
		}
		else  if (m_corridorX >= 680.f) {
			m_drawShadow = 1;
		}
		break;

	case CutsceneState::WAIT_AFTER_MOVE:
		if (m_cutsceneTimer >= 0.5f) {
			m_cutsceneState = CutsceneState::SHOWING_TEXT;
			m_textTime = 1;
			m_disableInput = 1;
			m_cutsceneTimer = 0.f;
		}
		break;

	case CutsceneState::SHOWING_TEXT:
		if (m_pTextBox->IsEndOfText()) {
			m_cutsceneState = CutsceneState::SOUL_APPEARING;
			m_textTime = 0;
			const float friskWidth = 40.f;
			const float friskHeight = 60.f;
			m_soulPos = Vector2f{ m_friskPos.x + friskWidth / 2.f - 16.f, m_friskPos.y + friskHeight / 2.f - 16.f };
			m_cutsceneTimer = 0.f;
		}
		break;

	case CutsceneState::SOUL_APPEARING:
		if (m_cutsceneTimer >= 0.04f) {
			m_soulVisible = !m_soulVisible;
			if (m_soulVisible) {
				m_soulFlash.Play();
			}
			m_cutsceneTimer = 0.f;
			if (++m_animFrame >= 4) {
				m_soulMove.Play();
				m_cutsceneState = CutsceneState::SOUL_MOVING;
			}
		}

		break;

	case CutsceneState::SOUL_MOVING:
		m_soulVisible = 1;
		m_soulPos.x += 1000.f * elapsedSec;
		if (m_soulPos.x >= 320.f) {
			m_soulPos.x = 320.f;
			m_cutsceneState = CutsceneState::COMPLETE;
			m_fightStarted = 1;
		}
		break;
	}
}

void Corridor::StartCutscene() {
	m_animFrame = 0;
	m_cutsceneActive = 1;
	m_disableInput = 1;
	m_cutsceneState = CutsceneState::WAIT_BEFORE_MOVE;
	m_cutsceneTimer = 0.f;
}

void Corridor::OnKeyDown(const SDL_KeyboardEvent& e) {
	if (m_textTime && m_pTextBox) {
		if (e.keysym.sym == SDLK_LSHIFT || e.keysym.sym == SDLK_RSHIFT || e.keysym.sym == SDLK_x) {
			m_pTextBox->SkipAnimation();
		}
		else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) {
			m_pTextBox->NextString();
			if (m_pTextBox->IsEndOfText()) {
				m_textTime = 0;
			}
		}
	}
}

void Corridor::DrawCorridor() const {
	m_pCorridorTexture->Draw(
		Rectf{ m_viewport.left, m_viewport.bottom + m_viewport.height - m_pCorridorTexture->GetHeight() * 2.f,
			  m_viewport.width, m_pCorridorTexture->GetHeight() * 2.f },
		Rectf{ m_corridorX, 0, m_viewport.width / 2.f, m_pCorridorTexture->GetHeight() }
	);
}

void Corridor::DrawPillars() const {
	for (int i{ 0 }; i < 8; i++) {
		m_pPillar->Draw(Rectf{ m_pCorridorTexture->GetWidth() - 1000.f + 500.f * i - m_corridorX * 4.f, 80, m_pPillar->GetWidth() * 2.f, m_pPillar->GetHeight() * 2.f });
		if (i == 5) {
			m_pPillar->Draw(Rectf{ m_pCorridorTexture->GetWidth() - 750.f + 500.f * i - m_corridorX * 4.f, 80, m_pPillar->GetWidth() * 2.f, m_pPillar->GetHeight() * 2.f });
		}
	}
}

void Corridor::DrawFrisk() const {
	const float frameWidth = 20.f;
	const float frameHeight = 30.f;

	int frameOffset = 0;
	int displayFrame = m_animFrame;

	switch (m_lastDirection) {
	case 0: frameOffset = 0; break;
	case 1: frameOffset = 4; displayFrame = std::min(m_animFrame, 1); break;
	case 2: frameOffset = 6; displayFrame = std::min(m_animFrame, 1); break;
	case 3: frameOffset = 8; break;
	}

	Rectf srcRect{ (frameOffset + displayFrame) * frameWidth, 0, frameWidth, frameHeight };
	Rectf destRect{ m_friskPos.x, m_friskPos.y, frameWidth * 2.f, frameHeight * 2.f };
	m_pFrisk->Draw(destRect, srcRect);
}

void Corridor::DrawTextBox() const {
	Rectf textBoxRect{ 70.f, 330.f, 500.f, 130.f };
	utils::SetColor(Color4f{ 0, 0, 0, 1 });
	utils::FillRect(textBoxRect);
	utils::SetColor(Color4f{ 1, 1, 1, 1 });
	utils::DrawRect(textBoxRect, 5.f);
	m_pTextBox->Draw();
}

void Corridor::DrawSoul() const {
	Rectf srcRect{ 0.f, 0.f, 16.f, 16.f };
	Rectf destRect{ m_soulPos.x, m_soulPos.y, 16.f, 16.f };
	m_pSoul->Draw(destRect, srcRect);
}