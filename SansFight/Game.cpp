#include "pch.h"
#include "Game.h"
#include <iostream>

Game::Game(const Window& window)
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	GetViewPort();
	m_pSoul = new Soul();
	m_pSans = new Sans(GetViewPort());
	m_pCorridor = new Corridor(GetViewPort());
	m_pAttackManager = new AttackManager(this);
	m_pBattleUI = new BattleUI(m_pSoul, m_pSans, m_pAttackManager);
}

void Game::Cleanup()
{
	if (m_pSoul) {
		delete m_pSoul;
		m_pSoul = nullptr;
	}

	if (m_pSans) {
		delete m_pSans;
		m_pSans = nullptr;
	}

	if (m_pCorridor) {
		delete m_pCorridor;
		m_pCorridor = nullptr;
	}

	if (m_pAttackManager) {
		delete m_pAttackManager;
		m_pAttackManager = nullptr;
	}

	if (m_pBattleUI) {
		delete m_pBattleUI;
		m_pBattleUI = nullptr;
	}

	for (int i{ 0 }; i < m_pWeapons.size(); i++) {
		if (m_pWeapons[i]) {
			delete m_pWeapons[i];
			m_pWeapons[i] = nullptr;
		}
	}
	m_pWeapons.clear();
}

void Game::Update(float elapsedSec) {
	if (m_pBattleUI->IsEndScreen() || m_pBattleUI->IsAttackMenu()) {
		if (m_megalovania.IsPlaying()) {
			m_megalovania.Pause();
		}
		return;
	}

	if (m_pBattleUI->ShouldReset()) {
		m_pBattleUI->SetReset(0);
		if (m_pCorridor) {
			delete m_pCorridor;
			m_pCorridor = nullptr;
		}
		m_pCorridor = new Corridor(GetViewPort());
		m_megalovania.Stop();
		m_corridor = 1;
		for (int i{ 0 }; i < m_pWeapons.size(); i++) {
			if (m_pWeapons[i]) {
				delete m_pWeapons[i];
				m_pWeapons[i] = nullptr;
			}
		}
		m_pWeapons.clear();
		m_pAttackManager->m_completedPatterns.assign(m_pAttackManager->m_attackPatterns.size(), false);
		m_pAttackManager->m_isAttackInProgress = 0;
		return;
	}

	constexpr float TargetFPS{ 1.f / 30.f };
	m_AccumulatedElapsedSec += elapsedSec;

	while (m_AccumulatedElapsedSec >= TargetFPS) {
		if (m_corridor) {
			m_pCorridor->Update(TargetFPS * 0.5f);
			if (m_pCorridor->DidFightStart()) {
				delete m_pCorridor;
				m_pCorridor = nullptr;
				m_corridor = 0;
				m_pAttackManager->StartPattern(0);
			}
		}
		SpawnMenuBones();
		HandleDamage();
		m_pBattleUI->Update(TargetFPS);
		m_pSoul->Move(TargetFPS, m_pBattleUI->GetBattleBounds(), GetPlatformsFromWeapons(m_pWeapons));
		m_pSans->Update(TargetFPS, m_CurrentAnimation, m_blueEye);
		if (!m_pBattleUI->IsSansSpeaking() && m_pAttackManager->IsAttackInProgress() && m_pAttackManager) {
			m_pAttackManager->Update(TargetFPS * 0.5f);
		}

		for (auto it = m_pWeapons.begin(); it != m_pWeapons.end(); )
		{
			(*it)->Update(TargetFPS);

			if ((*it) && (*it)->ShouldDestroy())
			{
				delete* it;
				it = m_pWeapons.erase(it);
			}
			else
			{
				++it;
			}
		}

		m_AccumulatedElapsedSec -= TargetFPS;
	}
}

std::vector<Platform*> Game::GetPlatformsFromWeapons(const std::vector<Weapon*>& weapons)
{
	std::vector<Platform*> platforms;

	for (Weapon* weapon : weapons) {
		Platform* platform = dynamic_cast<Platform*>(weapon);
		if (platform != nullptr) {
			platforms.push_back(platform);
		}
	}

	return platforms;
}

