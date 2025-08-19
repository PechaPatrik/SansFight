#include "pch.h"
#include "AttackManager.h"
#include <iostream>

AttackManager::AttackManager(Game* game)
    : m_pGame(game), m_currentPattern(-1), m_currentStep(-1), m_stepFrames(-1), m_isAttackInProgress(0)
{
    size_t currentAttack{ 0 };

    // Intro attack (0)
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155, 0.5f); });
    SoulToMiddle(0);
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Stop(); });
    AddStep(currentAttack, [this]() { m_pGame->m_birdsChirp.Play(1); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_CurrentAnimation = "none"; });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "ready?");
    AddStep(currentAttack, [this]() { m_pGame->m_birdsChirp.Stop(); });
    Blackout(currentAttack, 5);
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_blueEye = 1; });
    AddStep(currentAttack, [this]() { m_pGame->m_CurrentAnimation = "slamDown"; });
    Slam(currentAttack, "down", 40, 50.f, 45.f);
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
    AddStep(currentAttack, [this]() { m_pGame->m_blueEye = 0; });
    AddStep(currentAttack, [this]() { m_pGame->m_CurrentAnimation = "slamRight"; });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); }, 12);

    float baseGapY = 70.f;
    float amplitude = 25.f;
    float cycles = 2.f;

    for (int i = 0; i < 20; i++) {
        float progress = float(i) / 19.f;
        float angle = progress * cycles * float(M_PI);
        float currentBottomGapY = baseGapY - amplitude * sin(angle);

        SpawnBoneGap(currentAttack, 35.f, (i < 19) ? -1 : 25, currentBottomGapY, 350.f, -1);

        if (i == 16) {
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
            AddStep(currentAttack, [this]() { m_pGame->m_CurrentAnimation = "none"; });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 150, 200 }, Vector2f{ 90, 70 }, 2, 0)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 220, 270 }, Vector2f{ 90, 70 }, 2, -90)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 400, 100 }, Vector2f{ 90, 70 }, 2, 180)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 330, 20 }, Vector2f{ 90, 70 }, 2, 90)); });
        }
    }
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 160, 260 }, Vector2f{ 90, 70 }, 2, -45)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 390, 260 }, Vector2f{ 90, 70 }, 2, -135)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 160, 25 }, Vector2f{ 90, 70 }, 2, 45)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 390, 25 }, Vector2f{ 90, 70 }, 2, 135)); }, 30);
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 150, 200 }, Vector2f{ 90, 70 }, 2, 0)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 220, 270 }, Vector2f{ 90, 70 }, 2, -90)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 400, 100 }, Vector2f{ 90, 70 }, 2, 180)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 330, 20 }, Vector2f{ 90, 70 }, 2, 90)); }, 30);
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 50, 125 }, Vector2f{ 140, 110 }, 3, 0)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 410, 125 }, Vector2f{ 140, 110 }, 3, 180)); }, 120);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    AddSansSpeech(currentAttack, "here we go.");
    AddStep(currentAttack, [this]() { m_pGame->m_CurrentAnimation = "idle"; });
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Play(1); });
    EndPattern(currentAttack);

    //Small bones from sides 1 (1)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddSansSpeech(currentAttack, "what? you think i'm just gonna stand there and take it?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); }, 20);
    for (int i{}; i < 8; i++) {
        SpawnBoneGap(currentAttack, 20.f, (i < 7) ? 20 : 60, 20.f);
    }
    EndPattern(currentAttack);

    //Blue bone pattern (2)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddSansSpeech(currentAttack, std::vector<std::string>{ "our reports showed a massive anomaly in the timespace continuum.", "timelines jumping left and right, stopping and starting..."});
    for (int i{}; i < 6; i++) {
        if (i < 3) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 10.f,bounds.bottom },
                    bounds,
                    Bone::Type::BLUE,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 100.f },
                    250.f
                )); }, 7);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 10.f,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 20.f },
                    250.f
                )); }, (i < 2) ? 15 : 30);
        }
        else {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 20.f },
                    250.f
                )); }, 13);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left,bounds.bottom },
                    bounds,
                    Bone::Type::BLUE,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 100.f },
                    250.f
                )); }, (i < 5) ? 10 : 60);
        }

    }
    EndPattern(currentAttack);

    //Random bone gaps 1 (3)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "until suddenly, everything ends.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    int boneAmount{ rand() % 4 + 3 };
    for (int i{ boneAmount }; i >= 0; i--) {
        AddStep(currentAttack, [this, i, boneAmount]() {
            m_savedGapBottomY = 20.f + (rand() % 31);
            auto& currentStep = m_attackPatterns[m_currentPattern][m_currentStep];
            currentStep.second = (i < boneAmount) ? m_savedGapBottomY * 1.2f : m_savedGapBottomY * 0.4f;
            });

        SpawnBoneGap(currentAttack);
        if (i == 0) {
            AddStep(currentAttack, [this]() {}, m_savedGapBottomY * 20.f);
        }
    }
    EndPattern(currentAttack);

    //Parkour 1 (4)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "heh heh heh...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
    AddSansSpeech(currentAttack, "that's your fault, isn't it?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ 30.f, 140.f }, m_pGame->GetViewPort(), 60.f, 1, 150.f)); }, 10);
    for (int i{}; i < 41; i++) {
        if (i == 8) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ 0.f, 140.f }, m_pGame->GetViewPort(), 60.f, 1, 155.f)); });
        }
        if (i == 20) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ 0.f, 140.f }, m_pGame->GetViewPort(), 60.f, 1, 160.f)); });
        }
        if (i >= 30 && i <= 32) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left,bounds.bottom + bounds.height - 40.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 40.f },
                    170.f
                )); }, 0);
        }
        AddStep(currentAttack, [this]() {
            const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left,bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1.f, 0 },
                Vector2f{ 0, 30.f },
                150.f
            )); }, 2);
    }
    AddStep(currentAttack, [this]() {}, 30);
    AddStep(currentAttack, [this]() {
        const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
        m_pGame->m_pWeapons.push_back(new Bone(
            Vector2f{ bounds.left,bounds.bottom + bounds.height - 90.f },
            bounds,
            Bone::Type::NORMAL,
            Vector2f{ 1.f, 0 },
            Vector2f{ 0, 90.f },
            300.f
        )); }, 2);
    AddStep(currentAttack, [this]() {}, 50);
    EndPattern(currentAttack);

    //Parkour 2 (5)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, "you can't understand how this feels.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 140.f }, m_pGame->GetViewPort(), 50.f, -1, 160.f)); }, 15);
    for (int i{}; i < 58; i++) {
        if (i == 3) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 180.f }, m_pGame->GetViewPort(), 50.f, -1, 160.f)); });
        }
        else if (i == 7) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 140.f }, m_pGame->GetViewPort(), 50.f, -1, 160.f)); });
        }
        else if (i == 11) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 60.f },
                    160.f
                )); });
        }
        else if (i == 12) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 190.f }, m_pGame->GetViewPort(), 25.f, -1, 65.f)); });
        }
        else if (i == 17) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 165.f }, m_pGame->GetViewPort(), 50.f, -1, 160.f)); });
        }
        else if (i == 22) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 155.f }, m_pGame->GetViewPort(), 50.f, -1, 160.f)); });
        }
        else if (i == 26) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom + bounds.height - 30.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 30.f },
                    160.f
                )); });
        }
        else if (i == 27) {
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 5.f, 175.f }, m_pGame->GetViewPort(), 50.f, -1, 160.f)); });
        }
        else if (i == 31) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom + bounds.height - 40.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 40.f },
                    160.f
                )); });
        }
        else if (i == 36) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom + bounds.height - 25.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 25.f },
                    160.f
                )); });
        }
        else if (i == 51) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom + bounds.height - 95.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 95.f },
                    110.f
                )); });
        }
        else if (i == 57) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 95.f },
                    200.f
                )); });
        }
        AddStep(currentAttack, [this]() {
            const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ -1.f, 0 },
                Vector2f{ 0, 30.f },
                150.f
            )); }, 2);
    }
    AddStep(currentAttack, [this]() {}, 60);
    EndPattern(currentAttack);

    //Platform+bones from sides (6)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "knowing that one day, without any warning...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "tired2"); });
    AddSansSpeech(currentAttack, "it's all going to be reset.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 100.f, 140.f }, m_pGame->GetViewPort(), 100.f, -1, 150.f)); });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ -70.f, 180.f }, m_pGame->GetViewPort(), 140.f, 1, 150.f)); });
    for (int i{}; i < 16; i++) {
        int random{ rand() % 6 };
        switch (random) {
        case 0:
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 30.f },
                    150.f
                )); }, 15);
            break;
        case 1:
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 30.f },
                    150.f
                )); }, 15);
            break;
        case 2:
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom + 35.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 40.f },
                    150.f
                )); }, 15);
            break;
        case 3:
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left,bounds.bottom + 35.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 40.f },
                    150.f
                )); }, 15);
            break;
        case 4:
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f,bounds.bottom + 80.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 35.f },
                    150.f
                )); }, 15);
            break;
        case 5:
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left,bounds.bottom + 80.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 35.f },
                    150.f
                )); }, 15);
            break;
        }
        if ((i + 1) % 2 == 0) {
            AddStep(currentAttack, [this, i]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 100.f, 140.f }, m_pGame->GetViewPort(), 100.f, -1, 150.f)); });

        }
        if ((i + 1) % 3 == 0) {
            AddStep(currentAttack, [this, i]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ -70.f, 180.f }, m_pGame->GetViewPort(), 140.f, 1, 150.f)); });
        }
    }
    AddStep(currentAttack, [this]() {}, 10);
    EndPattern(currentAttack);

    //Vertical bone parkour 1 (7)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "tired2"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddSansSpeech(currentAttack, "look. i gave up trying to go back a long time ago.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(375, 150, 0.5f, 18); }, 15);
    AddStep(currentAttack, [this]() {
        const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
        float position = bounds.left - 5.f;

        while (position < bounds.left + bounds.width) {
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ position,bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1.f, 0 },
                Vector2f{ 0, 20.f },
                100.f
            ));
            position += 14.f;
        }
        });

    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->Teleport(Vector2f{ m_pGame->m_pBattleUI->GetBattleBounds().left + 35.f, 160.f }); });
    AddStep(currentAttack, [this]() {
        m_pMainPlatform = new Platform(Vector2f{ m_pGame->m_pBattleUI->GetBattleBounds().left + 20.f , 150.f }, m_pGame->GetViewPort(), 40.f, 1, 70.f);
        m_pGame->m_pWeapons.push_back(m_pMainPlatform); }, 3);
    for (int i{}; i < 62; i++) {

        if (i % 5 == 0) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + 160.f, bounds.bottom - 30.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 0, 1.f },
                    Vector2f{ 0, 40.f },
                    150.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + 235.f, bounds.bottom + bounds.height - 10.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 0, -1.f },
                    Vector2f{ 0, 40.f },
                    150.f
                ));
                });
        }

        if (i % 8 == 0) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + 300.f, bounds.bottom - 30.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 0, 1.f },
                    Vector2f{ 0, 40.f },
                    100.f
                ));
                });
        }

        AddStep(currentAttack, [this]() {
            const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left - 5.f,bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1.f, 0 },
                Vector2f{ 0, 20.f },
                100.f
            ));

            if (m_pMainPlatform) {
                const Rectf& hitbox = m_pMainPlatform->GetHitbox();
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();

                if ((hitbox.left + hitbox.width >= bounds.left + bounds.width)) {
                    m_pMainPlatform->SetDirection(-1);
                }
            }

            }, 3);
    }
    EndPattern(currentAttack);

    //Platforms+blasters from sides (8)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "And getting to the surface doesn't really appeal anymore, either.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    for (int i{}; i < 30; i++) {
        AddStep(currentAttack, [this]() {}, 10);

        if (i % 3 == 0) {

            int random;

            do {
                random = rand() % 6;
            } while (random == m_previousRandom);

            m_previousRandom = random;

            switch (random) {
            case 0:
                AddStep(currentAttack, [this]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Blaster(
                        Vector2f{ bounds.left - 100.f,bounds.bottom },
                        Vector2f{ 100.f, 30.f },
                        2,
                        0
                    ));
                    });
                break;
            case 1:
                AddStep(currentAttack, [this]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Blaster(
                        Vector2f{ bounds.left + bounds.width,bounds.bottom },
                        Vector2f{ 100.f, 30.f },
                        2,
                        180
                    ));
                    });
                break;
            case 2:
                AddStep(currentAttack, [this]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Blaster(
                        Vector2f{ bounds.left - 100.f,bounds.bottom + 40.f },
                        Vector2f{ 100.f, 30.f },
                        2,
                        0
                    ));
                    });
                break;
            case 3:
                AddStep(currentAttack, [this]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Blaster(
                        Vector2f{ bounds.left + bounds.width,bounds.bottom + 40.f },
                        Vector2f{ 100.f, 30.f },
                        2,
                        180
                    ));
                    });
                break;
            case 4:
                AddStep(currentAttack, [this]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Blaster(
                        Vector2f{ bounds.left - 100.f,bounds.bottom + 80.f },
                        Vector2f{ 100.f, 30.f },
                        2,
                        0
                    ));
                    });
                break;
            case 5:
                AddStep(currentAttack, [this]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Blaster(
                        Vector2f{ bounds.left + bounds.width,bounds.bottom + 80.f },
                        Vector2f{ 100.f, 30.f },
                        2,
                        180
                    ));
                    });
                break;
            }
            AddStep(currentAttack, [this, i]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ m_pGame->GetViewPort().width - 100.f, 140.f }, m_pGame->GetViewPort(), 40.f, -1, 100.f)); });

        }
        if (i % 4 == 0) {
            AddStep(currentAttack, [this, i]() { m_pGame->m_pWeapons.push_back(new Platform(Vector2f{ -70.f, 180.f }, m_pGame->GetViewPort(), 40.f, 1, 100.f)); });
        }
    }
    AddStep(currentAttack, [this]() {}, 30);
    EndPattern(currentAttack);

    //Vertical bone parkour 2 (9)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "cause even if we do...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
    AddSansSpeech(currentAttack, "we'll just end up right back here, without any memory of it, right?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(375, 150, 0.5f, 18); }, 15);
    AddStep(currentAttack, [this]() {
        const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
        float position = bounds.left - 5.f;

        while (position < bounds.left + bounds.width) {
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ position,bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1.f, 0 },
                Vector2f{ 0, 20.f },
                100.f
            ));
            position += 14.f;
        }
        });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->Teleport(Vector2f{ m_pGame->m_pBattleUI->GetBattleBounds().left + 35.f, 160.f }); });
    AddStep(currentAttack, [this]() {
        m_pMainPlatform = new Platform(Vector2f{ m_pGame->m_pBattleUI->GetBattleBounds().left + 20.f , 150.f }, m_pGame->GetViewPort(), 40.f, 1, 70.f);
        m_pGame->m_pWeapons.push_back(m_pMainPlatform); }, 3);
    for (int i{}; i < 62; i++) {

        if (i % 5 == 0) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + 235.f, bounds.bottom + bounds.height - 10.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 0, -1.f },
                    Vector2f{ 0, 40.f },
                    130.f
                ));
                });
        }

        if (i % 4 == 0) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + 300.f, bounds.bottom - 30.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 0, 1.f },
                    Vector2f{ 0, 40.f },
                    130.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + 160.f, bounds.bottom - 30.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 0, 1.f },
                    Vector2f{ 0, 40.f },
                    130.f
                ));
                });
        }

        AddStep(currentAttack, [this]() {
            const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left - 5.f,bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1.f, 0 },
                Vector2f{ 0, 20.f },
                100.f
            ));

            if (m_pMainPlatform) {
                const Rectf& hitbox = m_pMainPlatform->GetHitbox();
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();

                if ((hitbox.left + hitbox.width >= bounds.left + bounds.width)) {
                    m_pMainPlatform->SetDirection(-1);
                }
            }

            }, 3);
    }
    EndPattern(currentAttack);

    //Small bones from sides 2 (10)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, "to be blunt...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "it makes it kind of hard to give it my all.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); }, 20);
    for (int i{}; i < 8; i++) {
        SpawnBoneGap(currentAttack, 20.f, (i < 7) ? 20 : 60, 20.f);
    }
    EndPattern(currentAttack);

    //Alternating bones (11)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, "... or is that just a poor excuse for being lazy...?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "hell if i know.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); }, 20);
    for (int i{}; i < 8; i++) {
        AddStep(currentAttack, [this]() {
            const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left, bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1.f, 0 },
                Vector2f{ 0, 20.f },
                100.f
            ));
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom + 20.f },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ -1.f, 0 },
                Vector2f{ 0, bounds.height - 20.f },
                100.f
            ));
            }, (i < 7) ? 20 : 60);
    }
    EndPattern(currentAttack);

    //Random bone gaps 2 (12)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "all i know is... seeing what comes next...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "tired2"); });
    AddSansSpeech(currentAttack, "i can't afford not to care anymore.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    boneAmount = rand() % 4 + 3;
    for (int i{ boneAmount }; i >= 0; i--) {
        AddStep(currentAttack, [this, i, boneAmount]() {
            m_savedGapBottomY = 20.f + (rand() % 31);
            auto& currentStep = m_attackPatterns[m_currentPattern][m_currentStep];
            currentStep.second = (i < boneAmount) ? m_savedGapBottomY * 1.2f : m_savedGapBottomY * 0.4f;
            });

        SpawnBoneGap(currentAttack);
        if (i == 0) {
            AddStep(currentAttack, [this]() {}, m_savedGapBottomY * 20.f);
        }
    }
    EndPattern(currentAttack);

    //Preparing for second phase (13)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "tired2"); });
    AddSansSpeech(currentAttack, "ugh... that being said.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, "you, uh, really like swinging that thing around, huh?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "let's make a deal.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    AddSansSpeech(currentAttack, "you have this round to heal up and prepare without me attacking.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
    AddSansSpeech(currentAttack, "but after this i will not be holding back anymore.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "deal?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    EndPattern(currentAttack);

    //Flash attack 1 (14)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddSansSpeech(currentAttack, "it's time for the real deal.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
    Blackout(currentAttack, 10);
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{0, -20.f}); });
    for (int i{}; i < 5; i++) {
        
        AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 100.f + static_cast<float>(rand() % 400 - m_pGame->m_pSans->GetLegPos().x), 0}); });

        int random{ rand() % 5 };
        int direction{ rand() % 2 };

        switch (random)
        {
        case 0:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(260, 110); });
            SoulToBottomMiddle(currentAttack);
            for (int i{ 2 }; i >= 0; i--) {
                AddStep(currentAttack, [this, i]() {
                    m_savedGapBottomY = 20.f + (rand() % 16);
                    auto& currentStep = m_attackPatterns[m_currentPattern][m_currentStep];
                    currentStep.second = (i < 2) ? m_savedGapBottomY * 1.5f : m_savedGapBottomY * 0.5f;
                    });

                SpawnBoneGap(currentAttack);
                if (i == 0) {
                    AddStep(currentAttack, [this]() {}, 20);
                }
            }
            break;
        case 1:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(260, 110); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() {}, 10);
            for (int i{}; i < 3; i++) {
                SpawnBoneGap(currentAttack, 20.f, (i < 2) ? 20 : 5, 20.f);
            }
            break;
        case 2:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() {}, 7);
            for (int i{}; i < 5; i++) {
                AddStep(currentAttack, [this, i]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ bounds.left, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ 1.f, 0 },
                        Vector2f{ 0, (i < 4) ? 40.f : 90.f },
                        200.f
                    ));
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ -1.f, 0 },
                        Vector2f{ 0, (i < 4) ? 40.f : 90.f },
                        200.f
                    ));
                    }, -1);
            }
            AddStep(currentAttack, [this]() {}, 15);
            break;
        case 3:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            AddStep(currentAttack, [this, direction]() { m_pGame->m_pSoul->Teleport(Vector2f{
                (direction == 0) ? 
                m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width :
                m_pGame->m_pBattleUI->GetBattleBounds().left,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pSoul->GetHitbox().height }); });
            for (int i{}; i < 22; i++) {
                AddStep(currentAttack, [this, i, direction]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left : bounds.left + bounds.width - 5.f, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                        Vector2f{ 0, (i < 11) ? 45.f : 15.f },
                        300.f
                    ));
                    }, -1);
            }
            AddStep(currentAttack, [this]() {}, 5);
            break;
        case 4:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom },
                    bounds,
                    Bone::Type::BLUE,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::BLUE,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                }, 10);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 20.f },
                    200.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 20.f },
                    200.f
                ));
                }, 3);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                }, 19);
            break;
        default:
            break;
        }

        AddStep(currentAttack, [this]() {
            for (int i{ 0 }; i < m_pGame->m_pWeapons.size(); i++) {
                delete m_pGame->m_pWeapons[i];
                m_pGame->m_pWeapons[i] = nullptr;
            }
            m_pGame->m_pWeapons.clear();
            });

        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
        Blackout(currentAttack, 10);
        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });
    }
    EndPattern(currentAttack);

    //Small blaster arena (15)
    currentAttack += 1;
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 60.f }); });
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(360, 190, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "sounds strange, but before all this i was secretly hoping we could be friends.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, std::vector<std::string>{"i always thought the anomaly was doing this cause they were unhappy.", "and when they got what they wanted, they would stop all this."});
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    for (int i{}; i < 15; i++) {
        AddStep(currentAttack, [this]() {
            Rectf soulHitbox = m_pGame->m_pSoul->GetHitbox();
            Vector2f soulCenter{
                soulHitbox.left + soulHitbox.width / 2.f,
                soulHitbox.bottom + soulHitbox.height / 2.f
            };

            const float blasterWidth = 100.f;
            const float blasterHeight = 30.f;

            Vector2f blasterPos;
            Vector2f blasterCenter;
            float distance;
            int attempts = 0;
            const int maxAttempts = 100; // Prevent infinite loops

            do {
                blasterPos.x = 50.f + static_cast<float>(rand() % 450);
                blasterPos.y = 20.f + static_cast<float>(rand() % 280);

                blasterCenter = Vector2f{
                    blasterPos.x + blasterWidth / 2.f,
                    blasterPos.y + blasterHeight / 2.f
                };

                Vector2f diff = soulCenter - blasterCenter;
                distance = sqrtf(diff.x * diff.x + diff.y * diff.y);

                attempts++;
                if (attempts >= maxAttempts) {
                    // Fallback position if we can't find a valid spot
                    blasterPos = Vector2f{ 50.f, 300.f };
                    blasterCenter = Vector2f{
                        blasterPos.x + blasterWidth / 2.f,
                        blasterPos.y + blasterHeight / 2.f
                    };
                    break;
                }
            } while (distance < 100.f); // Keep trying until it's at least 100 pixels away from the current soul position

            Vector2f direction = soulCenter - blasterCenter;
            float angle = atan2f(direction.y, direction.x) * 180.f / static_cast<float>(M_PI);

            m_pGame->m_pWeapons.push_back(new Blaster(
                blasterPos,
                Vector2f{ blasterWidth, blasterHeight },
                1,
                angle
            ));
            }, 17);
    }
    EndPattern(currentAttack);

    //Flash attack 2 (16)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "and maybe all they needed was... i dunno.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddSansSpeech(currentAttack, "some good food, some bad laughs, some nice friends.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
    Blackout(currentAttack, 10);
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });
    for (int i{}; i < 5; i++) {

        AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 100.f + static_cast<float>(rand() % 400 - m_pGame->m_pSans->GetLegPos().x), 0 }); });

        int random{ rand() % 5 };
        int direction{ rand() % 2 };

        switch (random)
        {
        
        case 0:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(190, 155); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y}); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
            AddStep(currentAttack, [this, direction]() { m_pGame->m_pSoul->Teleport(Vector2f{
                (direction == 0) ? m_pGame->m_pBattleUI->GetBattleBounds().left + 24.f : m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width - 40.f,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height / 2.f }); });
            AddStep(currentAttack, [this]() {}, 5);
            cycles = 1.5f;
            for (int i = 0; i < 16; i++) {
                float progress = float(i) / 15.f;
                float angle = progress * cycles * float(M_PI);
                float currentBottomGapY = baseGapY + amplitude * sin(angle);

                SpawnBoneGap(currentAttack, 35.f, (i < 15) ? -1 : 15, currentBottomGapY, 300.f, (direction == 0) ? 1 : -1);
            }
            break;
        case 1:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->Teleport(Vector2f{
                m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width / 2.f,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom + 80.f }); });
            AddStep(currentAttack, [this, direction]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                float positionX = bounds.left - 5.f;

                while (positionX < bounds.left + bounds.width) {
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ positionX,bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ 1.f, 0 },
                        Vector2f{ 0, 20.f },
                        0.f
                    ));
                    positionX += 14.f;
                }

                m_pGame->m_pWeapons.push_back(new Platform(
                    Vector2f{ 320.f, bounds.bottom + 30.f },
                    m_pGame->GetViewPort(),
                    40.f
                ));
                m_pGame->m_pWeapons.push_back(new Platform(
                    Vector2f{ 320.f, bounds.bottom + 65.f },
                    m_pGame->GetViewPort(),
                    40.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ (direction == 0) ? bounds.left : bounds.left + bounds.width - 5.f,bounds.bottom + 70.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                    Vector2f{ 0, 40.f },
                    180.f
                ));
                }, 25);
            AddStep(currentAttack, [this, direction]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ (direction == 0) ? bounds.left + bounds.width - 5.f : bounds.left,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ (direction == 0) ? -1.f : 1.f, 0 },
                    Vector2f{ 0, 65.f },
                    250.f
                ));
                }, 40);
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetOnPlatform(0); });
            break;
        case 2:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
            SoulToMiddle(currentAttack);
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 160, 260 }, Vector2f{ 90, 70 }, 2, -45, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 390, 260 }, Vector2f{ 90, 70 }, 2, -135, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 160, 25 }, Vector2f{ 90, 70 }, 2, 45, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 390, 25 }, Vector2f{ 90, 70 }, 2, 135, 1)); }, 50);
            break;
        case 3:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
            SoulToMiddle(currentAttack);
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 150, 145 }, Vector2f{ 90, 70 }, 2, 0, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 275, 270 }, Vector2f{ 90, 70 }, 2, -90, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 400, 145 }, Vector2f{ 90, 70 }, 2, 180, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 275, 20 }, Vector2f{ 90, 70 }, 2, 90, 1)); }, 50);
            break;
        case 4:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 130); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this, direction]() { m_pGame->m_pSoul->Teleport(Vector2f{
                (direction == 1) ? m_pGame->m_pBattleUI->GetBattleBounds().left + 24.f : m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width - 40.f,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom }); });
            for (int i{}; i < 2; i++) {
                float currentXchange = 80.f * (i + 1);
                AddStep(currentAttack, [this, direction, currentXchange]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left + currentXchange : bounds.left + bounds.width - 5.f - currentXchange, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                        Vector2f{ 0, 20.f },
                        120.f
                    ));
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left + bounds.width - 5.f - currentXchange : bounds.left + currentXchange, bounds.bottom + 20.f },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? -1.f : 1.f, 0 },
                        Vector2f{ 0, bounds.height - 20.f },
                        120.f
                    ));
                    });
            }
            for (int i{}; i < 3; i++) {
                AddStep(currentAttack, [this, direction]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left : bounds.left + bounds.width - 5.f, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                        Vector2f{ 0, 20.f },
                        120.f
                    ));
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left + bounds.width - 5.f : bounds.left, bounds.bottom + 20.f },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? -1.f : 1.f, 0 },
                        Vector2f{ 0, bounds.height - 20.f },
                        120.f
                    ));
                    }, (i < 2) ? 20 : 10);
            }
            break;
        default:
            break;
        }

        AddStep(currentAttack, [this]() {
            for (int i{ 0 }; i < m_pGame->m_pWeapons.size(); i++) {
                delete m_pGame->m_pWeapons[i];
                m_pGame->m_pWeapons[i] = nullptr;
            }
            m_pGame->m_pWeapons.clear();
            });

        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
        Blackout(currentAttack, 10);
        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });
    }
    EndPattern(currentAttack);

    //Slam attack 1 (17)
    currentAttack += 1;
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "but that's ridiculous, right?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
    AddSansSpeech(currentAttack, "yeah, you're the type of person who won't EVER be happy.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    for (int i{}; i < 8; i++) {
        const std::string directions[] = {
                    "up",
                    "down",
                    "left",
                    "right",
        };
        Slam(currentAttack, directions[rand() % (sizeof(directions) / sizeof(directions[0]))], 40, 50.f, 25.f);
    }
    EndPattern(currentAttack);

    //Slam attack 2 (18)
    currentAttack += 1;
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "noEyes"); });
    AddSansSpeech(currentAttack, "you'll keep consuming timelines over and over, until...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "well.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("body", "shrug"); });
    AddSansSpeech(currentAttack, "hey.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, std::vector<std::string>{"take it from me, kid.", "someday...", "you gotta learn when to QUIT."});
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() {}, 15);
    for (int i{}; i < 8; i++) {
        const std::string directions[] = {
                    "up",
                    "down",
                    "left",
                    "right",
        };
        Slam(currentAttack, directions[rand() % (sizeof(directions) / sizeof(directions[0]))], 30, 70.f, 25.f);
    }
    EndPattern(currentAttack);

    //Big blaster arena (19)
    currentAttack += 1;
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(360, 190, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 60.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "and that day's TODAY.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 60.f }); });
    for (int i{}; i < 15; i++) {
        AddStep(currentAttack, [this]() {
            Rectf soulHitbox = m_pGame->m_pSoul->GetHitbox();
            Vector2f soulCenter{
                soulHitbox.left + soulHitbox.width / 2.f,
                soulHitbox.bottom + soulHitbox.height / 2.f
            };

            const float blasterWidth = 100.f;
            const float blasterHeight = 70.f;

            Vector2f blasterPos;
            Vector2f blasterCenter;
            float distance;
            int attempts = 0;
            const int maxAttempts = 100; // Prevent infinite loops

            do {
                blasterPos.x = 50.f + static_cast<float>(rand() % 450);
                blasterPos.y = 20.f + static_cast<float>(rand() % 280);

                blasterCenter = Vector2f{
                    blasterPos.x + blasterWidth / 2.f,
                    blasterPos.y + blasterHeight / 2.f
                };

                Vector2f diff = soulCenter - blasterCenter;
                distance = sqrtf(diff.x * diff.x + diff.y * diff.y);

                attempts++;
                if (attempts >= maxAttempts) {
                    // Fallback position if we can't find a valid spot
                    blasterPos = Vector2f{ 50.f, 300.f };
                    blasterCenter = Vector2f{
                        blasterPos.x + blasterWidth / 2.f,
                        blasterPos.y + blasterHeight / 2.f
                    };
                    break;
                }
            } while (distance < 100.f); // Keep trying until it's at least 100 pixels away from the current soul position

            Vector2f direction = soulCenter - blasterCenter;
            float angle = atan2f(direction.y, direction.x) * 180.f / static_cast<float>(M_PI);

            m_pGame->m_pWeapons.push_back(new Blaster(
                blasterPos,
                Vector2f{ blasterWidth, blasterHeight },
                1,
                angle
            ));
            }, 17);
    }
    EndPattern(currentAttack);

    //Long horizontal bones (20)
    currentAttack += 1;
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "cause... y'see..");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, "all this fighting is really tiring me out.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    for (int i{}; i < 15; i++) {
        if (i % 2 == 0) {
            AddStep(currentAttack, [this]() {
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ 120.f, m_pGame->GetViewPort().height - 5.f },
                    m_pGame->GetViewPort(),
                    Bone::Type::NORMAL,
                    Vector2f{ 0, -1.f },
                    Vector2f{ 200.f, 0.f },
                    250.f
                ));
                }, 8);
        }
        else {
            AddStep(currentAttack, [this]() {
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ 320.f, 0 },
                    m_pGame->GetViewPort(),
                    Bone::Type::NORMAL,
                    Vector2f{ 0, 1.f },
                    Vector2f{ 200.f, 0.f },
                    250.f
                ));
                }, 8);
        }
    }
    AddStep(currentAttack, [this]() {}, 60);
    EndPattern(currentAttack);

    //Flash attack 3 (21)
    currentAttack += 1;
    StartBoneJump(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "and if you keep pushing me...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "then i'll be forced to use my special attack.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
    Blackout(currentAttack, 5);
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });
    for (int i{}; i < 6; i++) {

        AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 100.f + static_cast<float>(rand() % 400 - m_pGame->m_pSans->GetLegPos().x), 0 }); });

        int random{ rand() % 10 };
        int direction{ rand() % 2 };

        switch (random)
        {
        case 0:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(260, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            SoulToBottomMiddle(currentAttack);
            for (int i{ 2 }; i >= 0; i--) {
                AddStep(currentAttack, [this, i]() {
                    m_savedGapBottomY = 20.f + (rand() % 16);
                    auto& currentStep = m_attackPatterns[m_currentPattern][m_currentStep];
                    currentStep.second = (i < 2) ? m_savedGapBottomY * 1.5f : m_savedGapBottomY * 0.5f;
                    });

                SpawnBoneGap(currentAttack);
                if (i == 0) {
                    AddStep(currentAttack, [this]() {}, 20);
                }
            }
            break;
        case 1:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(260, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() {}, 10);
            for (int i{}; i < 3; i++) {
                SpawnBoneGap(currentAttack, 20.f, (i < 2) ? 20 : 5, 20.f);
            }
            break;
        case 2:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() {}, 7);
            for (int i{}; i < 5; i++) {
                AddStep(currentAttack, [this, i]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ bounds.left, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ 1.f, 0 },
                        Vector2f{ 0, (i < 4) ? 40.f : 90.f },
                        200.f
                    ));
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ -1.f, 0 },
                        Vector2f{ 0, (i < 4) ? 40.f : 90.f },
                        200.f
                    ));
                    }, -1);
            }
            AddStep(currentAttack, [this]() {}, 15);
            break;
        case 3:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this, direction]() { m_pGame->m_pSoul->Teleport(Vector2f{
                (direction == 0) ?
                m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width :
                m_pGame->m_pBattleUI->GetBattleBounds().left,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pSoul->GetHitbox().height }); });
            for (int i{}; i < 22; i++) {
                AddStep(currentAttack, [this, i, direction]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left : bounds.left + bounds.width - 5.f, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                        Vector2f{ 0, (i < 11) ? 45.f : 15.f },
                        300.f
                    ));
                    }, -1);
            }
            AddStep(currentAttack, [this]() {}, 5);
            break;
        case 4:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom },
                    bounds,
                    Bone::Type::BLUE,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::BLUE,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                }, 10);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 20.f },
                    200.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 20.f },
                    200.f
                ));
                }, 3);
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ 1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ -1.f, 0 },
                    Vector2f{ 0, 90.f },
                    200.f
                ));
                }, 19);
            break;
        case 5:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(190, 155); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
            AddStep(currentAttack, [this, direction]() { m_pGame->m_pSoul->Teleport(Vector2f{
                (direction == 0) ? m_pGame->m_pBattleUI->GetBattleBounds().left + 24.f : m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width - 40.f,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height / 2.f }); });
            AddStep(currentAttack, [this]() {}, 5);
            cycles = 1.5f;
            for (int i = 0; i < 16; i++) {
                float progress = float(i) / 15.f;
                float angle = progress * cycles * float(M_PI);
                float currentBottomGapY = baseGapY + amplitude * sin(angle);

                SpawnBoneGap(currentAttack, 35.f, (i < 15) ? -1 : 15, currentBottomGapY, 300.f, (direction == 0) ? 1 : -1);
            }
            break;
        case 6:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 110); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->Teleport(Vector2f{
                m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width / 2.f,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom + 80.f }); });
            AddStep(currentAttack, [this, direction]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                float positionX = bounds.left - 5.f;

                while (positionX < bounds.left + bounds.width) {
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ positionX,bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ 1.f, 0 },
                        Vector2f{ 0, 20.f },
                        0.f
                    ));
                    positionX += 14.f;
                }

                m_pGame->m_pWeapons.push_back(new Platform(
                    Vector2f{ 320.f, bounds.bottom + 30.f },
                    m_pGame->GetViewPort(),
                    40.f
                ));
                m_pGame->m_pWeapons.push_back(new Platform(
                    Vector2f{ 320.f, bounds.bottom + 65.f },
                    m_pGame->GetViewPort(),
                    40.f
                ));
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ (direction == 0) ? bounds.left : bounds.left + bounds.width - 5.f,bounds.bottom + 70.f },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                    Vector2f{ 0, 40.f },
                    180.f
                ));
                }, 25);
            AddStep(currentAttack, [this, direction]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ (direction == 0) ? bounds.left + bounds.width - 5.f : bounds.left,bounds.bottom },
                    bounds,
                    Bone::Type::NORMAL,
                    Vector2f{ (direction == 0) ? -1.f : 1.f, 0 },
                    Vector2f{ 0, 65.f },
                    250.f
                ));
                }, 40);
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetOnPlatform(0); });
            break;
        case 7:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
            SoulToMiddle(currentAttack);
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 160, 260 }, Vector2f{ 90, 70 }, 2, -45, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 390, 260 }, Vector2f{ 90, 70 }, 2, -135, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 160, 25 }, Vector2f{ 90, 70 }, 2, 45, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 390, 25 }, Vector2f{ 90, 70 }, 2, 135, 1)); }, 50);
            break;
        case 8:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
            SoulToMiddle(currentAttack);
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 150, 145 }, Vector2f{ 90, 70 }, 2, 0, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 275, 270 }, Vector2f{ 90, 70 }, 2, -90, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 400, 145 }, Vector2f{ 90, 70 }, 2, 180, 1)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Blaster(Vector2f{ 275, 20 }, Vector2f{ 90, 70 }, 2, 90, 1)); }, 50);
            break;
        case 9:
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 130); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
            AddStep(currentAttack, [this, direction]() { m_pGame->m_pSoul->Teleport(Vector2f{
                (direction == 1) ? m_pGame->m_pBattleUI->GetBattleBounds().left + 24.f : m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width - 40.f,
                m_pGame->m_pBattleUI->GetBattleBounds().bottom }); });
            for (int i{}; i < 2; i++) {
                float currentXchange = 80.f * (i + 1);
                AddStep(currentAttack, [this, direction, currentXchange]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left + currentXchange : bounds.left + bounds.width - 5.f - currentXchange, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                        Vector2f{ 0, 20.f },
                        120.f
                    ));
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left + bounds.width - 5.f - currentXchange : bounds.left + currentXchange, bounds.bottom + 20.f },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? -1.f : 1.f, 0 },
                        Vector2f{ 0, bounds.height - 20.f },
                        120.f
                    ));
                    });
            }
            for (int i{}; i < 3; i++) {
                AddStep(currentAttack, [this, direction]() {
                    const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left : bounds.left + bounds.width - 5.f, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? 1.f : -1.f, 0 },
                        Vector2f{ 0, 20.f },
                        120.f
                    ));
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ (direction == 0) ? bounds.left + bounds.width - 5.f : bounds.left, bounds.bottom + 20.f },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ (direction == 0) ? -1.f : 1.f, 0 },
                        Vector2f{ 0, bounds.height - 20.f },
                        120.f
                    ));
                    }, (i < 2) ? 20 : 10);
            }
            break;
        default:
            break;
        }

        AddStep(currentAttack, [this]() {
            for (int i{ 0 }; i < m_pGame->m_pWeapons.size(); i++) {
                delete m_pGame->m_pWeapons[i];
                m_pGame->m_pWeapons[i] = nullptr;
            }
            m_pGame->m_pWeapons.clear();
            });

        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
        Blackout(currentAttack, 5);
        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });
    }
    EndPattern(currentAttack);

    //Slam attack 3 (22)
    currentAttack += 1;
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "yeah, my special attack. sound familiar?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "lookLeft"); });
    AddSansSpeech(currentAttack, "well, get ready. cause after the next move, i'm going to use it.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "so, if you don't wanna see it, now would be a good time to die.");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() {}, 15);
    for (int i{}; i < 8; i++) {
        const std::string directions[] = {
                    "up",
                    "down",
                    "left",
                    "right",
        };
        Slam(currentAttack, directions[rand() % (sizeof(directions) / sizeof(directions[0]))], 30, 60.f, 35.f);
    }
    EndPattern(currentAttack);

    //Final attack (23)
    currentAttack += 1;
    SoulToMiddle(currentAttack);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155, 0.5f); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "closedEyes"); });
    AddSansSpeech(currentAttack, "well, here goes nothing...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "are you ready?");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); }, 15);
    for (int i{}; i < 4; i++) {
        const std::string directions[] = {
                    "up",
                    "down",
                    "left",
                    "right",
        };
        Slam(currentAttack, directions[rand() % (sizeof(directions) / sizeof(directions[0]))], 30, 60.f, 35.f);
    }
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
    for (int i{}; i < 6; i++) {
        if (i % 2 == 0) {
            AddStep(currentAttack, [this]() {
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ 120.f, m_pGame->GetViewPort().height - 5.f },
                    m_pGame->GetViewPort(),
                    Bone::Type::NORMAL,
                    Vector2f{ 0, -1.f },
                    Vector2f{ 200.f, 0.f },
                    250.f
                ));
                }, 8);
        }
        else {
            AddStep(currentAttack, [this]() {
                m_pGame->m_pWeapons.push_back(new Bone(
                    Vector2f{ 320.f, 0 },
                    m_pGame->GetViewPort(),
                    Bone::Type::NORMAL,
                    Vector2f{ 0, 1.f },
                    Vector2f{ 200.f, 0.f },
                    250.f
                ));
                }, 8);
        }
    }
    AddStep(currentAttack, [this]() {}, 30);
    AddStep(currentAttack, [this]() {
        if (m_pGame->m_CurrentAnimation == "slamLeft") {
            m_pGame->m_pSans->Animate("body", "left", 5, 0, 1);;
        }
        else {
            m_pGame->m_CurrentAnimation = "slamLeft";
        }
        });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->Slam(Vector2f{ -1, 0 }); }, 15);
    AddStep(currentAttack, [this]() {
        if (m_pGame->m_CurrentAnimation == "slamRight") {
            m_pGame->m_pSans->Animate("body", "right", 5, 0, 1);
        }
        else {
            m_pGame->m_CurrentAnimation = "slamRight";
        }
        });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetGravityDirection(Vector2f{ 1, 0 }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(m_pGame->GetViewPort().width + 48.f, 110, 5.f, 0, 1); });
    AddStep(currentAttack, [this]() { 
        m_finalAttackX = m_pGame->m_pBattleUI->GetBattleBounds().left;
        m_pGame->m_pSoul->SetFinalAttackX(m_finalAttackX);
        });
    for (int i{}; i < 340; i++) {
        if (i >= 70 && i <= 160 && i % 2 == 0) {
            float baseGapY = 30.f;
            float amplitude = 20.f;
            float cycles = 7.f;
            float progress = (i - 71) / 2.f / 45.f;
            float angle = progress * cycles * float(M_PI);
            float currentBottomGapY = baseGapY - amplitude * sin(angle);
            SpawnBoneGap(currentAttack, 30.f, 0, currentBottomGapY, 1000.f, 1);
        }
        else if (i >= 170 && i <= 250 && i % 20 == 0) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                for (int i{}; i < 3; i++) {
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ bounds.left + bounds.width - 5.f - 15.f * i, bounds.bottom },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ -1.f, 0 },
                        Vector2f{ 0, bounds.height / 2.f },
                        1000.f
                    ));
                }
                });
        }
        else if (i >= 170 && i <= 250 && (i + 10) % 20 == 0) {
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                for (int i{}; i < 3; i++) {
                    m_pGame->m_pWeapons.push_back(new Bone(
                        Vector2f{ bounds.left + bounds.width - 5.f - 15.f * i, bounds.bottom + bounds.height / 2.f },
                        bounds,
                        Bone::Type::NORMAL,
                        Vector2f{ -1.f, 0 },
                        Vector2f{ 0, bounds.height / 2.f },
                        1000.f
                    ));
                }
                });
        }
        else if (i >= 260 && i <= 310 && i % 2 == 0) {
            float gapSize = 80.f - 2.f * (i - 260) / 2.f;
            SpawnBoneGap(currentAttack, gapSize, 0, (95.f - gapSize) / 2.f, 1000.f, 1);
        }
        AddStep(currentAttack, [this, i]() {
            if (m_pGame->m_pSans->GetLegPos().y !=
                m_pGame->m_pBattleUI->GetBattleBounds().bottom +
                m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) {
                    m_pGame->m_pSans->Move(Vector2f{ 0,
                    (m_pGame->m_pBattleUI->GetBattleBounds().bottom +
                    m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) -
                    m_pGame->m_pSans->GetLegPos().y });
            }

            if (i == 10) {
                m_pGame->m_pSans->ResetSans();
                m_pGame->m_CurrentAnimation = "idle";
            }
            if (i > 10) {
                m_pGame->m_pSans->Move(Vector2f{ -30.f, 0.f });
            }

            if (m_pGame->m_pSans->GetLegPos().x <= -300.f) {
                m_pGame->m_pSans->ResetSans();
                const std::string headOptions[] = {
                    "closedEyes",
                    "idle",
                    "lookLeft",
                    "noEyes",
                    "wink"
                };
                m_pGame->m_pSans->ChangeTexture("head", headOptions[rand() % (sizeof(headOptions) / sizeof(headOptions[0]))]);
                const std::string bodyOptions[] = {
                    "idle",
                    "shrug"
                };
                m_pGame->m_pSans->ChangeTexture("body", bodyOptions[rand() % (sizeof(bodyOptions) / sizeof(bodyOptions[0]))]);
                m_pGame->m_pSans->Move(Vector2f{ 400.f, -20.f });
            }

            if (i < 15) {
                m_finalAttackX += 8.f;
                m_pGame->m_pSoul->SetFinalAttackX(m_finalAttackX);
            } else if (i < 60) {
                m_finalAttackX -= 7.f;
                m_pGame->m_pSoul->SetFinalAttackX(m_finalAttackX);
            }
            else if (i == 330) {
                m_pGame->m_pBattleUI->ChangeBattleBox(m_pGame->GetViewPort().width + 48.f, 155, 2.f, -200.f);
            }
            }, 1);
    }
    AddStep(currentAttack, [this]() {
        m_finalAttackX = 0.f;
        m_pGame->m_pSoul->SetFinalAttackX(m_finalAttackX);
    });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->Slam(Vector2f{ 1, 0 }, 200.f); }, 15);
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
    AddStep(currentAttack, [this]() {
        if (m_pGame->m_CurrentAnimation == "slamRight") {
            m_pGame->m_pSans->Animate("body", "right", 5, 0, 1);
        }
        else {
            m_pGame->m_CurrentAnimation = "slamRight";
        }
        });
    AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 1, 0 }, Vector2f{ 40, 40 }, 50.f)); }, 40);
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155); });
    AddStep(currentAttack, [this]() { 
        m_pGame->m_CurrentAnimation = "idle";
        m_pGame->m_pSans->ResetSans();
        m_pGame->m_pSans->Move(Vector2f{ 0, (m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pBattleUI->GetBattleBounds().height + 20.f) - m_pGame->m_pSans->GetLegPos().y }); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(1); });
    for (int i{}; i < 4; i++) {
        
        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
        Blackout(currentAttack, 5);
        AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Resume(); });

        AddStep(currentAttack, [this]() {
            for (int i{ 0 }; i < m_pGame->m_pWeapons.size(); i++) {
                delete m_pGame->m_pWeapons[i];
                m_pGame->m_pWeapons[i] = nullptr;
            }
            m_pGame->m_pWeapons.clear();
            });
        if (i == 0) {
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetGravityDirection(Vector2f{ 0, -1 }); });
            SoulToBottomMiddle(currentAttack);
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 0, 1 }, Vector2f{ 40, 40 }, 50.f)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 0, -1 }, Vector2f{ 40, 40 }, 50.f)); }, 32);
        } else if (i == 1) {
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetGravityDirection(Vector2f{ 0, 1 }); });
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pSoul->Teleport(Vector2f{ bounds.left, bounds.bottom + bounds.height }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ -1, 0 }, Vector2f{ 40, 40 }, 50.f)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 0, 1 }, Vector2f{ 40, 40 }, 50.f)); }, 32);
        }
        else if (i == 2) {
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetGravityDirection(Vector2f{ 1, 0 }); });
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds(); 
                m_pGame->m_pSoul->Teleport(Vector2f{ bounds.left + bounds.width, bounds.bottom }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 1, 0 }, Vector2f{ 40, 40 }, 50.f)); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 0, -1 }, Vector2f{ 40, 40 }, 50.f)); }, 32);
        }
        else if (i == 3) {
            AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetGravityDirection(Vector2f{ -1, 0 }); });
            AddStep(currentAttack, [this]() {
                const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
                m_pGame->m_pSoul->Teleport(Vector2f{ bounds.left, bounds.bottom + bounds.height }); });
            AddStep(currentAttack, [this]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ -1, 0 }, Vector2f{ 40, 40 }, 50.f)); }, 25);
        }
    }
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(155, 155); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
    for (int i{}; i < 122; i++) {
        AddStep(currentAttack, [this, i]() {
            const Rectf& bounds{ m_pGame->m_pBattleUI->GetBattleBounds() };
            const Vector2f battleCenter{
                bounds.left + bounds.width / 2.f,
                bounds.bottom + bounds.height / 2.f
            };
            const Vector2f blasterSize{ 100.f, 30.f };
            const Vector2f halfBlasterSize{ blasterSize * 0.5f };
            const float distance{ 90.f };
            float angleStep{ 10.f + 5.f * i / 121.f };
            float currentAngle = fmod(angleStep * i, 360.f);

            Vector2f blasterPos{
            battleCenter.x + (distance + halfBlasterSize.x) * cosf(currentAngle * float(M_PI) / 180.f) - halfBlasterSize.x,
            battleCenter.y + (distance + halfBlasterSize.x) * sinf(currentAngle * float(M_PI) / 180.f) - halfBlasterSize.y
            };

            m_pGame->m_pWeapons.push_back(new Blaster(
                blasterPos,
                blasterSize,
                1,
                currentAngle - 180
            ));

            }, (i == 121) ? 50 : - 1);
    }

    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->SetCurrentPattern(24); });
    AddStep(currentAttack, [this]() { m_pGame->m_blueEye = 1; });
    for (int i{}; i < 38; i++) {
        float speed{};
        int random{};
        do {
            random = rand() % 4;
        } while (random == m_previousRandom || (random == 1 && i == 35));

        m_previousRandom = random;

        if (i == 30) {
            AddStep(currentAttack, [this]() { m_pGame->m_blueEye = 0; });
            AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "idle"); });
        }

        if (i < 22) {
            speed = 150.f;
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->TakeDamage(1); });
        }
        else if (i < 31) {
            speed = 70.f;
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->TakeDamage(1); });
        }
        else if (i < 36) {
            speed = 40.f;
            AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->TakeDamage(1); });
        }
        else if (i == 36) {
            random = 1;
            speed = 10.f;
        }
        else if (i == 37) {
            random = 0;
            speed = 1.f;
        }

        if (random == 0) {
            AddStep(currentAttack, [this]() {
                if (m_pGame->m_CurrentAnimation == "slamDown") {
                    m_pGame->m_pSans->Animate("body", "down", 4, 0, 1);
                }
                else {
                    m_pGame->m_CurrentAnimation = "slamDown";
                }
                });
            AddStep(currentAttack, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ 0, -1 }, speed); }, (i == 37) ? 100.f : 1000.f / speed);
        }
        else if (random == 1) {
            AddStep(currentAttack, [this]() {
                if (m_pGame->m_CurrentAnimation == "slamUp") {
                    m_pGame->m_pSans->Animate("body", "up", 4, 0, 1);
                }
                else {
                    m_pGame->m_CurrentAnimation = "slamUp";
                }
                });
            AddStep(currentAttack, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ 0, 1 }, speed); }, 1000.f / speed);
        }
        else if (random == 2) {
            AddStep(currentAttack, [this]() {
                if (m_pGame->m_CurrentAnimation == "slamLeft") {
                    m_pGame->m_pSans->Animate("body", "left", 4, 0, 1);
                }
                else {
                    m_pGame->m_CurrentAnimation = "slamLeft";
                }
                });
            AddStep(currentAttack, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ -1, 0 }, speed); }, 1000.f / speed);
        }
        else if (random == 3) {
            AddStep(currentAttack, [this]() {
                if (m_pGame->m_CurrentAnimation == "slamRight") {
                    m_pGame->m_pSans->Animate("body", "right", 4, 0, 1);
                }
                else {
                    m_pGame->m_CurrentAnimation = "slamRight";
                }
                });
            AddStep(currentAttack, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ 1, 0 }, speed); }, 1000.f / speed);
        }
    }
    AddStep(currentAttack, [this]() { m_pGame->m_pSoul->SetMode(0); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->Move(Vector2f{ 0, 20.f }); });
    AddStep(currentAttack, [this]() { m_pGame->m_CurrentAnimation = "idle"; });
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "tired2"); });
    AddStep(currentAttack, [this]() { m_pGame->m_megalovania.Pause(); });
    AddSansSpeech(currentAttack, "huff... puff...");
    AddStep(currentAttack, [this]() { m_pGame->m_pSans->ChangeTexture("head", "wink"); });
    AddSansSpeech(currentAttack, "alright, i guess you win.");
    AddStep(currentAttack, [this]() { m_pGame->m_pBattleUI->SetCurrentPattern(25); });
}

