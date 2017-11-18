#include <glwrapper.h>
#include "glfw3.h"
#include <thread>
#include <iostream>
#include "ShadersLoader.h"

GLuint createVao(const GLfloat *vertexData, const GLsizei &dataSize, const GLuint *indexes, const GLsizei &indexesSize) {
    GLuint buffer;
    glGenBuffers(1, &buffer);

    GLuint elements;
    glGenBuffers(1, &elements);

    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, dataSize, vertexData, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void *>(sizeof(GLfloat) * 2));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexesSize, indexes, GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return vertexArray;
}

GLuint buildProgram(const char *vertex, const char *fragment) {
    auto shaderSourceFromFileVertex = shaders::loadShaderSourceFromFile(vertex);
    auto vertexSource = shaderSourceFromFileVertex.c_str();

    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetShaderInfoLog(vertexShader, sizeof(info), NULL, info);
        std::cerr << "ERROR::SHADER LINK_FAILED\n" << info;
        return 0;
    }

    auto shaderSourceFromFileFragment = shaders::loadShaderSourceFromFile(fragment);
    auto fragmentSource = shaderSourceFromFileFragment.c_str();

    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetShaderInfoLog(fragmentShader, sizeof(info), NULL, info);
        std::cerr << "ERROR::SHADER LINK_FAILED\n" << info;
        return 0;
    }

    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        GLchar info[512];
        glGetProgramInfoLog(program, sizeof(info), NULL, info);
        std::cerr << "ERROR::PROGRAM LINK_FAILED\n" << info;
        std::cerr << vertexSource << std::endl;
        std::cerr << fragmentSource << std::endl;

        glDeleteProgram(program);
        glfwTerminate();
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(1280, 720, "OpenGL", NULL, NULL);

    if (window == NULL) {
        std::cerr << "Can not create glfw window! \n";
        glfwTerminate();
        return -1;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);
    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing glew. Exiting.\n";
        return -1;
    }
#endif

    auto program = buildProgram("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
    auto programCircle = buildProgram("resources/shaders/vertex_circle.glsl", "resources/shaders/fragment_circle.glsl");

    GLfloat bodyData[] = {
            0.0f, 0.4f, 0.3f, 0.4f, 1.0f,
            -0.3f, -0.4f, 0.3f, 0.4f, 1.0f,
            0.3f, -0.4f, 0.3f, 0.4f, 1.0f
    };

    GLuint bodyIndexes[] = {
            1, 2, 0
    };

    GLfloat armsLegsData[] = {
            // Left arm
            -0.038f, 0.3f, 1.0f, 0.95f, 0.55f,
            -0.18f, 0.25f, 1.0f, 0.95f, 0.55f,
            -0.055f, 0.25f, 1.0f, 0.95f, 0.55f,
            -0.18f, 0.3f, 1.0f, 0.95f, 0.55f,
            // Right arm
            0.038f, 0.3f, 1.0f, 0.95f, 0.55f,
            0.18f, 0.25f, 1.0f, 0.95f, 0.55f,
            0.055f, 0.25f, 1.0f, 0.95f, 0.55f,
            0.18f, 0.3f, 1.0f, 0.95f, 0.55f,
            // Left leg
            -0.24f, -0.56f, 1.0f, 0.95f, 0.55f,
            -0.16f, -0.4f, 1.0f, 0.95f, 0.55f,
            -0.24f, -0.4f, 1.0f, 0.95f, 0.55f,
            -0.16f, -0.56f, 1.0f, 0.95f, 0.55f,
            // Right leg
            0.24f, -0.56f, 1.0f, 0.95f, 0.55f,
            0.16f, -0.4f, 1.0f, 0.95f, 0.55f,
            0.24f, -0.4f, 1.0f, 0.95f, 0.55f,
            0.16f, -0.56f, 1.0f, 0.95f, 0.55f,
    };

    GLuint armsLegsIndexes[] = {
            1, 0, 3,
            1, 2, 0,

            6, 7, 4,
            6, 5, 7,

            8, 9, 10,
            8, 11, 9,

            12, 13, 14,
            12, 15, 13
    };

    GLfloat headData[] = {
            0.0f, 0.5f, 1.0f, 0.95f, 0.55f
    };

    GLuint headIndexes[] = {
            0
    };

    GLuint bodyVao = createVao(bodyData, sizeof(bodyData), bodyIndexes, sizeof(bodyIndexes));
    GLuint armsLegsVao = createVao(armsLegsData, sizeof(armsLegsData), armsLegsIndexes, sizeof(armsLegsIndexes));
    GLuint headVao = createVao(headData, sizeof(headData), headIndexes, sizeof(headIndexes));

    glViewport(0, 0, width, height);
    glPointSize(150);

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        glClearColor(0.4f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(program);

        glBindVertexArray(bodyVao);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glBindVertexArray(armsLegsVao);
        glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);

        glUseProgram(programCircle);

        glUniform2f(glGetUniformLocation(programCircle, "uResolution"), width, height);

        glBindVertexArray(headVao);
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::nanoseconds(15000));
    }

    glfwTerminate();
    return 0;
}