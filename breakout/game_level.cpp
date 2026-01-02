#include "game_level.h"

using namespace breakout;

void GameLevel::Draw(SpriteRenderer& renderer)
{
    for (auto tile : this->Bricks)
        if (!tile->Destroyed) // fk you,
            tile->Draw(renderer);
}

GLboolean GameLevel::IsCompleted()
{
    for (auto tile : this->Bricks)
        if (!tile->IsSolid && !tile->Destroyed)
            return GL_FALSE;
    return GL_TRUE;
}

// 从文件中加载关卡
/* ex.
1 1 1 1 1 1
2 2 0 0 2 2
3 3 4 4 3 3
*/
void GameLevel::Load(const GLchar* file, GLuint levelWidth, GLuint levelHeight)
{
    {
        // 清空过期数据
        this->Bricks.clear();
        // 从文件中加载
        GLuint tileCode;
        GameLevel level;
        std::string line;
        std::ifstream fstream(ResourceManager::tryGetValidFile(file));
        std::vector<std::vector<GLuint>> tileData;
        if (fstream)
        {
            while (std::getline(fstream, line)) // 读取关卡文件的每一行
            {
                std::istringstream sstream(line);
                std::vector<GLuint> row;
                while (sstream >> tileCode) // 读取被空格分隔的每个数字
                    row.push_back(tileCode);
                tileData.push_back(row);
            }
            if (tileData.size() > 0)
                this->init(tileData, levelWidth, levelHeight);
        }
    }
}

// 由砖块数据初始化关卡
void GameLevel::init(std::vector<std::vector<GLuint>> tileData, GLuint levelWidth, GLuint levelHeight)
{
    // 计算每个维度的大小
    GLuint height = tileData.size();
    GLuint width = tileData[0].size();
    GLfloat unit_width = 32;
    GLfloat unit_height = 32;
    // 基于tileDataC初始化关卡     
    for (GLuint y = 0; y < height; ++y)
    {
        for (GLuint x = 0; x < width; ++x)
        {
            // 检查砖块类型
            if (tileData[y][x] == 1)
            {
                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                auto obj = std::make_shared<GameObject>(pos, size,
                    ResourceManager::GetTexture("block_solid"),
                    //glm::vec3(0.8f, 0.8f, 0.7f)
                    glm::vec3(1.f, 1.f, 1.f)
                );
                obj->IsSolid = GL_TRUE;
                this->Bricks.push_back(obj);
            }
            else if (tileData[y][x] > 1)
            {
                glm::vec3 color = glm::vec3(1.0f); // 默认为白色
                if (tileData[y][x] == 2)
                    color = glm::vec3(0.2f, 0.6f, 1.0f);
                else if (tileData[y][x] == 3)
                    color = glm::vec3(0.0f, 0.7f, 0.0f);
                else if (tileData[y][x] == 4)
                    color = glm::vec3(0.8f, 0.8f, 0.4f);
                else if (tileData[y][x] == 5)
                    color = glm::vec3(1.0f, 0.5f, 0.0f);

                glm::vec2 pos(unit_width * x, unit_height * y);
                glm::vec2 size(unit_width, unit_height);
                this->Bricks.push_back(
                    std::make_shared<GameObject>(pos, glm::vec2(), ResourceManager::GetTexture("block"), color)
                );
            }
        }
    }
}
