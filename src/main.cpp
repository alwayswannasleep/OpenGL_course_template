#include <glwrapper.h>
#include "glfw3.h"
#include <thread>
#include <iostream>

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

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glfwSwapBuffers(window);
        std::this_thread::sleep_for(std::chrono::nanoseconds(15000));
    }

    glfwTerminate();
    return 0;
}