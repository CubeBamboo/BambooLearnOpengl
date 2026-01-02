#define STB_IMAGE_IMPLEMENTATION

#include "resource_manager.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <filesystem>

#include <glad/glad.h>

#include "stb_image.h"
using namespace breakout;

namespace fs = std::filesystem;

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;

std::vector<std::string> gResourceRootPaths = {
    "res/",
    "breakout/",
    "F:/MyProject/LearnOpenGL-Ash/BambooLearnOpengl/res/",
    "F:/MyProject/LearnOpenGL-Ash/BambooLearnOpengl/breakout/",
};

Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile, std::string name)
{
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader& ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const char* file, bool alpha, std::string name)
{
    Textures[name] = loadTextureFromFile(file, alpha);
    return Textures[name];
}

Texture2D& ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader ResourceManager::loadShaderFromFile(const char* vShaderFile, const char* fShaderFile, const char* gShaderFile)
{
    auto file1 = tryGetValidFile(vShaderFile);
    auto file2 = tryGetValidFile(fShaderFile);
    auto file3 = std::string();
    vShaderFile = file1.c_str();
    fShaderFile = file2.c_str();
    if (gShaderFile != nullptr)
    {
        file3 = tryGetValidFile(gShaderFile);
        gShaderFile = file3.c_str();
    }

    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::string geometryCode;
    try
    {
        // open files
        std::ifstream vertexShaderFile(vShaderFile);
        std::ifstream fragmentShaderFile(fShaderFile);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vertexShaderFile.rdbuf();
        fShaderStream << fragmentShaderFile.rdbuf();
        // close file handlers
        vertexShaderFile.close();
        fragmentShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
        // if geometry shader path is present, also load a geometry shader
        if (gShaderFile != nullptr)
        {
            std::ifstream geometryShaderFile(gShaderFile);
            std::stringstream gShaderStream;
            gShaderStream << geometryShaderFile.rdbuf();
            geometryShaderFile.close();
            geometryCode = gShaderStream.str();
        }
    }
    catch (std::exception e)
    {
        std::cout << "ERROR::SHADER: Failed to read shader files" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    const char* gShaderCode = geometryCode.c_str();
    // 2. now create shader object from source code
    Shader shader;//
    shader.Compile(vShaderCode, fShaderCode, gShaderFile != nullptr ? gShaderCode : nullptr);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const char* file, bool alpha)
{
    auto file1 = tryGetValidFile(file);
    file = file1.c_str();

    // create texture object
    Texture2D texture;
    if (alpha)
    {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    // load image
    int width, height, nrChannels;
    unsigned char* data = stbi_load(file, &width, &height, &nrChannels, 0);
    // now generate texture
    texture.Generate(width, height, data);
    // and finally free image data
    stbi_image_free(data);
    return texture;
}

std::string breakout::ResourceManager::loadTextFromFile(const char* file)
{
    std::ifstream in(tryGetValidFile(file), std::ios::in);
    if (!in)
    {
        std::cerr << "[ResourceManager] Failed to open file: "
            << file << std::endl;
        return {};
    }

    std::ostringstream ss;
    ss << in.rdbuf();
    return ss.str();
}

std::string breakout::ResourceManager::tryGetValidFile(std::string file)
{
    if (fs::exists(file))
    {
        return file;
    }

    for (const std::string& root : gResourceRootPaths)
    {
        fs::path fullPath = fs::path(root) / file;
        
        if (fs::exists(fullPath))
        {
            return fullPath.string();
        }
    }

    std::cerr << "[ResourceManager] tryGetValidFile File not found: " << file << std::endl;
    return {};
}

