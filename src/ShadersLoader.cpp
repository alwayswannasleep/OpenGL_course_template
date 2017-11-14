#include <string>
#include <ios>
#include <fstream>
#include <iostream>
#include <sstream>
#include "ShadersLoader.h"

std::string shaders::loadShaderSourceFromFile(const char *path) {
    std::string shaderCode;
    std::ifstream fileStream;

    fileStream.exceptions(std::ios::failbit | std::ios::badbit);
    try {
        fileStream.open(path, std::ios::in);
        std::string line = "";
        while (fileStream.peek() != EOF) {
            if (!getline(fileStream, line)) {
                std::cerr << "Error get line from ifstream.\n";
                break;
            }
            shaderCode += "\n" + line;
        }
        fileStream.close();
    } catch (std::ios::failure exception) {
        std::cerr << "Error opening file '" << path << "', exception - '" << exception.what() << "', error code - '" << exception.code() << "'.\n";
    }

    return shaderCode;
}