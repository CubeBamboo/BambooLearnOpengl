#include "ball_object.h"
#include "breakout_utils.h"
#include "game_object.h"

using namespace breakout;

BallObject::BallObject()
    : GameObject(), Radius(12.5f), Stuck(true)
{
}

BallObject::BallObject(glm::vec2 pos, GLfloat radius, glm::vec2 velocity, Texture2D sprite)
    : GameObject(pos, glm::vec2(radius * 2, radius * 2), sprite, glm::vec3(1.0f), velocity), Radius(radius), Stuck(true)
{
    float s = (2 * radius) / sprite.Width; // circle width -> match the radius length.
    this->Scale  = glm::vec2(s);
}

// Resets the ball to initial Stuck Position (if ball is outside window bounds)
void BallObject::Reset(glm::vec2 position, glm::vec2 velocity)
{
    this->Position = position;
    this->Velocity = velocity;
    this->Stuck = true;
}

glm::vec2 BallObject::MoveUpdate(GLfloat dt)
{
    if (!this->Stuck)
    {
        glm::vec2 size = GetSize();
        float spriteWidth = size.x;
        float spriteHeight = size.y;

        this->Position += this->Velocity * dt;
        if (this->Position.x <= 0.0f)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = 0.0f;
        }
        else if (this->Position.x >= GameBreakout.Width - spriteWidth)
        {
            this->Velocity.x = -this->Velocity.x;
            this->Position.x = GameBreakout.Width - spriteWidth;
        }
        if (this->Position.y <= 0.0f)
        {
            this->Velocity.y = -this->Velocity.y;
            this->Position.y = 0.0f;
        }

        {
            if (EnableDVD && this->Position.y >= GameBreakout.Height - spriteHeight)
            {
                this->Velocity.y = -this->Velocity.y;
                this->Position.y = GameBreakout.Height - spriteHeight;
                debug_log("ball " << this->Position.x << " " << this->Position.y);
            }
        }
    }
    return this->Position;
}