#pragma once

#include "../components/camera_component.h"
#include "../components/transform_component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraSystem {
public:
    glm::mat4 getViewMatrix(const CameraComponent& camera);
    
};
