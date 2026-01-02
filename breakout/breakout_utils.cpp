#include "breakout_utils.h"

using namespace breakout;

bool DEBUG_BOUND = true;

std::string breakout::toString(glm::vec2 v)
{
    return "(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ")";
}

std::string breakout::toString(Collision v)
{
    return "(" + toString(v.diff) + ", " + std::to_string(v.hit) + ", " + std::to_string(v.dir) + ")";
}

Direction breakout::VectorDirection(glm::vec2 target)
{
    glm::vec2 compass[] = {
        glm::vec2(0.0f, 1.0f),  // 上
        glm::vec2(1.0f, 0.0f),  // 右
        glm::vec2(0.0f, -1.0f), // 下
        glm::vec2(-1.0f, 0.0f)  // 左
    };
    GLfloat max = 0.0f;
    GLuint best_match = -1;
    for (GLuint i = 0; i < 4; i++)
    {
        GLfloat dot_product = glm::dot(glm::normalize(target), compass[i]);
        if (dot_product > max)
        {
            max = dot_product;
            best_match = i;
        }
    }
    return (Direction)best_match;
}

GLboolean breakout::CheckCollision(GameObject& one, GameObject& two) // AABB - AABB collision
{
    // x轴方向碰撞？
    bool collisionX = one.Position.x + one.Sprite.Width * one.Scale.x >= two.Position.x &&
        two.Position.x + two.Sprite.Width * two.Scale.x >= one.Position.x;
    // y轴方向碰撞？
    bool collisionY = one.Position.y + one.Sprite.Height * one.Scale.y >= two.Position.y &&
        two.Position.y + one.Sprite.Height * one.Scale.y >= one.Position.y;
    // 只有两个轴向都有碰撞时才碰撞
    return collisionX && collisionY;
}

Collision breakout::CheckCollision2(BallObject& one, GameObject& two) // AABB - Circle collision
{
    // 获取圆的中心 
    auto oneWidth = one.Sprite.Width * one.Scale.x;
    auto oneHeight = one.Sprite.Height * one.Scale.y;

    auto twoWidth = two.Sprite.Width * two.Scale.x;
    auto twoHeight = two.Sprite.Height * two.Scale.y;

    glm::vec2 center(one.Position + one.Radius);
    // 计算AABB的信息（中心、半边长）
    glm::vec2 aabb_half_extents(twoWidth / 2, twoHeight / 2);
    glm::vec2 aabb_center(
        two.Position.x + aabb_half_extents.x,
        two.Position.y + aabb_half_extents.y
    );
    // 获取两个中心的差矢量
    glm::vec2 difference = center - aabb_center;
    glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
    // AABB_center加上clamped这样就得到了碰撞箱上距离圆最近的点closest
    glm::vec2 closest = aabb_center + clamped;
    // 获得圆心center和最近点closest的矢量并判断是否 length <= radius
    difference = closest - center;

    if (glm::length(difference) <= one.Radius)
    {
        Collision ret;
        ret.hit = GL_TRUE;
        ret.diff = difference;
        ret.dir = VectorDirection(difference);
        return ret;
    }
    else
    {
        Collision ret;
        ret.hit = GL_FALSE;
        ret.diff = glm::vec2(0, 0);
        ret.dir = UP;
        return ret;
    }
}
