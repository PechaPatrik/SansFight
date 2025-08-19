#pragma once
#include "Weapon.h"
#include "Texture.h"
#include "SoundEffect.h"

class Bone : public Weapon
{
public:
    enum class Type { NORMAL, BLUE, MENU, SPREAD };

    Bone(const Vector2f& pos, const Rectf& battleBox, Type type, const Vector2f& direction, const Vector2f& size = {}, float speed = 70.f);
    ~Bone() override;

    Bone(const Bone& other) = delete;
    Bone& operator=(const Bone& other) = delete;
    Bone(Bone&& other) = delete;
    Bone& operator=(Bone&& other) = delete;

    void Draw() const override;
    Rectf GetHitbox() const override;
    void Update(float elapsedSec) override;
    bool GetBlue() const;
    bool IsMenuBone() const { return m_type == Type::MENU; };

private:
    Type m_type;
    Vector2f m_direction;
    Vector2f m_size;
    Texture* m_pTexture;
    SoundEffect m_warningSound{"Audio/Warning.ogg"};
    SoundEffect m_stabSound{ "Audio/BoneStab.ogg" };
    bool m_hasPlayedStabSound{ false };

    const float m_speed{};
    bool m_IsRetracting{ false };
    Vector2f m_InitialSize;
    float m_MaxSize{ 0.f };
    float m_warningTimer{ 1.f };
    enum class MenuMovementState {
        FIRST_MOVE,
        SECOND_MOVE,
        FINAL_MOVE
    };
    MenuMovementState m_MenuMoveState{ MenuMovementState::FIRST_MOVE };
    float m_MenuMoveProgress{ 0.f };
    const float m_MenuFirstMoveDistance{};
    const float m_MenuSecondMoveDistance{ 100.f };

    Rectf m_BattleBound;
    Rectf m_Hitbox;
};