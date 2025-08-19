#pragma once
#include "Game.h"
#include <vector>
#include <functional>

class AttackManager {
    friend class Game;
public:
    AttackManager(Game* game);

    void Update(float elapsedSec);
    void StartPattern(size_t patternIndex);
    bool IsAttackInProgress() const { return m_isAttackInProgress; }
    size_t GetCurrentPattern() const { return m_currentPattern; }
private:
    SoundEffect m_flashSound{ "Audio/Flash.ogg" };

    Game* m_pGame;
    std::vector<std::vector<std::pair<std::function<void()>, float>>> m_attackPatterns;
    std::vector<bool> m_completedPatterns;
    size_t m_currentPattern;
    size_t m_currentStep;
    int m_stepFrames;
    bool m_isAttackInProgress;

    Platform* m_pMainPlatform = nullptr;
    int m_previousRandom{ -1 };
    float m_finalAttackX{};

    void AddStep(size_t patternIndex, std::function<void()> action, int delay = 0);
    void EndPattern(size_t patternIndex);

    void Slam(size_t patternIndex, std::string direction, int delay = 30, float speed = 100.f, float height = 50.f);
    void Blackout(size_t patternIndex, int duration);
    void AddSansSpeech(size_t patternIndex, const std::string text, int delay = 0);
    void AddSansSpeech(size_t patternIndex, const std::vector<std::string> strings, int delay = 0);
    void SoulToMiddle(size_t patternIndex);
    void SoulToBottomMiddle(size_t patternIndex);
    void SpawnBoneGap(size_t patternIndex, float gapSize = 20.f, int delay = 0, float gapBottomY = 0.f, float speed = 200.f, int spawnSide = 0);
    float m_savedGapBottomY{ 0.f };
    void StartBoneJump(size_t patternIndex);
};