void Game::Draw() const {
	ClearBackground();
	if (m_blackout) {
		return;
	}
	if (m_pBattleUI->IsEndScreen() || m_pBattleUI->IsAttackMenu()) {
		m_pBattleUI->Draw();
		return;
	}
	if (m_corridor) {
		m_pCorridor->Draw();
		return;
	}
	m_pSans->Draw(m_pBattleUI);
	m_pBattleUI->Draw();
	m_pSoul->Draw();
	for (Weapon* pWeapon : m_pWeapons) {
		pWeapon->Draw();
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e) {

	ToggleFullscreen(e);

	if (!m_pAttackManager->IsAttackInProgress() && e.keysym.sym == SDLK_m) {
		m_pBattleUI->SetAttackMenu(!m_pBattleUI->IsAttackMenu());
	}

	if (m_pBattleUI->IsEndScreen() || m_pBattleUI->IsAttackMenu()) {
		if (m_pBattleUI->IsAttackMenu() && (e.keysym.sym == SDLK_RETURN || e.keysym.sym == SDLK_KP_ENTER || e.keysym.sym == SDLK_z)) {
			m_corridor = 0;
			m_megalovania.Resume();
		}
		m_pBattleUI->OnKeyPress(e);
		return;
	}

	if (m_corridor) {
		m_pCorridor->OnKeyDown(e);
		return;
	}

	if (m_pAttackManager->IsAttackInProgress() && !m_pBattleUI->IsSansSpeaking()) {
		return;
	}

	if (m_pBattleUI->IsInMenu() || m_pBattleUI->IsSansSpeaking()) {
		m_pBattleUI->OnKeyPress(e);
		return;
	}

	m_pSoul->OnKeyDown(e);
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e) {
	m_pSoul->OnKeyUp(e);
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{

}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{

}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{

}

void Game::HandleDamage()
{
	bool isOverlappingAnyWeapon{ false };

	// Get soul's hitbox and calculate its four corners
	Rectf soulHitbox = m_pSoul->GetHitbox();
	Vector2f soulCorners[4] = {
		Vector2f(soulHitbox.left, soulHitbox.bottom),
		Vector2f(soulHitbox.left + soulHitbox.width, soulHitbox.bottom),
		Vector2f(soulHitbox.left, soulHitbox.bottom + soulHitbox.height),
		Vector2f(soulHitbox.left + soulHitbox.width, soulHitbox.bottom + soulHitbox.height)
	};

	for (Weapon* weapon : m_pWeapons) {
		Blaster* pBlaster = dynamic_cast<Blaster*>(weapon);
		if (pBlaster) {
			auto lines = pBlaster->GetHitboxLines();
			if (lines.size() >= 4) {
				bool cornerBetweenLines = false;

				for (const auto& corner : soulCorners) {
					if (IsPointBetweenLines(corner, lines[0], lines[1], lines[2], lines[3])) {
						cornerBetweenLines = 1;
						break;
					}
				}

				if (cornerBetweenLines) {
					m_pBattleUI->TakeDamage();
					isOverlappingAnyWeapon = 1;
					continue;
				}
			}
		}

		if (weapon != nullptr && !utils::IsOverlapping(soulHitbox, weapon->GetHitbox())) {
			continue;
		}

		isOverlappingAnyWeapon = 1;

		Bone* pBone = dynamic_cast<Bone*>(weapon);
		if (pBone) {
			if (!pBone->GetBlue() || !m_pSoul->GetVelocity().Equals({})) {
				m_pBattleUI->TakeDamage(pBone->IsMenuBone());
				break;
			}
		}
	}

	if (!isOverlappingAnyWeapon) {
		m_pBattleUI->ResetDamage();
	}
}

bool Game::IsPointBetweenLines(const Vector2f& point, const Vector2f& line1Start, const Vector2f& line1End, const Vector2f& line2Start, const Vector2f& line2End)
{
	Vector2f line1Dir = line1End - line1Start;
	Vector2f line2Dir = line2End - line2Start;
	Vector2f pointVec1 = point - line1Start;
	Vector2f pointVec2 = point - line2Start;

	float cross1 = line1Dir.x * pointVec1.y - line1Dir.y * pointVec1.x;
	float cross2 = line2Dir.x * pointVec2.y - line2Dir.y * pointVec2.x;

	return (cross1 * cross2) < 0;
}

void Game::SpawnMenuBones()
{
	if (m_pAttackManager->IsAttackInProgress() && m_boneSpawnFrames > 0) {
		m_boneSpawnFrames = 0;
	}
	else if (m_pBattleUI->GetCurrentPattern() > 13 && !m_pAttackManager->IsAttackInProgress()) {
		m_boneSpawnFrames++;

		if (m_boneSpawnFrames == 1) {
			if (m_pBattleUI->GetCurrentPattern() > 13 && m_pBattleUI->GetCurrentPattern() != 16 && m_pBattleUI->GetCurrentPattern() != 17) {
				m_pWeapons.push_back(new Bone(Vector2f{ 0.f, 180.f }, GetViewPort(), Bone::Type::MENU, Vector2f{ -1, 0 }, Vector2f{ 100.f, 0.f }, 200.f));
			}
			if (m_pBattleUI->GetCurrentPattern() > 15) {
				m_pWeapons.push_back(new Bone(Vector2f{ 160.f, -40.f }, GetViewPort(), Bone::Type::MENU, Vector2f{ 0, -1 }, Vector2f{ 0.f, 50.f }, 200.f));
				m_pWeapons.push_back(new Bone(Vector2f{ 430.f, -40.f }, GetViewPort(), Bone::Type::MENU, Vector2f{ 0, -1 }, Vector2f{ 0.f, 50.f }, 200.f));
			}
		}
		if (m_boneSpawnFrames == 25 && m_pBattleUI->GetCurrentPattern() > 15) {
			m_pWeapons.push_back(new Bone(Vector2f{ 295.f, -40.f }, GetViewPort(), Bone::Type::MENU, Vector2f{ 0, -1 }, Vector2f{ 0.f, 50.f }, 200.f));
			m_pWeapons.push_back(new Bone(Vector2f{ 565.f, -40.f }, GetViewPort(), Bone::Type::MENU, Vector2f{ 0, -1 }, Vector2f{ 0.f, 50.f }, 200.f));
		}
		if (m_boneSpawnFrames >= 50) {
			m_boneSpawnFrames = 0;
		}
	}
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// for fullscreen testing to see everything more clearly
void Game::ToggleFullscreen(const SDL_KeyboardEvent& e) {
	if (e.keysym.sym == SDLK_F4)
	{
		m_IsFullscreen = !m_IsFullscreen;
		SDL_Window* window = SDL_GetWindowFromID(1);
		SDL_SetWindowFullscreen(window, m_IsFullscreen ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

		int windowW{}, windowH{};
		SDL_GetWindowSize(window, &windowW, &windowH);

		float targetAspect{ GetViewPort().width / GetViewPort().height };
		float windowAspect{ static_cast<float>(windowW) / windowH };

		int viewportW{}, viewportH{};
		int viewportX{ 0 }, viewportY{ 0 };

		if (windowAspect > targetAspect) {
			// if window is wider than 4:3 add vertical bars
			viewportH = windowH;
			viewportW = int(viewportH * targetAspect);
			viewportX = (windowW - viewportW) / 2;
		}
		else {
			// if window is taller than 4:3 add horizontal bars
			viewportW = windowW;
			viewportH = int(viewportW / targetAspect);
			viewportY = (windowH - viewportH) / 2;
		}

		glViewport(viewportX, viewportY, viewportW, viewportH);
	}
}
