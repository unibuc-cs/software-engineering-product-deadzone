#pragma once

#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "../ResourceManager/Font.h"
#include "../ResourceManager/Shader.h"

class TextRenderer
{
public:
    static TextRenderer& get();

    void draw(Shader& shader, Font& font, const std::string& text, float x, float y, float scale, const glm::vec3& color = glm::vec3(1.0f));

    double getTextHeight(Font& font, double scale) const { return font.Characters['H'].bearing.y * scale; }
    double getTextWidth(Font& font, double scale, const std::string& str) const;

private:
    TextRenderer();
    ~TextRenderer();
    TextRenderer(const TextRenderer& other) = delete;
    TextRenderer& operator= (const TextRenderer& other) = delete;
    TextRenderer(const TextRenderer&& other) = delete;
    TextRenderer& operator= (const TextRenderer&& other) = delete;

    unsigned int VAO, VBO;
};

