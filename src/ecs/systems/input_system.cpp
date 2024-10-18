#include "input_system.h"
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void InputSystem::processInput(GLFWwindow* window, CameraComponent& camera, TransformComponent& transform) {
    // Calculate the front vector based on yaw and pitch for accurate movement direction
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front = glm::normalize(front);

    glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));  // Right direction
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);  // Up direction

    // Define base camera speed
    float baseSpeed = camera.speed * 0.1f; // Adjusted for a more natural walking pace
    float velocity = baseSpeed;

    // Increase speed when holding Shift
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        velocity *= 2.5f;  // Increase speed multiplier
    }

    // Process input for WASD movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.posX += front.x * velocity, camera.posY += front.y * velocity, camera.posZ += front.z * velocity;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.posX -= front.x * velocity, camera.posY -= front.y * velocity, camera.posZ -= front.z * velocity;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.posX -= right.x * velocity, camera.posY -= right.y * velocity, camera.posZ -= right.z * velocity;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.posX += right.x * velocity, camera.posY += right.y * velocity, camera.posZ += right.z * velocity;

    // Process input for elevation movement
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.posY += velocity;  // Move upwards when Space is pressed
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
        camera.posY -= velocity;  // Move downwards when Ctrl is pressed
    }

    // Process input for mouse movement
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    static float lastX = 400.0f, lastY = 300.0f; // Assuming the window initially centers the cursor
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Reversed since y-coordinates range from bottom to top

    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.1f; // Sensitivity multiplier for smooth mouse movement
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    camera.yaw += xoffset;
    camera.pitch += yoffset;

    // Constrain pitch to prevent flipping
    if (camera.pitch > 89.0f)
        camera.pitch = 89.0f;
    if (camera.pitch < -89.0f)
        camera.pitch = -89.0f;
}
