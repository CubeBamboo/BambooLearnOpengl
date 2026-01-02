#pragma once

#include "game_object.h"
#include "texture.h"

namespace breakout
{
    class BallObject : public GameObject
    {
    public:
        GLfloat   Radius;
        GLboolean Stuck;
        GLboolean EnableDVD = true; // ÎûÎûÎû

        BallObject();
        BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite);

        glm::vec2 MoveUpdate(GLfloat dt);
        void      Reset(glm::vec2 position, glm::vec2 velocity);
    };
}