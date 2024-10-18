#include "camera_system.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 CameraSystem::getViewMatrix(const CameraComponent& camera) {
    glm::vec3 position(camera.posX, camera.posY, camera.posZ);
    
    // Calculate front vector based on yaw and pitch
    glm::vec3 front;
    front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front.y = sin(glm::radians(camera.pitch));
    front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
    front = glm::normalize(front);

    glm::vec3 up(0.0f, 1.0f, 0.0f);  // Up direction
    
    return glm::lookAt(position, position + front, up);
}
