#include "pch.h"
#include "BattleUI.h"
#include "Sans.h"
#include "AttackManager.h"
#include <iostream>

BattleUI::BattleUI(Soul* soul, Sans* sans, AttackManager* attackManager) :
	m_pSoul(soul),
	m_pSans(sans),
	m_pAttackManager(attackManager),
	m_battleBox(),
	m_battleBoxLineWidth(5.f),
	m_HP(m_maxHP),
	m_pHPtexture(new Texture("BattleUI/HP.png")),
	m_pKRtexture(new Texture("BattleUI/KR.png")),
	m_pDamageFont(TTF_OpenFont("Fonts/DamageFont.TTF", 26)),
	m_pUI_Font(TTF_OpenFont("Fonts/UI_Font.ttf", 15)),
	m_pEndScreenFont(TTF_OpenFont("Fonts/DamageFont.TTF", 80)),
	m_pEndScreenText(nullptr),
	m_pRestartPrompt(new Texture("Press [ENTER/Z] to restart", m_pUI_Font, Color4f(1, 1, 1, 1))),
	m_pAttackMenuTexture(new Texture("< 0 >", m_pEndScreenFont, Color4f(1, 1, 1, 1))),
	m_pAttackMenuHelpTexture(new Texture("Select an attack pattern", m_pUI_Font, Color4f(1, 1, 1, 1))),
	m_pHPtext(new Texture("92 / 92", m_pUI_Font, Color4f(1, 1, 1, 1))),
	m_pNameAndLevel(new Texture("CHARA  LV 19", m_pUI_Font, Color4f(1, 1, 1, 1))),
	m_pSpeechBubble(new Texture("BattleUI/speech_bubble.png")),
	m_pSansTextBox(new TextBox(TextBox::Font::SANS, Vector2f{ 425, 355 }, 180, "ready?", 0, Color4f{ 0, 0, 0, 1 })),
	m_pDefaultTextBox(new TextBox(TextBox::Font::DEFAULT, Vector2f{ 130, 185 }, 420, "This is a default text appearing in the menu")),
	m_pTarget(new Texture("BattleUI/target.png")),
	m_pMissText(new Texture("MISS", m_pDamageFont, Color4f{ 0.75f, 0.75f, 0.75f, 1.f })),
	m_selectedIndex(-1),
	m_currentButtonText("This is a default text appearing in the menu")
{
	ChangeBattleBox(155, 155);
	m_pButtons.push_back(new Texture("BattleUI/fight.png"));
	m_pButtons.push_back(new Texture("BattleUI/act.png"));
	m_pButtons.push_back(new Texture("BattleUI/item.png"));
	m_pButtons.push_back(new Texture("BattleUI/mercy.png"));

	m_pSelectedButtons.push_back(new Texture("BattleUI/fight_select.png"));
	m_pSelectedButtons.push_back(new Texture("BattleUI/act_select.png"));
	m_pSelectedButtons.push_back(new Texture("BattleUI/item_select.png"));
	m_pSelectedButtons.push_back(new Texture("BattleUI/mercy_select.png"));

	m_pTargetChoice.push_back(new Texture("BattleUI/target_choice0.png"));
	m_pTargetChoice.push_back(new Texture("BattleUI/target_choice1.png"));

	for (int i{}; i < 5; i++) {
		m_pStrike.push_back(new Texture("BattleUI/strike" + std::to_string(i) + ".png"));
	}
}