void AttackManager::Update(float elapsedSec) {

    if (!m_isAttackInProgress || m_pGame->m_pBattleUI->IsSansSpeaking() || m_pGame->m_pBattleUI->IsEndScreen()) return;

    if (m_stepFrames == -1) {
        m_stepFrames = 0;
    }

    if (m_stepFrames > 0) {
        m_stepFrames--;
        return;
    }

    while (m_isAttackInProgress && m_stepFrames <= 0) {
        if (m_stepFrames == -1) {
            m_stepFrames = 1;
            break;
        }

        auto& pattern = m_attackPatterns[m_currentPattern];
        pattern[m_currentStep].first();

        if (m_currentStep + 1 < pattern.size()) {
            m_stepFrames = pattern[m_currentStep].second;
            m_currentStep++;
        }
        else {
            m_isAttackInProgress = false;
            m_currentPattern += 1;
        }

        if (m_stepFrames > 0 || m_pGame->m_pBattleUI->IsSansSpeaking()) {
            if (m_stepFrames == 1) {
                m_stepFrames = 0;
            }
            break;
        }
    }
}

void AttackManager::StartPattern(size_t patternIndex) {
    if (patternIndex >= m_attackPatterns.size() || m_attackPatterns[patternIndex].empty()) return;
    if (!m_pGame->m_megalovania.IsPlaying() && patternIndex > 0) m_pGame->m_megalovania.Play(1);
    m_currentPattern = patternIndex;
    m_currentStep = 0;
    m_stepFrames = 0;
    m_isAttackInProgress = true;

    if (m_pGame->m_pBattleUI->IsInMenu()) {
        m_pGame->m_pBattleUI->SetInMenu(0, m_pGame->m_pSoul);
    }
}

