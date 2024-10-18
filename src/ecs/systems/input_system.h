#pragma once

#include "../components/camera_component.h"
#include "../components/transform_component.h"
#include <GLFW/glfw3.h>

class InputSystem {
public:
    void processInput(GLFWwindow* window, CameraComponent& camera, TransformComponent& transform);
};