BattleUI::~BattleUI() {
	for (Texture* tex : m_pButtons) {
		if (tex) {
			delete tex;
			tex = nullptr;
		}
	}
	m_pButtons.clear();

	for (Texture* tex : m_pSelectedButtons) {
		if (tex) {
			delete tex;
			tex = nullptr;
		}
	}
	m_pSelectedButtons.clear();

	for (Texture* tex : m_pTargetChoice) {
		if (tex) {
			delete tex;
			tex = nullptr;
		}
	}
	m_pTargetChoice.clear();

	for (Texture* tex : m_pStrike) {
		if (tex) {
			delete tex;
			tex = nullptr;
		}
	}
	m_pStrike.clear();

	if (m_pHPtexture) {
		delete m_pHPtexture;
		m_pHPtexture = nullptr;
	}

	if (m_pKRtexture) {
		delete m_pKRtexture;
		m_pKRtexture = nullptr;
	}

	if (m_pHPtext) {
		delete m_pHPtext;
		m_pHPtext = nullptr;
	}

	if (m_pNameAndLevel) {
		delete m_pNameAndLevel;
		m_pNameAndLevel = nullptr;
	}

	if (m_pTarget) {
		delete m_pTarget;
		m_pTarget = nullptr;
	}

	if (m_pSpeechBubble) {
		delete m_pSpeechBubble;
		m_pSpeechBubble = nullptr;
	}

	if (m_pSansTextBox) {
		delete m_pSansTextBox;
		m_pSansTextBox = nullptr;
	}

	if (m_pDefaultTextBox) {
		delete m_pDefaultTextBox;
		m_pDefaultTextBox = nullptr;
	}

	if (m_pMissText) {
		delete m_pMissText;
		m_pMissText = nullptr;
	}

	if (m_pEndScreenText) {
		delete m_pEndScreenText;
		m_pEndScreenText = nullptr;
	}

	if (m_pRestartPrompt) {
		delete m_pRestartPrompt;
		m_pRestartPrompt = nullptr;
	}

	if (m_pAttackMenuTexture) {
		delete m_pAttackMenuTexture;
		m_pAttackMenuTexture = nullptr;
	}

	if (m_pAttackMenuHelpTexture) {
		delete m_pAttackMenuHelpTexture;
		m_pAttackMenuHelpTexture = nullptr;
	}

	if (m_pDamageFont) {
		TTF_CloseFont(m_pDamageFont);
		m_pDamageFont = nullptr;
	}

	if (m_pUI_Font) {
		TTF_CloseFont(m_pUI_Font);
		m_pUI_Font = nullptr;
	}

	if (m_pEndScreenFont) {
		TTF_CloseFont(m_pEndScreenFont);
		m_pEndScreenFont = nullptr;
	}
}

void BattleUI::Draw() const {

	if (m_isEndScreen) {
		Vector2f textPos(
			320 - m_pEndScreenText->GetWidth() / 2,
			240 - m_pEndScreenText->GetHeight() / 2
		);
		m_pEndScreenText->Draw(textPos);
		m_pRestartPrompt->Draw(Vector2f{ 320 - m_pRestartPrompt->GetWidth() / 2, textPos.y - 150.f });
		return;
	}

	if (m_selectingAttack) {
		Vector2f textPos(
			320 - m_pAttackMenuTexture->GetWidth() / 2,
			240 - m_pAttackMenuTexture->GetHeight() / 2
		);
		m_pAttackMenuTexture->Draw(textPos);
		m_pAttackMenuHelpTexture->Draw(Vector2f{ 320 - m_pAttackMenuHelpTexture->GetWidth() / 2, 240 + m_pAttackMenuTexture->GetHeight() / 2 + 100.f });
		return;
	}

	utils::SetColor(Color4f{ 1, 1, 1, 1 });
	utils::DrawRect(m_battleBox, m_battleBoxLineWidth);
	DrawHPBar();
	DrawButtons();

	if (m_attackState != AttackState::INACTIVE) {
		float targetX = GetBattleBounds().left + (GetBattleBounds().width / 2.f * (1.f - m_targetScale));
		float targetY = GetBattleBounds().bottom;

		m_pTarget->Draw(Rectf{ targetX, targetY, GetBattleBounds().width * m_targetScale, GetBattleBounds().height });

		if (m_attackState == AttackState::TARGET_MOVING || m_attackState == AttackState::TARGET_FLASHING) {
			int choiceIdx = (m_attackState == AttackState::TARGET_FLASHING && (m_flashFrame / 3) % 2) ? 1 : 0;
			float choiceY = m_battleBox.bottom + m_battleBox.height / 2 - m_pTargetChoice[0]->GetHeight() / 2;
			m_pTargetChoice[choiceIdx]->Draw(Vector2f{ m_targetX, choiceY });
		}

		if (m_pMissText && (m_attackState == AttackState::SHOWING_RESULT || m_attackState == AttackState::SHRINKING_TARGET)) {
			float resultX = m_battleBox.left + m_battleBox.width / 2 - m_pMissText->GetWidth() / 2;
			m_pMissText->Draw(Vector2f{ resultX, m_resultY });

			if (m_hit && m_strikeFrame < m_pStrike.size()) {
				float strikeY = 350.f - (m_strikeFrame * 20.f);
				if (m_strikeFrame >= m_pStrike.size() - 2) {
					strikeY = 350.f - ((m_pStrike.size() - 2) * 20.f);
				}
				resultX = m_battleBox.left + m_battleBox.width / 2;
				m_pStrike[m_strikeFrame]->Draw(Vector2f(resultX, strikeY));
			}
		}

		if (m_attackState == AttackState::SHRINKING_TARGET) {
			utils::SetColor(Color4f{ 0, 0, 0, 1 - m_targetScale });
			utils::FillRect(Rectf{ targetX, targetY, GetBattleBounds().width * m_targetScale, GetBattleBounds().height });
			utils::SetColor(Color4f{ 1, 1, 1, 1 });
		}
	}
	if (m_selectedIndex >= 0 && m_selectedIndex != 8) {
		m_pDefaultTextBox->Draw();
	}

	if (m_sansTextTime) {
		DrawSansSpeech();
	}
}