void AttackManager::EndPattern(size_t patternIndex)
{
    AddStep(patternIndex, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(patternIndex, [this]() { m_pGame->m_CurrentAnimation = "idle"; });
    AddStep(patternIndex, [this]() { m_pGame->m_pBattleUI->SetInMenu(1, m_pGame->m_pSoul); });
    AddStep(patternIndex, [this, patternIndex]() {
        if (patternIndex < m_completedPatterns.size() && !m_completedPatterns[patternIndex]) {
            m_completedPatterns[patternIndex] = true;
        }
        });
    AddStep(patternIndex, [this]() {
        for (int i{ 0 }; i < m_pGame->m_pWeapons.size(); i++) {
            delete m_pGame->m_pWeapons[i];
            m_pGame->m_pWeapons[i] = nullptr;
        }
        m_pGame->m_pWeapons.clear();
        });
}

void AttackManager::AddStep(size_t patternIndex, std::function<void()> action, int delayBeforeNext) {
    if (patternIndex >= m_attackPatterns.size()) {
        m_attackPatterns.resize(patternIndex + 1);
        m_completedPatterns.resize(patternIndex + 1, false);
    }
    m_attackPatterns[patternIndex].push_back({ action, delayBeforeNext });
}

void AttackManager::StartBoneJump(size_t patternIndex)
{
    AddStep(patternIndex, [this]() { m_pGame->m_pBattleUI->ChangeBattleBox(340, 130, 0.5f); });
    SoulToBottomMiddle(patternIndex);
    AddStep(patternIndex, [this]() { m_pGame->m_pSoul->SetMode(1); });
    AddStep(patternIndex, [this]() { m_pGame->m_pSans->ResetSans(); });
    AddStep(patternIndex, [this]() { m_pGame->m_CurrentAnimation = "idle"; });
}

void AttackManager::Slam(size_t patternIndex, std::string direction, int delay, float speed, float height)
{
    if (direction == "down") {
        AddStep(patternIndex, [this]() { 
            if (m_pGame->m_CurrentAnimation == "slamDown") {
                m_pGame->m_pSans->Animate("body", "down", 4, 0, 1);
            }
            else {
                m_pGame->m_CurrentAnimation = "slamDown";
            }
            });
        AddStep(patternIndex, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ 0, -1 }, speed); });
        AddStep(patternIndex, [this, height, speed]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 0, -1 }, Vector2f{ height, height }, speed)); }, delay);
    }
    else if (direction == "left") {
        AddStep(patternIndex, [this]() {
            if (m_pGame->m_CurrentAnimation == "slamLeft") {
                m_pGame->m_pSans->Animate("body", "left", 5, 0, 1);;
            }
            else {
                m_pGame->m_CurrentAnimation = "slamLeft";
            }
            });
        AddStep(patternIndex, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ -1, 0 }, speed); });
        AddStep(patternIndex, [this, height, speed]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ -1, 0 }, Vector2f{ height, height }, speed)); }, delay);
    }
    else if (direction == "up") {
        AddStep(patternIndex, [this]() {
            if (m_pGame->m_CurrentAnimation == "slamUp") {
                m_pGame->m_pSans->Animate("body", "up", 5, 0, 1);
            }
            else {
                m_pGame->m_CurrentAnimation = "slamUp";
            }
            });
        AddStep(patternIndex, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ 0, 1 }, speed); });
        AddStep(patternIndex, [this, height, speed]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 0, 1 }, Vector2f{ height, height }, speed)); }, delay);
    }
    else if (direction == "right") {
        AddStep(patternIndex, [this]() {
            if (m_pGame->m_CurrentAnimation == "slamRight") {
                m_pGame->m_pSans->Animate("body", "right", 5, 0, 1);
            }
            else {
                m_pGame->m_CurrentAnimation = "slamRight";
            }
            });
        AddStep(patternIndex, [this, speed]() { m_pGame->m_pSoul->Slam(Vector2f{ 1, 0 }, speed); });
        AddStep(patternIndex, [this, height, speed]() { m_pGame->m_pWeapons.push_back(new Bone(Vector2f{}, m_pGame->m_pBattleUI->GetBattleBounds(), Bone::Type::SPREAD, Vector2f{ 1, 0 }, Vector2f{ height, height }, speed)); }, delay);
    }
}

