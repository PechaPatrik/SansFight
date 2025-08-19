#pragma once
#include "utils.h"
#include "SoundEffect.h"
#include "Texture.h"
#include "TextBox.h"
#include "Soul.h"

class Sans;
class AttackManager;

class BattleUI
{
public:
    BattleUI(Soul* soul, Sans* sans, AttackManager* attackManager);
    ~BattleUI();

    BattleUI(const BattleUI& other) = delete;
    BattleUI& operator=(const BattleUI& other) = delete;
    BattleUI(BattleUI&& other) = delete;
    BattleUI& operator=(BattleUI&& other) = delete;

    void Draw() const;
    void Update(float elapsedSec);
    void SetButtonSelection(int index);
    void TakeDamage(bool isMenuBone = 0);
    void ResetDamage() { if (m_isDamaging) { m_damage = 7; m_isDamaging = 0; } }
    bool ShouldReset() const { return m_shouldReset; }
    void SetReset(bool shouldReset);
    Rectf GetBattleBounds() const;
    void ChangeBattleBox(float width, float height, float duration = 0.f, float rightOffset = 0.f, bool finalAttack = 0);
    void SansSpeech(std::vector<std::string> strings);
    void SansSpeech(std::string string);
    void OnKeyPress(const SDL_KeyboardEvent& e);
    void SetInMenu(bool newInMenu, Soul* soul);
    bool IsInMenu() const { return m_selectedIndex >= 0; }
    bool IsSansSpeaking() const { return m_sansTextTime; }
    void SetCurrentPattern(size_t newPattern) { m_currentPattern = newPattern; }
    size_t GetCurrentPattern() const { return m_currentPattern; }
    bool IsEndScreen() const { return m_isEndScreen; }
    void SetAttackMenu(bool newInAttackMenu) { m_selectingAttack = newInAttackMenu; }
    bool IsAttackMenu() { return m_selectingAttack; }
private:
    const Vector2f m_HPBarStartPos{ Vector2f{274.f, 75.f} };
    const float m_HPBarHeight{ 18.f };
    Soul* m_pSoul;
    Sans* m_pSans;
    float m_sansTimer{ 0.f };
    bool m_isSansMoving{ 0 };

    AttackManager* m_pAttackManager;
    size_t m_currentPattern{ 0 };
    size_t m_extraRounds{ 0 };

    std::vector<Texture*> m_pButtons;
    std::vector<Texture*> m_pSelectedButtons;
    Texture* m_pHPtexture;
    Texture* m_pKRtexture;
    TTF_Font* m_pUI_Font;
    TTF_Font* m_pDamageFont;
    Texture* m_pHPtext;
    Texture* m_pNameAndLevel;
    Texture* m_pTarget;
    std::vector<Texture*> m_pTargetChoice;
    std::vector<Texture*> m_pStrike;
    TTF_Font* m_pEndScreenFont;
    Texture* m_pEndScreenText;
    Texture* m_pRestartPrompt;
    bool m_isEndScreen{ 0 };
    bool m_shouldReset{ 0 };
    Texture* m_pAttackMenuTexture;
    Texture* m_pAttackMenuHelpTexture;
    bool m_selectingAttack{ 0 };
    size_t m_menuSelectedAttack{ 0 };

    Texture* m_pSpeechBubble;
    TextBox* m_pSansTextBox;
    TextBox* m_pDefaultTextBox;
    bool m_sansTextTime{ 0 };
    int m_selectedIndex;
    bool m_isTeleported{ 0 };
    bool m_textNeedsUpdate{ 1 };
    bool m_attackTriggered{ 0 };
    std::string m_currentButtonText;
    bool m_shouldSkipTextAnimation{ 0 };
    float m_textUpdateCooldown{ 0.f };
    const float m_textUpdateDelay{ 0.1f };
    void DrawButtons() const;

    SoundEffect m_damageSound{ "Audio/PlayerDamaged.ogg" };
    SoundEffect m_menuCursorSound{ "Audio/MenuCursor.ogg" };
    SoundEffect m_menuSelectSound{ "Audio/MenuSelect.ogg" };
    SoundEffect m_hitSound{ "Audio/PlayerFight.ogg" };
    SoundEffect m_healSound{ "Audio/Heal.wav" };

    Rectf m_battleBox{};
    float m_battleBoxLineWidth{};
    Rectf m_targetBattleBox;
    float m_animationDuration;
    float m_currentAnimationTime;
    bool m_isAnimating;
    void UpdateBattleBox(float elapsedSec);
    static float Lerp(float a, float b, float t);

    const int m_maxHP{ 92 };
    int m_HP;
    int m_karmaHP{};
    int m_damage{ 7 };
    bool m_isDamaging{ 0 };
    int m_accumulatedFrames{};
    void DrawHPBar() const;
    void UpdateHPbar();
    void UpdateHPText();
    void DrawSansSpeech() const;
    void Attack();
    void UpdateAttack(float elapsedSec);

    enum class AttackState {
        INACTIVE,
        TARGET_MOVING,
        TARGET_FLASHING,
        SHOWING_RESULT,
        SHRINKING_TARGET
    };
    AttackState m_attackState{ AttackState::INACTIVE };
    float m_targetX{ 0.f };
    float m_targetY{ 0.f };
    float m_attackTimer{ 0.f };
    int m_flashFrame{ 0 };
    int m_strikeFrame{ 0 };
    bool m_hit{ 0 };
    Texture* m_pMissText{ nullptr };
    float m_resultY{ 0.f };
    float m_resultVelocity{ 0.f };
    float m_targetScale{ 1.f };
};