void BattleUI::DrawHPBar() const {
	m_pHPtexture->Draw(Vector2f(m_HPBarStartPos.x - m_pHPtexture->GetWidth() - 10.f, m_HPBarStartPos.y + m_HPBarHeight / 2.f - m_pHPtexture->GetHeight() / 2.f));
	m_pKRtexture->Draw(Vector2f(m_HPBarStartPos.x + static_cast<float>(m_maxHP) + 10.f, m_HPBarStartPos.y + m_HPBarHeight / 2.f - m_pKRtexture->GetHeight() / 2.f));
	m_pNameAndLevel->Draw(Vector2f(63.f, m_HPBarStartPos.y + m_HPBarHeight / 2.f - m_pNameAndLevel->GetHeight() / 2.f));
	m_pHPtext->Draw(Vector2f(m_HPBarStartPos.x + static_cast<float>(m_maxHP) + 50.f, m_HPBarStartPos.y + m_HPBarHeight / 2.f - m_pHPtext->GetHeight() / 2.f));

	utils::SetColor(Color4f{ 191, 0, 0, 1 });
	utils::FillRect(m_HPBarStartPos, static_cast<float>(m_maxHP), m_HPBarHeight);

	utils::SetColor(Color4f{ 255, 255, 0, 1 });
	utils::FillRect(m_HPBarStartPos, static_cast<float>(m_HP), m_HPBarHeight);

	utils::SetColor(Color4f{ 255, 0, 255, 1 });
	utils::FillRect(Vector2f{ m_HPBarStartPos.x + static_cast<float>(m_HP), m_HPBarStartPos.y }, static_cast<float>(m_karmaHP), m_HPBarHeight);
}

void BattleUI::DrawButtons() const {
	for (size_t i = 0; i < m_pButtons.size(); ++i) {
		if (i == m_selectedIndex)
			m_pSelectedButtons[i]->Draw(Vector2f(63.f + i * (25.f + m_pSelectedButtons[i]->GetWidth()), 15.f));
		else
			m_pButtons[i]->Draw(Vector2f(63.f + i * (25.f + m_pButtons[i]->GetWidth()), 15.f));
	}
}

void BattleUI::DrawSansSpeech() const {
	if (m_pSpeechBubble) {
		m_pSpeechBubble->Draw(Rectf{ 390, m_pSans->GetLegPos().y + 30.f, m_pSpeechBubble->GetWidth(), m_pSpeechBubble->GetHeight() });
	}
	if (m_pSansTextBox) {
		m_pSansTextBox->Draw();
	}
}

