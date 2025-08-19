#pragma once
#include "Texture.h"
#include "SoundEffect.h"
#include <vector>

class TextBox
{
public:
    enum class Font { SANS, DEFAULT };

    TextBox(Font font, Vector2f startingPos, float width, std::vector<std::string> strings, bool drawAsterisk = 1, Color4f color = Color4f{1, 1, 1, 1});
    TextBox(Font font, Vector2f startingPos, float width, std::string string, bool drawAsterisk = 1, Color4f color = Color4f{ 1, 1, 1, 1 });
    ~TextBox();

    TextBox(const TextBox& other) = delete;
    TextBox& operator=(const TextBox& other) = delete;
    TextBox(TextBox&& other) = delete;
    TextBox& operator=(TextBox&& other) = delete;

    void Update(float elapsedSec);
    void Draw() const;

    bool IsEndOfText() const { return m_endOfText; }
    void SkipAnimation();
    void NextString();

    int GetCurrentStringIndex() const { return m_currentStringIndex; }

private:
    void GenerateTextures();
    void AdvanceText();
    bool ShouldPlaySound(char c) const;

    Font m_font;
    TTF_Font* m_pFont;
    SoundEffect m_sansSound{ "Audio/SansSpeak.ogg" };
    SoundEffect m_defaultSound{ "Audio/BattleText.wav" };
    Vector2f m_startPos;
    Color4f m_color;
    float m_maxWidth;
    std::vector<std::string> m_strings;

    Texture* m_asteriskTexture;
    std::vector<Texture*> m_textTextures;
    std::vector<std::string> m_wrappedLines;

    int m_currentStringIndex;
    int m_currentLineIndex;
    int m_currentCharIndex;
    int m_pauseFrames;
    const int m_maxPauseFrames = 10;

    bool m_shouldDrawAsterisk;
    bool m_endOfText;
    bool m_currentStringComplete;
};