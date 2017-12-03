#include <glwrapper.h>
#include "glfw3.h"
#include <thread>
#include <iostream>
#include "ShadersLoader.h"
#include "Camera.h"
#include <stb_image.h>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLfloat lastCursorX;
GLfloat lastCursorY;
GLfloat deltaTime;
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f));

void cursorPositionCallback(GLFWwindow *window, double x, double y) {
    auto xDelta = static_cast<GLfloat>(x - lastCursorX);
    auto yDelta = static_cast<GLfloat>(lastCursorY - y);

    lastCursorX = static_cast<GLfloat>(x);
    lastCursorY = static_cast<GLfloat>(y);

    camera.rotateCamera(xDelta, yDelta);
}

void processInput(GLFWwindow *window) {

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.moveCamera(FORWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.moveCamera(BACKWARD, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.moveCamera(LEFT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.moveCamera(RIGHT, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.moveCamera(DOWN, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.moveCamera(UP, deltaTime);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window = glfwCreateWindow(1280, 720, "OpenGL", nullptr, nullptr);

    if (window == nullptr) {
        std::cerr << "Can not create glfw window! \n";
        glfwTerminate();
        return -1;
    }

    int width;
    int height;
    glfwGetFramebufferSize(window, &width, &height);

    lastCursorX = width / 2.0f;
    lastCursorY = height / 2.0f;

    glfwSetCursorPos(window, lastCursorX, lastCursorY);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, cursorPositionCallback);

    glfwMakeContextCurrent(window);

#ifndef __APPLE__
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Error initializing glew. Exiting.\n";
        return -1;
    }
#endif

    auto sourceFromFileVertex = shaders::loadShaderSourceFromFile("resources/shaders/vertex.glsl");
    auto vertexShaderCode = sourceFromFileVertex.c_str();
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderCode, nullptr);
    glCompileShader(vertexShader);

    GLint vertexShaderStatus;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
    if (!vertexShaderStatus) {
        char info[512];
        glGetShaderInfoLog(vertexShader, sizeof(info), nullptr, info);

        std::cerr << info << std::endl;
    }

    stbi_set_flip_vertically_on_load(GL_TRUE);

    auto sourceFromFileFragment = shaders::loadShaderSourceFromFile("resources/shaders/fragment.glsl");
    auto fragmentShaderCode = sourceFromFileFragment.c_str();
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderCode, nullptr);
    glCompileShader(fragmentShader);

    GLint fragmentShaderStatus;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fragmentShaderStatus);
    if (!fragmentShaderStatus) {
        char info[512];
        glGetShaderInfoLog(fragmentShader, sizeof(info), nullptr, info);

        std::cerr << info << std::endl;
    }


    auto program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);

    if (!status) {
        char info[512];
        glGetProgramInfoLog(program, sizeof(info), nullptr, info);

        std::cerr << info << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    GLfloat coordinates[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f
    };

    GLuint vbo;
    GLuint vao;

    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(coordinates), coordinates, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void *>(sizeof(GLfloat) * 0));
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void *>(sizeof(GLfloat) * 3));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 8, reinterpret_cast<void *>(sizeof(GLfloat) * 5));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLint textureWidth;
    GLint textureHeight;
    GLint textureChannelsCount;
    auto data = stbi_load("resources/wall.jpg", &textureWidth, &textureHeight, &textureChannelsCount, STBI_rgb);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    GLuint texture2;
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    data = stbi_load("resources/texture.jpeg", &textureWidth, &textureHeight, &textureChannelsCount, STBI_rgb);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    auto projectionMatrix = glm::perspective(glm::radians(45.0f), ((float) width / (float) height), 0.01f, 100.0f);
    glm::mat4 modelMatrix(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 0.0f, -5.0f));

    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        auto startTime = glfwGetTime();

        glfwPollEvents();

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const glm::mat4 tmp = glm::rotate(modelMatrix, (float) (glm::radians(30.0f) * glfwGetTime()), glm::vec3(0.4f, 0.4f, 1.0f));

        glUseProgram(program);

        glUniformMatrix4fv(glGetUniformLocation(program, "projectionMatrix"), 1, GL_FALSE,
                           glm::value_ptr(projectionMatrix));
        glUniformMatrix4fv(glGetUniformLocation(program, "viewMatrix"), 1, GL_FALSE,
                           glm::value_ptr(camera.getViewMatrix()));
        glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_FALSE, glm::value_ptr(tmp));
        glUniformMatrix4fv(glGetUniformLocation(program, "inverseModelMatrix"), 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(tmp))));
        glUniform3fv(glGetUniformLocation(program, "cameraPosition"), 1, glm::value_ptr(camera.getPosition()));
        glUniform3fv(glGetUniformLocation(program, "cameraDirection"), 1, glm::value_ptr(camera.getDirection()));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUniform1i(glGetUniformLocation(program, "sampler"), 0);
        glUniform1i(glGetUniformLocation(program, "sampler2"), 1);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::nanoseconds(15000));
        deltaTime = static_cast<GLfloat>(glfwGetTime() - startTime);
    }

    glfwTerminate();
    return 0;
}