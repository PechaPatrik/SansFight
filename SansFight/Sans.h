#pragma once
#include "utils.h"
#include "Texture.h"
#include "BattleUI.h"

class Sans {
public:
    Sans(const Rectf& viewport);
    ~Sans();

    Sans(const Sans& other) = delete;
    Sans& operator=(const Sans& other) = delete;
    Sans(Sans&& other) = delete;
    Sans& operator=(Sans&& other) = delete;

    void Draw(BattleUI* battleUI) const;
    void Update(float elapsedSec, const std::string& bodyAnimation = "idle", bool blueEye = 0);
	void ChangeTexture(const std::string& bodyPart, const std::string& type);
    void Animate(const std::string& bodyPart, const std::string& baseName, int frameCount, bool isInfinite = 0, bool allowRepeat = 0);
    void Move(const Vector2f& direction);
    void ResetSans(bool changeTextures = 1);
    Vector2f GetLegPos() const { return m_legsPos; }

private:
    Texture* m_pHeadTexture{ nullptr };
    Texture* m_pBodyTexture{ nullptr };
    Texture* m_pLegsTexture{ nullptr };
    std::vector<Texture*> m_pSweatTextures{};

    const Vector2f m_baseLegsPos;
    const Vector2f m_baseBodyPos;
    const Vector2f m_baseHeadPos;

    Vector2f m_legsPos{};
    Vector2f m_bodyPos{};
    Vector2f m_headPos{};

    float m_totalMovedX;
	int m_headMovement{ 0 };
	Vector2f m_bodyMovement{ 0.f, 1.f };

    int m_accumulatedFrames{ 0 };
    bool m_isIdle{ 0 };
    bool m_noAnimation{ 0 };
    int m_idleIndex{ 0 };

    struct AnimationState {
        int currentFrame = 0;
        std::string currentAnim;
        bool isPlaying = false;
    };

    AnimationState m_headAnimation;
    AnimationState m_bodyAnimation;

    bool SafeTextureChange(Texture*& existingTex, const std::string& path);
};

