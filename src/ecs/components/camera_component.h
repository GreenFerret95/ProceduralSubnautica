#pragma once

class CameraComponent {
public:
    CameraComponent(float posX = 0.0f, float posY = 0.0f, float posZ = 3.0f,
                    float yaw = -90.0f, float pitch = 0.0f, float speed = 2.5f)
        : posX(posX), posY(posY), posZ(posZ), yaw(yaw), pitch(pitch), speed(speed) {}

    float posX, posY, posZ; // Position
    float yaw, pitch;       // Orientation
    float speed;            // Movement speed
};
