#include "SpriteRenderer.h"

SpriteRenderer::SpriteRenderer()
{
    // configure VAO and VBO
    GLuint VBO;

    float vertices[] = {
        // pos          // tex
        -0.5f, 0.5f,     0.0f, 0.0f,
        0.5, -0.5f,     1.0f, 1.0f,
        -0.5f, -0.5f,   0.0f, 1.0f,

        -0.5f, 0.5f,     0.0f, 0.0f,
        0.5f, 0.5f,     1.0f, 0.0f,
        0.5, -0.5f,     1.0f, 1.0f
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

SpriteRenderer::~SpriteRenderer()
{
    glDeleteVertexArrays(1, &this->VAO);
}

SpriteRenderer& SpriteRenderer::get()
{
    static SpriteRenderer instance;
    return instance;
}

void SpriteRenderer::draw(Shader& shader, const Texture2D& texture, const glm::vec2& position, const glm::vec2& size, const float& rotate, const glm::vec3& color)
{
    // set shader
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f)); // translate to the desired position
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate
    model = glm::scale(model, glm::vec3(size, 1.0f)); // scale

    // set uniforms
    shader.setMatrix4("model", model);
    shader.setVector3f("spriteColor", color);

    // set texture
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    // draw
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}



// for drawing only a subrectangle of a sprite

void SpriteRenderer::draw(Shader& shader, const Texture2D& texture, const glm::vec2& position, const glm::vec2& size, const float& rotate, const glm::vec3& color, const glm::vec2& posTr, const glm::vec2& sizeTr)
{
    // set shader
    shader.use();

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(position, 0.0f)); // translate to the desired position
    model = glm::rotate(model, glm::radians(rotate), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate
    model = glm::scale(model, glm::vec3(size, 1.0f)); // scale

    // set uniforms
    shader.setMatrix4("model", model);
    shader.setVector3f("spriteColor", color);

    // set texture
    glActiveTexture(GL_TEXTURE0);
    texture.bind();

    // get the current viewport settings
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // set viewport to truncate the drawing
    glViewport(posTr.x, posTr.y, sizeTr.x, sizeTr.y);

    // draw
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // restore the original viewport settings
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);
}

