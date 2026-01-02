#include "game_object.h"
#include "game_level.h"
#include "breakout_utils.h"

using namespace breakout;

GameObject::GameObject()
    : Position(0.0f, 0.0f), Scale(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) {
}

GameObject::GameObject(glm::vec2 pos, glm::vec2 _size_useless, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Scale(1.0f, 1.0f), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) {
}

GameObject::GameObject(glm::vec2 pos, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
    : Position(pos), Scale(1.0f, 1.0f), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) {
}

void GameObject::Draw(SpriteRenderer& renderer)
{
    if (Destroyed) return;

    renderer.DrawSprite(this->Sprite, this->Position, this->Scale, this->Rotation, this->Color);
    if (DEBUG_BOUND)
    {
        auto size = GetSize();
        auto col = glm::vec4(0., 1., 0., 0.2);
        //auto col = glm::vec4(1., 0., 0., 0.2);
        if (Destroyed)
            col = glm::vec4(1., 0., 0., 0.2);
        renderer.DrawSprite(ResourceManager::GetTexture("1px"), this->Position, size, 0., col);
    }
}

glm::vec2 breakout::GameObject::GetSize() const
{
    return glm::vec2(Sprite.Width * Scale.x, Sprite.Height * Scale.y);
}
