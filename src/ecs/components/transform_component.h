#pragma once

class TransformComponent {
public:
    TransformComponent(float posX = 0.0f, float posY = 0.0f, float posZ = 0.0f,
                       float rotationX = 0.0f, float rotationY = 0.0f, float rotationZ = 0.0f)
        : posX(posX), posY(posY), posZ(posZ), rotationX(rotationX), rotationY(rotationY), rotationZ(rotationZ) {}

    float posX, posY, posZ;          // Position
    float rotationX, rotationY, rotationZ; // Rotation
};
