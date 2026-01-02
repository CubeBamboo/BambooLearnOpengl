#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

#include "game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"
#include "breakout_utils.h"

using namespace breakout;

// ====declaration====
GameLevel getLevelCurrent(Game* game);

// ====variables====
SpriteRenderer* Renderer;
GameObject* Player;

// 初始化当班的速率
const GLfloat PLAYER_VELOCITY(500.0f);
float SPRITE_GLOBAL_UNITS = 0.8;

// 初始化球的速度
const glm::vec2 BALL_VELOCITY(-100.0f, -350.0f);
const float BALL_VELOCITY_SCALE = 1.0f;
// 球的半径
const GLfloat BALL_RADIUS = 18.5f;

BallObject* Ball;

bool Pause = false;

// ====function implementation====
Game::Game(GLuint width, GLuint height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height), Level(0)
{

}

Game::~Game()
{

}

void Game::Init()
{
    if (SPRITE_GLOBAL_UNITS < 0.)
    {
        SPRITE_GLOBAL_UNITS = 1.;
    }

    ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", nullptr, "sprite");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(this->Width),
        static_cast<GLfloat>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetTexture("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);
    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));

    // textures
    ResourceManager::LoadTexture("textures/awesomeface.png", GL_TRUE, "face");
    ResourceManager::LoadTexture("textures/block_2.png", GL_TRUE, "block");
    ResourceManager::LoadTexture("textures/block_3.png", GL_TRUE, "block_solid");
    ResourceManager::LoadTexture("textures/paddle.png", GL_TRUE, "paddle");
    ResourceManager::LoadTexture("textures/1px.png", GL_TRUE, "1px");

    // levels
    GameLevel one; one.Load("levels/1.txt", this->Width, this->Height);
    GameLevel two; two.Load("levels/2.txt", this->Width, this->Height);
    GameLevel three; three.Load("levels/3.txt", this->Width, this->Height);
    GameLevel four; four.Load("levels/4.txt", this->Width, this->Height);
    GameLevel fw; fw.Load("levels/5.txt", this->Width, this->Height);
    this->Levels.push_back(one);
    this->Levels.push_back(two);
    this->Levels.push_back(three);
    this->Levels.push_back(four);
    this->Levels.push_back(fw);
    this->Level = 1;

    glm::vec2 playerPos = glm::vec2(
        this->Width / 2 - 5,
        this->Height - 35
    );
    Player = new GameObject(playerPos, ResourceManager::GetTexture("paddle"));
    Player->Scale = glm::vec2(0.2);

    glm::vec2 ballPos = playerPos + glm::vec2(40 - BALL_RADIUS, -BALL_RADIUS * 2);
    Ball = new BallObject(ballPos, BALL_RADIUS, BALL_VELOCITY * BALL_VELOCITY_SCALE,
        ResourceManager::GetTexture("face"));

    State = GAME_ACTIVE;
    Ball->Stuck = false;
}

void Game::Update(GLfloat dt)
{
    if (Pause) return;

    Ball->MoveUpdate(dt);
    DoCollisions();

    if (Ball->Position.y >= this->Height) // 球是否接触底部边界？
    {
    } 
}

void Game::ProcessInput(GLfloat dt)
{
    GLfloat velocity = PLAYER_VELOCITY * dt;
    // 移动挡板
    if (this->Keys[GLFW_KEY_A])
    {
        if (Player->Position.x >= 0)
            Player->Position.x -= velocity;
    }
    if (this->Keys[GLFW_KEY_D])
    {
        if (Player->Position.x <= this->Width - 100)
            Player->Position.x += velocity;
    }
    if (this->Keys[GLFW_KEY_SPACE])
        Ball->Stuck = false;
}

void Game::Render()
{
    //auto face = ResourceManager::GetTexture("face");
    //Renderer->DrawSprite(face, glm::vec2(200, 200), glm::vec2(300, 400), 45.0f, glm::vec3(0.1f, 0.8f, 1.0f));

    if (State == GAME_ACTIVE)
    {
        //Renderer->DrawSprite(ResourceManager::GetTexture("background"),
            //glm::vec2(0, 0), glm::vec2(this->Width, this->Height), 0.0f);
        getLevelCurrent(this).Draw(*Renderer);
        Player->Draw(*Renderer);
        Ball->Draw(*Renderer);
    }
}

void Game::DoCollisions()
{
    for (auto box : getLevelCurrent(this).Bricks)
    {
        if (!box->Destroyed)
        {
            auto hit1 = CheckCollision2(*Ball, *box);
            if (hit1.hit)
            {
                if (!box->IsSolid)
                {
                    box->Destroyed = GL_TRUE;
                }

                // 处理碰撞
                debug_log( "hit1" << toString(hit1));

                auto dir = hit1.dir;
                auto diff = hit1.diff;
                if (dir == LEFT || dir == RIGHT) // 水平方向碰撞
                {
                    if (dir == LEFT)
                        Ball->Velocity.x = std::abs(Ball->Velocity.x);
                    else if (dir == RIGHT)
                        Ball->Velocity.x = -std::abs(Ball->Velocity.x);

                    // 重定位
                    GLfloat penetration = Ball->Radius - std::abs(diff.x);
                    if (dir == LEFT)
                        Ball->Position.x += penetration; // 将球右移
                    else
                        Ball->Position.x -= penetration; // 将球左移
                }
                else // 垂直方向碰撞
                {
                    if (dir == UP)
                        Ball->Velocity.y = -std::abs(Ball->Velocity.y);
                    else if (dir == DOWN)
                        Ball->Velocity.y = std::abs(Ball->Velocity.y);
                    // 重定位
                    GLfloat penetration = Ball->Radius - std::abs(diff.y);
                    if (dir == UP)
                        Ball->Position.y -= penetration; // 将球上移
                    else
                        Ball->Position.y += penetration; // 将球下移
                }
            }
        }
    }

    Collision hit2 = CheckCollision2(*Ball, *Player);
    if (!Ball->Stuck && hit2.hit)
    {
        debug_log("hit2 " << toString(hit2));

        // 检查碰到了挡板的哪个位置，并根据碰到哪个位置来改变速度
        GLfloat centerBoard = Player->Position.x + Player->GetSize().x / 2;
        GLfloat distance = (Ball->Position.x + Ball->Radius) - centerBoard;
        GLfloat percentage = distance / (Player->GetSize().x / 2);
        // 依据结果移动
        GLfloat strength = 2.0f;
        glm::vec2 oldVelocity = Ball->Velocity;
        Ball->Velocity.x = BALL_VELOCITY.x * BALL_VELOCITY_SCALE * percentage * strength;
        //Ball->Velocity.y = -Ball->Velocity.y;
        Ball->Velocity.y = -1 * abs(Ball->Velocity.y);
        Ball->Velocity = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
    }
}

void Game::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    auto& game = GameBreakout;
    
    if (action == GLFW_PRESS && key == GLFW_KEY_V)
    {
        game.Level++;
        if (game.Level >= 5)
        {
            game.Level = 1;
        }
        debug_log("level change to " << game.Level);
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_N)
    {
        Ball->Velocity *= 0.5;
        debug_log("ball vel change to " << toString(Ball->Velocity));
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_M)
    {
        Ball->Velocity *= 1.5;
        debug_log("ball vel change to " << toString(Ball->Velocity));
    }

    if (action == GLFW_PRESS && key == GLFW_KEY_P)
    {
        Pause = !Pause;
    }
}

GameLevel getLevelCurrent(Game* game)
{
    return game->Levels[game->Level - 1];
}