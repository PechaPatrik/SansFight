#pragma once
#include "Texture.h"
#include "TextBox.h"
#include "utils.h"
#include "SoundEffect.h"

class Corridor
{
public:
    Corridor(const Rectf& viewport);
    ~Corridor();

    Corridor(const Corridor& other) = delete;
    Corridor& operator=(const Corridor& other) = delete;
    Corridor(Corridor&& other) = delete;
    Corridor& operator=(Corridor&& other) = delete;

    void Draw() const;
    void Update(float elapsedSec);
    void OnKeyDown(const SDL_KeyboardEvent& e);
    bool DidFightStart() const { return m_fightStarted; }

private:
    Texture* m_pCorridorTexture;
    Texture* m_pFrisk;
    Texture* m_pSansShadow;
    Texture* m_pSansFace;
    Texture* m_pPillar;
    Texture* m_pSoul;
    SoundEffect m_soulFlash{"Audio/Flash.ogg"};
    SoundEffect m_soulMove{"Audio/BattleStart.wav"};
    TextBox* m_pTextBox;
    Rectf m_viewport;
    Vector2f m_friskPos{};
    float m_shadowX{};
    float m_animTimer{ 0.f };
    int m_animFrame{ 0 };
    int m_lastDirection{ 0 };
    const float m_cameraSpeed{ 180.f };
    const float m_animSpeed{ 0.1f };
    float m_corridorX{ 0.f };
    bool m_drawShadow{ 0 };
    bool m_disableInput{ 0 };
    bool m_textTime{ 0 };
    bool m_cutsceneActive{ 0 };
    bool m_fightStarted{ 0 };

    enum class CutsceneState {
        IDLE,
        WAIT_BEFORE_MOVE,
        MOVING_CAMERA,
        WAIT_AFTER_MOVE,
        SHOWING_TEXT,
        SOUL_APPEARING,
        SOUL_MOVING,
        COMPLETE
    };

    CutsceneState m_cutsceneState{ CutsceneState::IDLE };
    float m_cutsceneTimer{ 0.f };
    bool m_soulVisible{ 0 };
    Vector2f m_soulPos{};
    float m_targetCorridorX{ 0.f };

    void DrawCorridor() const;
    void DrawFrisk() const;
    void DrawPillars() const;
    void DrawTextBox() const;
    void DrawSoul() const;
    void UpdateCutscene(float elapsedSec);
    void StartCutscene();
};