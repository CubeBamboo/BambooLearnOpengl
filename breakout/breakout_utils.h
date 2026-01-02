#pragma once

#include <iostream>
#include <glad/glad.h>

#include "game.h"
#include "ball_object.h"
#include "game_object.h"

#define debug_log(s) std::cout << s << std::endl;

extern GLuint SCREEN_WIDTH;
extern GLuint SCREEN_HEIGHT;

extern breakout::Game GameBreakout;

extern bool DEBUG_BOUND;


namespace breakout
{
    std::string toString(glm::vec2 v);
    
    enum Direction {
        UP,
        RIGHT,
        DOWN,
        LEFT
    };

    class Collision
    {
    public:
        GLboolean hit;
        Direction dir; // 受力方向
        glm::vec2 diff;
    };

    Direction VectorDirection(glm::vec2 target);

    GLboolean CheckCollision(GameObject& one, GameObject& two);

    Collision CheckCollision2(BallObject& one, GameObject& two);

    std::string toString(Collision v);
}