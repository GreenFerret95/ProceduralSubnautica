#include "window.h"
#include <iostream>

Window::Window(int width, int height, const char* title)
    : width(width), height(height), title(title), window(nullptr) {}

bool Window::init() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return false;
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

void Window::clear() {
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::update() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

bool Window::shouldClose() {
    return glfwWindowShouldClose(window);
}

void Window::cleanup() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Window::getGLFWwindow() const {
    return window;
}