void BattleUI::Update(float elapsedSec) {

	if (m_currentPattern >= 25) {
		if (m_pEndScreenText) {
			delete m_pEndScreenText;
			m_pEndScreenText = nullptr;
		}
		m_pEndScreenText = new Texture("YOU WON", m_pEndScreenFont, Color4f(0, 1, 0, 1));
		m_isEndScreen = 1;
	}
	else if (m_HP <= 0) {
		if (m_pEndScreenText) {
			delete m_pEndScreenText;
			m_pEndScreenText = nullptr;
		}
		m_pEndScreenText = new Texture("YOU DIED", m_pEndScreenFont, Color4f(1, 0, 0, 1));
		m_isEndScreen = 1;
	}

	if (m_karmaHP > 0) {
		UpdateHPbar();
	}

	if (m_isAnimating) {
		UpdateBattleBox(elapsedSec);
	}

	if (m_sansTextTime) {
		m_pSansTextBox->Update(elapsedSec);
	}

	if (m_selectedIndex >= 0) {
		m_pDefaultTextBox->Update(elapsedSec);
	}

	if (m_textNeedsUpdate) {
		m_textNeedsUpdate = 0;

		if (m_pDefaultTextBox) {
			delete m_pDefaultTextBox;
			m_pDefaultTextBox = nullptr;
		}

		switch (m_selectedIndex) {
		case -1: case 0: case 1: case 2: case 3:
			if (m_currentPattern == 0) {
				m_currentButtonText = "You feel like you're going to have a bad time.";
			}
			else if (m_currentPattern == 3) {
				m_currentButtonText = "Sans's movements grow a little wearier.";
			}
			else if (m_currentPattern == 8) {
				m_currentButtonText = "Sans's movements seem to be slower.";
			}
			else if (m_currentPattern > 0 && m_currentPattern < 13) {
				const std::string options[] = {
					"Just keep attacking.",
					"You felt your sins crawling on your back."
				};
				m_currentButtonText = options[rand() % (sizeof(options) / sizeof(options[0]))];
			}
			else if (m_currentPattern == 13) {
				m_currentButtonText = "Sans is not holding back anymore.";
			}
			else if (m_currentPattern == 21) {
				m_currentButtonText = "Sans is preparing something.";
			}
			else if (m_currentPattern == 22) {
				m_currentButtonText = "Sans is getting ready to use his special attack.";
			}
			else if (m_currentPattern > 13) {
				const std::string options[] = {
					"Felt like a turning point.",
					"The REAL battle finally begins.",
					"Reading this doesn't seem like the best use of time.",
					"Sans is starting to look really tired.",
					"Just keep attacking.",
					"You felt your sins crawling on your back."
				};
				m_currentButtonText = options[rand() % (sizeof(options) / sizeof(options[0]))];
			}
			break;
		case 4: m_currentButtonText = "Sans"; break;
		case 5: m_currentButtonText = "Check"; break;
		case 6: m_currentButtonText = "Infinite steak"; break;
		case 7: m_currentButtonText = "You chose this, no running away now"; break;
		case 8:
			if (!m_attackTriggered) {
				m_attackTriggered = 1;
				Attack();
			}
			m_currentButtonText = "Attack!";
			break;
		case 9: m_currentButtonText = "The easiest enemy. Can only deal 1 damage."; break;
		case 10:
			if (m_HP + m_karmaHP + 60 >= m_maxHP) {
				m_HP = m_maxHP - m_karmaHP;
				m_currentButtonText = "HP fully restored!";
			}
			else {
				m_HP += 60;
				m_currentButtonText = "You recovered 60 HP";
			}
			UpdateHPbar();
			UpdateHPText();
			break;
		}

		m_pDefaultTextBox = new TextBox(TextBox::Font::DEFAULT, Vector2f{ 130, 185 }, 420, m_currentButtonText);
		if (m_selectedIndex >= 4 && m_selectedIndex < 8) {
			m_pDefaultTextBox->SkipAnimation();
		}
	}

	if (m_selectedIndex != 8) {
		m_attackTriggered = 0;
	}

	if (m_attackState != AttackState::INACTIVE) {
		UpdateAttack(elapsedSec);
	}

	if (m_isSansMoving) {
		m_sansTimer += elapsedSec;
		const float moveSpeed = 200.f;

		if (m_sansTimer <= 0.5f) {
			float moveAmount = moveSpeed * elapsedSec;
			m_pSans->Move(Vector2f(-moveAmount, 0.f));
		}
		else if (m_sansTimer > 1.5f && m_sansTimer <= 2.f) {
			float moveAmount = moveSpeed * elapsedSec;
			m_pSans->Move(Vector2f(moveAmount, 0.f));
		}
		else if (m_sansTimer > 2.f) {
			m_sansTimer = 0.f;
			m_isSansMoving = 0;
			m_pSans->ResetSans();
		}
	}
}

