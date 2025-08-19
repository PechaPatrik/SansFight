#include "pch.h"
#include "Blaster.h"
#include <cmath>
#include <algorithm>

Blaster::Blaster(const Vector2f& pos, const Vector2f& size, int swooshAmount, float angle, bool instant)
    : Weapon(pos),
    m_Size(size),
    m_SwooshAmount(swooshAmount),
    m_CurrentSwoosh(0),
    m_AnimationTimer(0),
    m_CurrentFrame(0),
    m_State(State::ENTERING),
    m_Speed(700),
    m_ShootDelay(0.5f),
    m_ShootTimer(0),
    m_BeamWidth(0),
    m_IsBeamActive(false)
{
    m_Direction = Vector2f(cos(angle * float(M_PI) / 180), sin(angle * float(M_PI) / 180));
    m_StartPos = pos - m_Direction * 500.0f;
    m_TargetPos = pos;
    m_StartAngle = angle + 180;
    m_TargetAngle = angle;
    m_Angle = m_StartAngle;
    m_Hitbox = Rectf{ m_StartPos.x, m_StartPos.y, size.x, size.y };
    LoadTextures();
    m_pCurrentTexture = m_pTextures[0];
    m_spawnSound.Play();

    if (instant) {
        m_State = State::IDLE;
        m_ShootDelay = 1.f;
        m_Hitbox.left = m_TargetPos.x;
        m_Hitbox.bottom = m_TargetPos.y;
        m_Angle = m_TargetAngle;
    }
}

Blaster::~Blaster()
{
    for (auto& texture : m_pTextures)
    {
        if (texture)
        {
            delete texture;
            texture = nullptr;
        }
    }
    m_pTextures.clear();
}

void Blaster::LoadTextures()
{
    m_pTextures.push_back(new Texture("Weapons/blaster_idle.png"));
    for (int i = 0; i < 5; ++i)
    {
        m_pTextures.push_back(new Texture("Weapons/blaster_shoot" + std::to_string(i) + ".png"));
    }
}

void Blaster::Draw() const
{
    if (m_IsBeamActive)
    {
        DrawBeam();
    }

    glPushMatrix();
    glTranslatef(m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2, 0);
    glRotatef(m_Angle, 0, 0, 1);
    glTranslatef(-m_Hitbox.width / 2, -m_Hitbox.height / 2, 0);
    m_pCurrentTexture->Draw(Rectf{ 0, 0, m_Hitbox.width, m_Hitbox.height });
    glPopMatrix();
}

void Blaster::DrawBeam() const
{
    float halfWidth = m_BeamWidth / 2;
    Vector2f center(m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2);
    Vector2f frontCenter = center + m_Direction * (m_Hitbox.width / 2);
    Vector2f normal(-sin(m_Angle * float(M_PI) / 180), cos(m_Angle * float(M_PI) / 180));
    Vector2f beamEdge1 = frontCenter + normal * halfWidth;
    Vector2f beamEdge2 = frontCenter - normal * halfWidth;
    float beamLength = 1000.0f;

    float maxOpacityWidth = m_Size.y * 0.7f;
    float opacity = std::min(m_BeamWidth / maxOpacityWidth, 1.0f);

    glColor4f(1.f, 1.f, 1.f, opacity);
    glBegin(GL_QUADS);
    glVertex2f(beamEdge1.x, beamEdge1.y);
    glVertex2f(beamEdge2.x, beamEdge2.y);
    glVertex2f(beamEdge2.x + m_Direction.x * beamLength, beamEdge2.y + m_Direction.y * beamLength);
    glVertex2f(beamEdge1.x + m_Direction.x * beamLength, beamEdge1.y + m_Direction.y * beamLength);
    glEnd();
}

std::vector<Vector2f> Blaster::GetHitboxLines() const
{
    if (!m_IsBeamActive) return {};

    float thirdWidth = m_BeamWidth / 3.f;
    Vector2f center(m_Hitbox.left + m_Hitbox.width / 2, m_Hitbox.bottom + m_Hitbox.height / 2);
    Vector2f normal(-sin(m_Angle * float(M_PI) / 180), cos(m_Angle * float(M_PI) / 180));

    Vector2f line1Start = center + normal * thirdWidth;
    Vector2f line2Start = center - normal * thirdWidth;

    return {
        line1Start - m_Direction * 2000,
        line1Start + m_Direction * 2000,
        line2Start - m_Direction * 2000,
        line2Start + m_Direction * 2000
    };
}

