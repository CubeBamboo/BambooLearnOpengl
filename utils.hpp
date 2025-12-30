#pragma once
#include<string>
#include<fstream>
#include<sstream>

std::string read_txt_file(const std::string& filePath) {
    if (filePath.empty())
        return std::string();

    // 检查文件是否存在
    if (!std::ifstream(filePath)) {
        std::cout << "文件不存在: " << filePath << std::endl;
        throw std::runtime_error("文件不存在1: " + filePath);
    }
    
    // 创建输入文件流对象
    std::ifstream file(filePath);

    // 检查文件是否成功打开
    if (!file.is_open()) {
        throw std::runtime_error("无法打开文件: " + filePath);
    }

    // 使用stringstream作为中介读取全部内容
    std::stringstream buffer;
    buffer << file.rdbuf();  // 读取文件全部内容到buffer

    // 将buffer内容转换为string并返回
    return buffer.str();
}

static uint32_t hash2i(int x, int y)
{
    uint32_t h = static_cast<uint32_t>(x);
    h ^= static_cast<uint32_t>(y) * 0x9e3779b9;
    h ^= h >> 16;
    h *= 0x7feb352d;
    h ^= h >> 15;
    h *= 0x846ca68b;
    h ^= h >> 16;
    return h;
}

static float hash2D(int x, int y)
{
    return (hash2i(x, y) & 0xFFFFFF) / float(0x1000000);
}

static float hash1D(int x)
{
    // Integer hash → [0,1]
    uint32_t h = static_cast<uint32_t>(x);
    h ^= h >> 16;
    h *= 0x7feb352d;
    h ^= h >> 15;
    h *= 0x846ca68b;
    h ^= h >> 16;
    return (h & 0xFFFFFF) / float(0x1000000); // [0,1)
}

static float smooth(float t)
{
    // Smoothstep (C1 continuous)
    return t * t * (3.0f - 2.0f * t);
}

// [0,1]
float noise1D(float x)
{
    int x0 = static_cast<int>(std::floor(x));
    int x1 = x0 + 1;

    float t = x - x0;

    float v0 = hash1D(x0);
    float v1 = hash1D(x1);

    return v0 + (v1 - v0) * smooth(t); // [0,1]
}

float noise1D(float x, int y)
{
    int x0 = static_cast<int>(std::floor(x));
    int x1 = x0 + 1;

    float t = x - x0;

    float v0 = hash2D(x0, y);
    float v1 = hash2D(x1, y);

    return v0 + (v1 - v0) * smooth(t);
}

float fbm1D(float x, int octaves = 5)
{
    float sum = 0.0f;
    float amp = 0.5f;
    float freq = 1.0f;

    for (int i = 0; i < octaves; ++i)
    {
        sum += amp * noise1D(x * freq);
        freq *= 2.0f;
        amp *= 0.5f;
    }
    return sum;
}

glm::vec3 noise3D(float t, float gap = 10000.)
{
    return normalize(glm::vec3(
        noise1D(t, 0),
        noise1D(t, 1),
        noise1D(t, 2)
    ));
}