void BattleUI::UpdateAttack(float elapsedSec) {
	m_attackTimer += elapsedSec;

	switch (m_attackState) {
	case AttackState::TARGET_MOVING:
		m_targetX += 300.f * elapsedSec;
		if (m_targetX > m_battleBox.left + m_battleBox.width) {
			m_extraRounds += 1;
			m_attackState = AttackState::SHOWING_RESULT;
			m_pMissText = new Texture("MISS", m_pDamageFont, Color4f{ 0.75f, 0.75f, 0.75f, 1.f });
			m_resultY = 350.f;
			m_resultVelocity = 10.f;
			m_hit = 0;
			m_attackTimer = 0.f;
		}
		break;

	case AttackState::TARGET_FLASHING:
		m_flashFrame++;
		if (m_attackTimer >= 1.f) {
			m_attackState = AttackState::SHOWING_RESULT;
			m_pMissText = new Texture("MISS", m_pDamageFont, Color4f{ 0.75f, 0.75f, 0.75f, 1.f });
			m_resultY = 350.f;
			m_resultVelocity = 10.f;
			m_hit = 1;
			m_hitSound.Play();
			m_attackTimer = 0.f;
		}
		break;

	case AttackState::SHOWING_RESULT:
		m_resultVelocity -= 0.5f;
		m_resultY += m_resultVelocity;

		if (m_hit) {
			if (m_attackTimer > 0.01f * m_strikeFrame && m_strikeFrame < m_pStrike.size()) {
				m_strikeFrame++;
			}
		}

		if (m_attackTimer > 1.0f) {
			m_attackState = AttackState::SHRINKING_TARGET;
			m_attackTimer = 0.f;
			m_targetScale = 1.f;
		}
		break;

	case AttackState::SHRINKING_TARGET:
		m_targetScale = std::max(0.f, m_targetScale - elapsedSec * 2.f);

		if (m_targetScale <= 0.01f) {
			m_attackTimer = 0.f;
			m_attackState = AttackState::INACTIVE;
			m_selectedIndex = -1;
			m_textNeedsUpdate = 1;
			if (m_pMissText) {
				delete m_pMissText;
				m_pMissText = nullptr;
			}
			SetInMenu(0, m_pSoul);
			if (m_currentPattern == 12 && m_extraRounds > 0) {
				int randomPattern{ (rand() % 12) + 1 };
				m_extraRounds--;

				while (randomPattern == 2) {
					randomPattern = (rand() % 12) + 1;
				}
				m_pAttackManager->StartPattern(static_cast<size_t>(randomPattern));
			}
			else if (m_currentPattern == 22 && m_extraRounds > 0) {
				m_extraRounds--;
				m_pAttackManager->StartPattern(static_cast<size_t>((rand() % 9) + 14));
			}
			else {
				m_currentPattern++;
				m_pAttackManager->StartPattern(m_currentPattern);
			}
		}
		break;
	}
}

void BattleUI::SetButtonSelection(int index) {
	if (m_selectedIndex != index) {
		if (!(m_selectedIndex >= 0 && m_selectedIndex < 4 && index >= 0 && index < 4)) {
			m_textNeedsUpdate = 1;
		}
		m_selectedIndex = index;
	}
}

