#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "../../utility/perlin_noise.h"
#include "../components/terrain_component.h"

class TerrainSystem {
public:
    std::vector<std::vector<float>> generateHeightmap(int width, int depth, float scale);
    std::vector<glm::vec3> generateVertices(const std::vector<std::vector<float>>& heightmap);
    std::vector<unsigned int> generateIndices(int width, int depth);
    std::vector<glm::vec3> calculateNormals(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices);
};
