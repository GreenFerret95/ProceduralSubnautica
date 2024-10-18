
// src/ecs/components/terrain_component.h
#pragma once
#include <vector>
#include <glm/glm.hpp>

class TerrainComponent {
public:
    std::vector<std::vector<float>> heightmap;
    std::vector<glm::vec3> vertices;
    std::vector<unsigned int> indices;
};