void BattleUI::OnKeyPress(const SDL_KeyboardEvent& e) {
	if (m_isEndScreen) {
		if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) {
			m_isEndScreen = 0;
			m_shouldReset = 1;
		}
		return;
	}

	if (m_selectingAttack) {
		if (e.keysym.sym == SDLK_d || e.keysym.sym == SDLK_RIGHT) {
			m_menuSelectedAttack = (m_menuSelectedAttack + 1) % 24;
			m_menuCursorSound.Play(0);
		}
		else if (e.keysym.sym == SDLK_a || e.keysym.sym == SDLK_LEFT) {
			m_menuSelectedAttack = (m_menuSelectedAttack + 23) % 24;
			m_menuCursorSound.Play(0);
		} else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) {
			m_selectingAttack = 0;
			m_extraRounds = 0;
			m_currentPattern = m_menuSelectedAttack;
			m_karmaHP = 0;
			m_HP = m_maxHP;
			UpdateHPbar();
			UpdateHPText();
			m_pAttackManager->StartPattern(m_currentPattern);
		}
		if (m_pAttackMenuTexture) {
			delete m_pAttackMenuTexture;
			m_pAttackMenuTexture = nullptr;
		}
		m_pAttackMenuTexture = new Texture("< " + std::to_string(m_menuSelectedAttack) + " >", m_pEndScreenFont, Color4f(1, 1, 1, 1));
		return;
	}

	constexpr int buttonCount{ 4 };
	constexpr float moveDistance{ 135.f };
	int currentButton{ m_selectedIndex };

	if (m_attackState != AttackState::INACTIVE) {
		if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) && m_attackState == AttackState::TARGET_MOVING) {
			m_pSans->ResetSans();
			m_isSansMoving = 1;
			m_sansTimer = 0.f;
			m_attackTimer = 0.f;
			m_attackState = AttackState::TARGET_FLASHING;
		}
		return;
	}

	if (m_sansTextTime) {
		if (e.keysym.sym == SDLK_LSHIFT || e.keysym.sym == SDLK_RSHIFT || e.keysym.sym == SDLK_x) {
			m_pSansTextBox->SkipAnimation();
		}
		else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) {
			m_pSansTextBox->NextString();
			if (m_pSansTextBox->IsEndOfText()) {
				m_sansTextTime = 0;
			}
		}
		return;
	}

	if (m_isTeleported) {
		if ((e.keysym.sym == SDLK_LSHIFT || e.keysym.sym == SDLK_RSHIFT || e.keysym.sym == SDLK_x) && currentButton < 8) {
			m_isTeleported = 0;
			SetButtonSelection(m_selectedIndex - buttonCount);
			if (m_pSoul) m_pSoul->Teleport(Vector2f{ 80.f + moveDistance * m_selectedIndex, 36.f });
			m_menuCursorSound.Play(0);
		}
		else if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) && currentButton < 7) {
			if (currentButton == 6) m_healSound.Play();
			SetButtonSelection(m_selectedIndex + buttonCount);
			if (m_pSoul) m_pSoul->Teleport(Vector2f{ -50.f, -50.f });
			m_menuSelectSound.Play(0);
		}
		else if ((e.keysym.sym == SDLK_LSHIFT || e.keysym.sym == SDLK_RSHIFT || e.keysym.sym == SDLK_x) && currentButton > 8) {
			m_pDefaultTextBox->SkipAnimation();
		}
		else if ((e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) && currentButton > 8) {
			m_pDefaultTextBox->NextString();
			if (m_pDefaultTextBox->IsEndOfText()) {
				SetInMenu(0, m_pSoul);
			}
			else {
				return;
			}

			if (m_currentPattern == 13) {
				SetInMenu(1, m_pSoul);
				return;
			}

			m_extraRounds++;
			if (m_currentPattern == 12 && m_extraRounds > 0) {
				int randomPattern{ (rand() % 12) + 1 };

				while (randomPattern == 2) {
					randomPattern = (rand() % 12) + 1;
				}
				m_pAttackManager->StartPattern(static_cast<size_t>(randomPattern));
			}
			else if (m_currentPattern == 22 && m_extraRounds > 0) {
				m_pAttackManager->StartPattern(static_cast<size_t>((rand() % 10) + 14));
			}
			else {
				m_currentPattern++;
				m_pAttackManager->StartPattern(m_currentPattern);
			}
		}
		return;
	}

	if (e.keysym.sym == SDLK_d || e.keysym.sym == SDLK_RIGHT) {
		currentButton = (currentButton + 1) % buttonCount;
		m_menuCursorSound.Play(0);
	}
	else if (e.keysym.sym == SDLK_a || e.keysym.sym == SDLK_LEFT) {
		currentButton = (currentButton - 1 + buttonCount) % buttonCount;
		m_menuCursorSound.Play(0);
	}
	else if (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z) {
		SetButtonSelection(m_selectedIndex + buttonCount);
		m_isTeleported = 1;
		if (m_pSoul) m_pSoul->Teleport(Vector2f{ 90.f, 200.f });
		m_menuSelectSound.Play(0);
		return;
	}

	SetButtonSelection(currentButton);
	if (m_pSoul) m_pSoul->Teleport(Vector2f{ 80.f + moveDistance * currentButton, 36.f });
}

