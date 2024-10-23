#pragma once 

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../ResourceManager/Texture.h"
#include "../ResourceManager/Shader.h"

class SpriteRenderer
{
public:
    static SpriteRenderer& get();

    void draw(Shader& shader, const Texture2D &texture, const glm::vec2& position, const glm::vec2& size = glm::vec2(100.0f, 100.0f), const float& rotate = 0.0f, const glm::vec3& color = glm::vec3(1.0f));

    
    // for drawing only a subrectangle of a sprite

    void draw(Shader& shader, const Texture2D& texture, const glm::vec2& position, const glm::vec2& size, const float& rotate, const glm::vec3& color, const glm::vec2& posTr, const glm::vec2& sizeTr);



private:
    SpriteRenderer();
    ~SpriteRenderer();
    SpriteRenderer(const SpriteRenderer& other) = delete;
    SpriteRenderer& operator= (const SpriteRenderer& other) = delete;
    SpriteRenderer(const SpriteRenderer&& other) = delete;
    SpriteRenderer& operator= (const SpriteRenderer&& other) = delete;

    GLuint VAO;
};