void Blaster::Update(float elapsedSec)
{
    switch (m_State)
    {
    case State::ENTERING:
        UpdatePosition(elapsedSec);
        if (Vector2f(m_Hitbox.left, m_Hitbox.bottom).Equals(m_TargetPos, 0.1f))
        {
            m_State = State::IDLE;
            m_ShootTimer = 0;
            m_Hitbox.left = m_TargetPos.x;
            m_Hitbox.bottom = m_TargetPos.y;
            m_Angle = m_TargetAngle;
        }
        break;

    case State::IDLE:
        m_ShootTimer += elapsedSec;
        if (m_ShootTimer >= m_ShootDelay)
        {
            m_State = State::SHOOTING;
            m_shootSound1.Play();
            m_shootSound2.Play();
            m_CurrentSwoosh = 0;
            m_AnimationTimer = 0;
            m_CurrentFrame = 1;
            m_pCurrentTexture = m_pTextures[m_CurrentFrame];
        }
        break;

    case State::SHOOTING:
        UpdateShooting(elapsedSec);
        break;

    case State::EXITING:
        UpdatePosition(elapsedSec);
        if ((Vector2f(m_Hitbox.left, m_Hitbox.bottom) - m_StartPos).Length() < 5)
        {
            m_ShouldDestroy = true;
        }
        break;
    }
}

void Blaster::UpdatePosition(float elapsedSec)
{
    Vector2f currentPos(m_Hitbox.left, m_Hitbox.bottom);
    Vector2f targetPos = (m_State == State::ENTERING) ? m_TargetPos : m_StartPos;

    Vector2f remaining = targetPos - currentPos;
    float distance = remaining.Length();

    float moveDistance = m_Speed * elapsedSec;

    if (distance <= moveDistance)
    {
        m_Hitbox.left = targetPos.x;
        m_Hitbox.bottom = targetPos.y;
    }
    else
    {
        Vector2f direction = remaining.Normalized();
        m_Hitbox.left += direction.x * moveDistance;
        m_Hitbox.bottom += direction.y * moveDistance;

        if (m_State == State::ENTERING) {
            float normalizedTarget = fmod(m_TargetAngle + 180, 360) - 180;

            bool clockwise = (normalizedTarget >= -45 && normalizedTarget < 45);
            float progress = moveDistance / distance;

            if (clockwise) {
                m_Angle = m_StartAngle + progress * (m_TargetAngle - m_StartAngle);
            }
            else {
                m_Angle = m_StartAngle - progress * (m_TargetAngle - m_StartAngle);
            }

            m_Angle = fmod(m_Angle + 360, 360);
        }
    }
}

void Blaster::UpdateShooting(float elapsedSec)
{
    m_AnimationTimer += elapsedSec;

    if (m_CurrentSwoosh == 0) {
        if (m_AnimationTimer < 0.06f) m_CurrentFrame = 1;
        else if (m_AnimationTimer < 0.12f) m_CurrentFrame = 2;
        else if (m_AnimationTimer < 0.18f) m_CurrentFrame = 3;
        else if (m_AnimationTimer < 0.24f) m_CurrentFrame = 4;
        else m_CurrentFrame = 5;
    }
    else {
        m_CurrentFrame = (fmod(m_AnimationTimer, 0.12f) < 0.06f ? 4 : 5);
    }

    m_pCurrentTexture = m_pTextures[m_CurrentFrame];

    if (m_CurrentFrame >= 4) {
        float swooshProgress = (m_AnimationTimer + (0.3f * m_CurrentSwoosh)) / (0.3f * m_SwooshAmount);
        float phase = swooshProgress * m_SwooshAmount * M_PI;

        if (m_CurrentSwoosh == m_SwooshAmount) {
            m_BeamWidth = m_Size.y * (1.f - m_AnimationTimer / 0.3f);
        }
        else if (m_CurrentSwoosh == 0) {
            m_BeamWidth = m_Size.y * 0.5f * (1.0f - cosf(phase));
        }
        else {
            m_BeamWidth = m_Size.y * (0.75f + 0.25f * cosf(phase * 2.0f));
        }

        m_IsBeamActive = 1;
    }
    else {
        m_BeamWidth = 0.0f;
        m_IsBeamActive = 0;

    }

    if (m_AnimationTimer >= 0.3f) {
        m_CurrentSwoosh++;
        m_AnimationTimer = 0;
        if (m_CurrentSwoosh > m_SwooshAmount) {
            m_State = State::EXITING;
            m_Speed *= 1.5f;
            m_BeamWidth = 0.0f;
            m_IsBeamActive = 0;
        }
    }
}

Rectf Blaster::GetHitbox() const
{
    return m_Hitbox;
}