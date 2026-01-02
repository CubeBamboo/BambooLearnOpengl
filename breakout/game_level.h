#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "game_object.h"
#include "resource_manager.h"

namespace breakout
{
    class GameLevel
    {
    public:
        std::vector<std::shared_ptr<GameObject>> Bricks;

        GameLevel() {}
        // 从文件中加载关卡
        /* ex.
        1 1 1 1 1 1 
        2 2 0 0 2 2
        3 3 4 4 3 3
        */
        void Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight);
        // 渲染关卡
        void Draw(SpriteRenderer& renderer);
        // 检查一个关卡是否已完成 (所有非坚硬的瓷砖均被摧毁)
        GLboolean IsCompleted();
    private:
        // 由砖块数据初始化关卡
        void init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight);
    };
}