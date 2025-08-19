#pragma once
#include "BaseGame.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include "Soul.h"
#include "BattleUI.h"
#include "Weapon.h"
#include "Corridor.h"
#include "Texture.h"
#include "utils.h"
#include "Sans.h"
#include "Bone.h"
#include "Blaster.h"
#include "Platform.h"
#include "AttackManager.h"

class Game : public BaseGame {
	friend class AttackManager;
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	Soul* m_pSoul{ nullptr };
	BattleUI* m_pBattleUI{ nullptr };
	Sans* m_pSans{ nullptr };
	Corridor* m_pCorridor{ nullptr };
	std::vector<Weapon*> m_pWeapons{};
	AttackManager* m_pAttackManager;

	std::string m_CurrentAnimation{ "idle" };
	bool m_blueEye{ 0 };
	SoundStream m_birdsChirp{ "Audio/BirdNoise.ogg" };
	SoundStream m_megalovania{ "Audio/Megalovania.ogg" };

	float m_AccumulatedElapsedSec{ 0.f };
	int m_boneSpawnFrames{ 0 };
	bool m_IsFullscreen{ 0 };
	bool m_corridor{ 1 };
	bool m_blackout{ 0 };

	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;

	std::vector<Platform*> GetPlatformsFromWeapons(const std::vector<Weapon*>& weapons);

	void HandleDamage();
	void SpawnMenuBones();
	bool IsPointBetweenLines(const Vector2f& point, const Vector2f& line1Start, const Vector2f& line1End, const Vector2f& line2Start, const Vector2f& line2End);
	void ToggleFullscreen(const SDL_KeyboardEvent& e);
};