void AttackManager::SpawnBoneGap(size_t patternIndex, float gapSize, int delay, float gapBottomY, float speed, int spawnSide) {

    AddStep(patternIndex, [this, gapSize, gapBottomY, speed, spawnSide]() {
        const Rectf& bounds = m_pGame->m_pBattleUI->GetBattleBounds();
        float actualGapBottomY{};

        if (gapBottomY == 0) {
            actualGapBottomY = bounds.bottom + m_savedGapBottomY;
        }
        else {
            actualGapBottomY = bounds.bottom + gapBottomY;
        }

        float gapTopY = actualGapBottomY + gapSize;
        float bottomBoneHeight = actualGapBottomY - bounds.bottom;

        if (spawnSide <= 0) { // Left side
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left - 5.f, bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1, 0 },
                Vector2f{ 0, bottomBoneHeight },
                speed
            ));

            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left - 5.f, gapTopY },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ 1, 0 },
                Vector2f{ 0, bounds.bottom + bounds.height - gapTopY },
                speed
            ));
        }

        if (spawnSide >= 0) { // Right side
            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left + bounds.width - 5.f, bounds.bottom },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ -1, 0 },
                Vector2f{ 0, bottomBoneHeight },
                speed
            ));

            m_pGame->m_pWeapons.push_back(new Bone(
                Vector2f{ bounds.left + bounds.width - 5.f, gapTopY },
                bounds,
                Bone::Type::NORMAL,
                Vector2f{ -1, 0 },
                Vector2f{ 0, bounds.bottom + bounds.height - gapTopY },
                speed
            ));
        }
        }, delay);
}

