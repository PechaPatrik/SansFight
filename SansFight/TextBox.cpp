#include "pch.h"
#include "TextBox.h"
#include "Texture.h"
#include <iostream>

TextBox::TextBox(Font font, Vector2f startingPos, float width, std::vector<std::string> strings, bool drawAsterisk, Color4f color) :
    m_font(font),
    m_startPos(startingPos),
    m_maxWidth(width),
    m_strings(strings),
    m_currentStringIndex(0),
    m_currentLineIndex(0),
    m_currentCharIndex(0),
    m_pauseFrames(0),
    m_endOfText(0),
    m_currentStringComplete(0),
    m_shouldDrawAsterisk(drawAsterisk),
    m_asteriskTexture(nullptr),
    m_color(color)
{
    switch (font) {
    case Font::DEFAULT:
        m_pFont = TTF_OpenFont("Fonts/DefaultFont.ttf", 20);
        break;
    case Font::SANS:
        if (m_color.r == 0.f) {
            m_pFont = TTF_OpenFont("Fonts/ComicSans.ttf", 18);
        }
        else {
            m_pFont = TTF_OpenFont("Fonts/ComicSans.ttf", 24);
        }
        break;
    }
    GenerateTextures();
}

TextBox::TextBox(Font font, Vector2f startingPos, float width, std::string string, bool drawAsterisk, Color4f color) :
    m_font(font),
    m_startPos(startingPos),
    m_maxWidth(width),
    m_currentStringIndex(0),
    m_currentLineIndex(0),
    m_currentCharIndex(0),
    m_pauseFrames(0),
    m_endOfText(0),
    m_currentStringComplete(0),
    m_shouldDrawAsterisk(drawAsterisk),
    m_asteriskTexture(nullptr),
    m_color(color)
{
    switch (font) {
    case Font::DEFAULT:
        m_pFont = TTF_OpenFont("Fonts/DefaultFont.ttf", 20);
        break;
    case Font::SANS:
        if (m_color.r == 0.f) {
            m_pFont = TTF_OpenFont("Fonts/ComicSans.ttf", 18);
        }
        else {
            m_pFont = TTF_OpenFont("Fonts/ComicSans.ttf", 24);
        }
        break;
    }
    m_strings.push_back(string);
    GenerateTextures();
}

TextBox::~TextBox()
{
    for (Texture* texture : m_textTextures)
    {
        delete texture;
    }
    m_textTextures.clear();

    if (m_asteriskTexture)
    {
        delete m_asteriskTexture;
        m_asteriskTexture = nullptr;
    }

    if (m_pFont) {
        TTF_CloseFont(m_pFont);
        m_pFont = nullptr;
    }
}

void TextBox::Update(float elapsedSec)
{
    if (m_endOfText || m_currentStringComplete) return;

    if (m_pauseFrames > 0)
    {
        m_pauseFrames--;
        return;
    }

    AdvanceText();
}

void TextBox::Draw() const
{
    if (m_textTextures.empty()) return;

    Vector2f currentPos = m_startPos;

    if (m_asteriskTexture && m_shouldDrawAsterisk)
    {
        Vector2f asteriskPos = currentPos;
        asteriskPos.x -= m_asteriskTexture->GetWidth() + 5.f;
        m_asteriskTexture->Draw(asteriskPos);
    }

    // Draw all completed lines
    for (int i = 0; i < m_currentLineIndex; ++i)
    {
        if (i < m_textTextures.size())
        {
            m_textTextures[i]->Draw(currentPos);
            currentPos.y -= m_textTextures[i]->GetHeight();
        }
    }

    // Draw current line in progress
    if (m_currentLineIndex < m_textTextures.size())
    {
        std::string partialText = m_wrappedLines[m_currentLineIndex].substr(0, m_currentCharIndex);
        if (!partialText.empty())
        {
            Texture partialTexture(partialText, m_pFont, m_color);
            partialTexture.Draw(currentPos);
        }
    }
}

void TextBox::SkipAnimation()
{
    if (m_currentLineIndex >= m_wrappedLines.size()) return;

    m_currentLineIndex = m_wrappedLines.size() - 1;
    m_currentCharIndex = m_wrappedLines.back().length();
    m_currentStringComplete = 1;
}

