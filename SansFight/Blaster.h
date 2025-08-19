#pragma once
#include "Weapon.h"
#include "Texture.h"
#include "SoundEffect.h"
#include <vector>

class Blaster : public Weapon
{
public:
    Blaster(const Vector2f& pos, const Vector2f& size, int swooshAmount, float angle, bool instant = 0);
    ~Blaster() override;

    Blaster(const Blaster& other) = delete;
    Blaster& operator=(const Blaster& other) = delete;
    Blaster(Blaster&& other) = delete;
    Blaster& operator=(Blaster&& other) = delete;

    void Draw() const override;
    void Update(float elapsedSec) override;
    Rectf GetHitbox() const;
    std::vector<Vector2f> GetHitboxLines() const;

private:
    enum class State {
        ENTERING,
        IDLE,
        SHOOTING,
        EXITING
    };

    void LoadTextures();
    void UpdatePosition(float elapsedSec);
    void UpdateShooting(float elapsedSec);
    void DrawBeam() const;

    SoundEffect m_spawnSound{ "Audio/GasterBlaster.ogg" };
    SoundEffect m_shootSound1{ "Audio/GasterBlast.ogg" };
    SoundEffect m_shootSound2{ "Audio/GasterBlast2.ogg" };
    std::vector<Texture*> m_pTextures;
    Texture* m_pCurrentTexture;
    Rectf m_Hitbox;
    Vector2f m_Size;
    int m_SwooshAmount;
    int m_CurrentSwoosh;
    float m_AnimationTimer;
    int m_CurrentFrame;
    State m_State;
    float m_Angle;
    float m_StartAngle;
    float m_TargetAngle;
    Vector2f m_StartPos;
    Vector2f m_TargetPos;
    Vector2f m_Direction;
    float m_Speed;
    float m_ShootDelay;
    float m_ShootTimer;
    float m_BeamWidth;
    bool m_IsBeamActive;
};