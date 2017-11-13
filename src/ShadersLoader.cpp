#include <string>
#include <ios>
#include <fstream>
#include "ShadersLoader.h"

std::string shaders::loadShaderSourceFromFile(const char *path) {
    std::string shaderCode;
    std::ifstream shaderStream(path, std::ios::in);
    if (shaderStream.is_open()) {
        std::string line = "";
        while (getline(shaderStream, line))
            shaderCode += "\n" + line;
        shaderStream.close();
    }
    shaderCode += "\n";

    return shaderCode;
}