void BattleUI::Attack() {
	m_attackState = AttackState::TARGET_MOVING;
	m_targetX = m_battleBox.left;
	m_targetY = m_battleBox.bottom + m_battleBox.height / 2 - m_pTargetChoice[0]->GetHeight() / 2;
	m_attackTimer = 0.f;
	m_flashFrame = 0;
	m_strikeFrame = 0;
	m_hit = 0;
	m_targetScale = 1.f;
	if (m_pMissText) {
		delete m_pMissText;
		m_pMissText = nullptr;
	}
}

void BattleUI::SansSpeech(std::vector<std::string> strings) {
	if (m_pSansTextBox) {
		delete m_pSansTextBox;
		m_pSansTextBox = nullptr;
	}
	m_pSansTextBox = new TextBox(TextBox::Font::SANS, Vector2f{ 425, m_pSans->GetLegPos().y + 105.f }, 180, strings, 0, Color4f{ 0, 0, 0, 1 });
	m_sansTextTime = 1;
}

void BattleUI::SansSpeech(std::string string) {
	if (m_pSansTextBox) {
		delete m_pSansTextBox;
		m_pSansTextBox = nullptr;
	}
	m_pSansTextBox = new TextBox(TextBox::Font::SANS, Vector2f{ 425, m_pSans->GetLegPos().y + 105.f }, 180, string, 0, Color4f{ 0, 0, 0, 1 });
	m_sansTextTime = 1;
}

void BattleUI::TakeDamage(bool isMenuBone) {
	if (!(isMenuBone && m_currentPattern >= 23)) {
		m_damageSound.Play(0);
	}

	if (isMenuBone) {
		if (m_HP == 1 && m_currentPattern >= 23) {
			return;
		}
		else if (m_HP >= 0) {
			m_HP--;
			UpdateHPText();
		}
		else if (m_karmaHP >= 1 && m_HP > 0) {
			m_karmaHP--;
			UpdateHPText();
		}
		return;
	}

	if (!m_isDamaging) {
		m_isDamaging = 1;
	}

	constexpr int minHP{ 1 };

	if (m_HP > minHP) {
		m_HP -= m_damage;
		if (m_damage > m_HP) {
			m_HP = minHP;
		}
		m_karmaHP += m_damage;
		UpdateHPText();
	}
	else if (m_karmaHP >= minHP) {
		m_karmaHP--;
		UpdateHPText();
	}
	else {
		m_HP--;
	}

	constexpr int minDamage{ 1 };
	constexpr int damageStep{ 3 };

	if (m_damage > minDamage) {
		m_damage -= damageStep;
	}
	else {
		m_damage = minDamage;
	}
}