void AttackManager::Blackout(size_t patternIndex, int duration)
{
    AddStep(patternIndex, [this]() { 
        m_pGame->m_blackout = 1;
        m_flashSound.Play();
        }, duration );
    AddStep(patternIndex, [this]() { 
        m_pGame->m_blackout = 0;
        m_flashSound.Play();
        });
}

void AttackManager::AddSansSpeech(size_t patternIndex, const std::string text, int delay) {
    AddStep(patternIndex, [this, patternIndex, text]() {
        if (!m_completedPatterns[patternIndex]) {
            m_pGame->m_pBattleUI->SansSpeech(text);
        }
        }, delay);
}

void AttackManager::AddSansSpeech(size_t patternIndex, const std::vector<std::string> strings, int delay)
{
    AddStep(patternIndex, [this, patternIndex, strings]() {
        if (!m_completedPatterns[patternIndex]) {
            m_pGame->m_pBattleUI->SansSpeech(strings);
        }
        }, delay);
}

void AttackManager::SoulToMiddle(size_t patternIndex)
{
    AddStep(patternIndex, [this]() {
        const Rectf bounds = m_pGame->m_pBattleUI->GetBattleBounds();
        const Rectf soulHitbox = m_pGame->m_pSoul->GetHitbox();

        Vector2f targetPos{
            bounds.left + bounds.width / 2,
            bounds.bottom + bounds.height / 2
        };

        m_pGame->m_pSoul->Teleport(targetPos);
        });
}

void AttackManager::SoulToBottomMiddle(size_t patternIndex)
{
    AddStep(patternIndex, [this]() { m_pGame->m_pSoul->Teleport(Vector2f{ 
        m_pGame->m_pBattleUI->GetBattleBounds().left + m_pGame->m_pBattleUI->GetBattleBounds().width / 2.f, 
        m_pGame->m_pBattleUI->GetBattleBounds().bottom + m_pGame->m_pSoul->GetHitbox().height}); });
}
