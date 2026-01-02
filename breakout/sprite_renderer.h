#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

#include "texture.h"
#include "shader.h"

namespace breakout
{
    class SpriteRenderer
    {
    public:
        // Constructor (inits shaders/shapes)
        SpriteRenderer(Shader shader);
        // Destructor
        ~SpriteRenderer();
        // Renders a defined quad textured with given sprite
        void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 scale = glm::vec2(1, 1), GLfloat rotateZ = 0.0f, glm::vec3 color = glm::vec3(1.0f));
        void DrawSprite(Texture2D& texture, glm::vec2 position, glm::vec2 scale = glm::vec2(1, 1), GLfloat rotateZ = 0.0f, glm::vec4 color = glm::vec4(1.0f));
    private:
        // Render state
        Shader shader;
        GLuint quadVAO;
        // Initializes and configures the quad's buffer and vertex attributes
        void initRenderData();
    };
}