void TextBox::NextString()
{
    if (!m_currentStringComplete && m_currentStringIndex < m_strings.size())
    {
        return;
    }

    m_currentStringIndex++;
    if (m_currentStringIndex >= m_strings.size())
    {
        m_endOfText = 1;
        return;
    }

    m_currentStringComplete = 0;
    GenerateTextures();
}

void TextBox::GenerateTextures()
{
    // Clear existing textures
    for (Texture* texture : m_textTextures)
    {
        delete texture;
    }
    m_textTextures.clear();
    m_wrappedLines.clear();

    if (m_asteriskTexture)
    {
        delete m_asteriskTexture;
        m_asteriskTexture = nullptr;
    }

    // Create asterisk texture
    m_asteriskTexture = new Texture("*", m_pFont, m_color);

    if (m_currentStringIndex >= m_strings.size()) return;

    std::string currentText = m_strings[m_currentStringIndex];

    // Simple word wrapping
    std::string currentLine;
    std::string currentWord;

    for (char c : currentText)
    {
        if (c == ' ')
        {
            if (!currentWord.empty())
            {
                std::string testLine = currentLine.empty() ? currentWord : currentLine + " " + currentWord;
                Texture* testTexture = new Texture(testLine, m_pFont, m_color);

                if (testTexture->GetWidth() <= m_maxWidth || currentLine.empty())
                {
                    currentLine = testLine;
                }
                else
                {
                    m_wrappedLines.push_back(currentLine);
                    m_textTextures.push_back(new Texture(currentLine, m_pFont, m_color));
                    currentLine = currentWord;
                }

                delete testTexture;
                currentWord.clear();
            }
        }
        else
        {
            currentWord += c;
        }
    }

    // Add the last word
    if (!currentWord.empty())
    {
        std::string testLine = currentLine.empty() ? currentWord : currentLine + " " + currentWord;
        Texture* testTexture = new Texture(testLine, m_pFont, m_color);

        if (testTexture->GetWidth() <= m_maxWidth || currentLine.empty())
        {
            currentLine = testLine;
        }
        else
        {
            m_wrappedLines.push_back(currentLine);
            m_textTextures.push_back(new Texture(currentLine, m_pFont, m_color));
            currentLine = currentWord;
        }

        delete testTexture;
    }

    // Add the last line
    if (!currentLine.empty())
    {
        m_wrappedLines.push_back(currentLine);
        m_textTextures.push_back(new Texture(currentLine, m_pFont, m_color));
    }

    // Reset animation state
    m_currentLineIndex = 0;
    m_currentCharIndex = 0;
    m_currentStringComplete = 0;
}

bool TextBox::ShouldPlaySound(char c) const
{
    return c != ' ' && c != '*';
}

void TextBox::AdvanceText()
{
    if (m_currentLineIndex >= m_wrappedLines.size())
    {
        m_currentStringComplete = 1;
        return;
    }

    const std::string& currentLine = m_wrappedLines[m_currentLineIndex];

    // Check if we need to pause for a comma
    if (m_currentCharIndex < currentLine.length() && (currentLine[m_currentCharIndex] == ',' || currentLine[m_currentCharIndex] == '?' || currentLine[m_currentCharIndex] == '!'))
    {
        m_pauseFrames = m_maxPauseFrames;
        m_currentCharIndex++;
        return;
    }

    // Play sound if appropriate (everything but spaces and asterisks)
    if (m_currentCharIndex < currentLine.length() && ShouldPlaySound(currentLine[m_currentCharIndex]))
    {
        if (m_font == Font::SANS) {
            m_sansSound.Play();
        }
        else if (m_font == Font::DEFAULT) {
            m_defaultSound.Play();
        }
    }

    m_currentCharIndex++;

    if (m_currentCharIndex > currentLine.length())
    {
        m_currentLineIndex++;
        m_currentCharIndex = 0;

        if (m_currentLineIndex >= m_wrappedLines.size())
        {
            m_currentStringComplete = 1;
        }
    }
}