Rectf BattleUI::GetBattleBounds() const {
	return Rectf{ m_battleBox.left + m_battleBoxLineWidth, m_battleBox.bottom + m_battleBoxLineWidth,
		m_battleBox.width - m_battleBoxLineWidth * 2, m_battleBox.height - m_battleBoxLineWidth * 2 };
}

void BattleUI::ChangeBattleBox(float width, float height, float duration, float rightOffset, bool finalAttack) {
	if (duration <= 0) {
		width -= m_battleBoxLineWidth;
		height -= m_battleBoxLineWidth;
		m_battleBox = Rectf{ 320.f - width / 2.f, 105.f, width + rightOffset, height };
		m_currentAnimationTime = 0.f;
		m_isAnimating = 0;
		return;
	}

	width -= m_battleBoxLineWidth;
	height -= m_battleBoxLineWidth;
	m_targetBattleBox = Rectf{ 320.f - width / 2.f, (finalAttack) ? 105.f + (m_battleBox.height - height) / 2.f : 105.f, width + rightOffset, height};

	m_animationDuration = duration;
	m_currentAnimationTime = 0.f;
	m_isAnimating = 1;
}

void BattleUI::UpdateBattleBox(float elapsedSec) {
	m_currentAnimationTime += elapsedSec;

	float t = m_currentAnimationTime / m_animationDuration;

	if (t >= 1.f) {
		t = 1.f;
		m_isAnimating = 0;
	}

	m_battleBox.left = Lerp(m_battleBox.left, m_targetBattleBox.left, t);
	m_battleBox.bottom = Lerp(m_battleBox.bottom, m_targetBattleBox.bottom, t);
	m_battleBox.width = Lerp(m_battleBox.width, m_targetBattleBox.width, t);
	m_battleBox.height = Lerp(m_battleBox.height, m_targetBattleBox.height, t);
}

void BattleUI::UpdateHPText() {
	if (m_pHPtext) {
		delete m_pHPtext;
		m_pHPtext = nullptr;
	}

	Color4f color;

	if (m_karmaHP > 0) {
		color = Color4f{ 1, 0, 1, 1 };
	}
	else {
		color = Color4f{ 1, 1, 1, 1 };
	}

	m_pHPtext = new Texture(std::to_string(m_HP + m_karmaHP) + " / " + std::to_string(m_maxHP), m_pUI_Font, color);
}

void BattleUI::UpdateHPbar() {
	m_accumulatedFrames += 1;

	int threshold{ 30 };
	if (m_karmaHP >= 10 && m_karmaHP < 20) threshold = 15;
	else if (m_karmaHP >= 20 && m_karmaHP < 30) threshold = 10;
	else if (m_karmaHP >= 30) threshold = 5;

	if (m_accumulatedFrames >= threshold) {
		m_accumulatedFrames = 0;
		m_karmaHP--;
		UpdateHPText();
	}

	if (m_karmaHP <= 0) {
		m_damage = 7;
	}
}

void BattleUI::SetInMenu(bool newInMenu, Soul* soul)
{
	if (newInMenu) {
		SetButtonSelection(0);
		ChangeBattleBox(515, 130, 0.5f);
		soul->SetMode(0);
		soul->DisableInput(1);
		soul->SetBoundSnapping(0);
		soul->Teleport(Vector2f{ 80.f, 36.f });
		m_menuCursorSound.Play(0);
	}
	else {
		m_isTeleported = 0;
		SetButtonSelection(-1);
		soul->DisableInput(0);
		soul->SetBoundSnapping(1);
		soul->Teleport(Vector2f{ GetBattleBounds().left + GetBattleBounds().width / 2.f, GetBattleBounds().bottom + GetBattleBounds().height / 2.f });

	}
}

void BattleUI::SetReset(bool shouldReset) 
{ 
	m_shouldReset = shouldReset;
	if (m_shouldReset == 0) {
		m_HP = m_maxHP; UpdateHPText(); 
		m_currentPattern = 0; } 
	if (IsInMenu()) { 
		SetInMenu(0, m_pSoul); 
	} 
}

float BattleUI::Lerp(float a, float b, float t) {
	return a + t * (b - a);
}