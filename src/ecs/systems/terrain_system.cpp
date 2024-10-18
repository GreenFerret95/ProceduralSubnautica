#include <iostream>  // Include the header file that declares the standard input/output stream objects
#include <random>    // Include the header file that declares random number generation facilities
#include "terrain_system.h"  // Include the header file that declares the class
#include "../../utility/simplex_noise.h"  // Ensure proper inclusion of perlin_noise.h
#include <vector>
#include <glm/glm.hpp>

// Correctly defined function, with qualification, outside of the class scope
std::vector<std::vector<float>> TerrainSystem::generateHeightmap(int width, int depth, float scale) {
    std::vector<std::vector<float>> heightmap(width, std::vector<float>(depth));
    SimplexNoise simplex;

    // Generate random offsets for x and z to avoid repetition
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 10000.0);  // Large random value for seed
    float xOffset = dis(gen);
    float zOffset = dis(gen);

    // Heightmap generation parameters
    float heightScale = 1.5f; // Adjust for height of terrain
    int octaves = 6;  // Number of octaves
    float persistence = 0.5f; // Amplitude reduction per octave
    float lacunarity = 2.0f;  // Frequency increase per octave

    for (int x = 0; x < width; ++x) {
        for (int z = 0; z < depth; ++z) {
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float height = 0.0f;

            // Randomized domain warping
            float totalX = x + xOffset;
            float totalZ = z + zOffset;

            for (int i = 0; i < octaves; ++i) {
                // Apply domain warping for more varied terrain
                float warpX = simplex.noise(totalX * scale * frequency, totalZ * scale * frequency, 0.0f);
                float warpZ = simplex.noise(totalZ * scale * frequency, totalX * scale * frequency, 0.0f);

                float nx = (totalX + warpX * 15.0f) * scale * frequency;
                float nz = (totalZ + warpZ * 15.0f) * scale * frequency;

                // Get the noise value
                height += simplex.noise(nx, nz, 0.0f) * amplitude;

                // Adjust amplitude and frequency for next octave
                amplitude *= persistence;
                frequency *= lacunarity;
            }

            // Apply height scaling
            heightmap[x][z] = height * heightScale;
        }
    }

    return heightmap;
}


// Define the other functions of TerrainSystem
std::vector<glm::vec3> TerrainSystem::generateVertices(const std::vector<std::vector<float>>& heightmap) {
    std::vector<glm::vec3> vertices;
    for (int x = 0; x < heightmap.size(); ++x) {
        for (int z = 0; z < heightmap[x].size(); ++z) {
            vertices.emplace_back(x, heightmap[x][z], z); // y component from heightmap
        }
    }
    return vertices;
}


std::vector<unsigned int> TerrainSystem::generateIndices(int width, int depth) {
    std::vector<unsigned int> indices;
    for (int x = 0; x < width - 1; ++x) {
        for (int z = 0; z < depth - 1; ++z) {
            int topLeft = x * depth + z;
            int topRight = (x + 1) * depth + z;
            int bottomLeft = x * depth + (z + 1);
            int bottomRight = (x + 1) * depth + (z + 1);

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }
    return indices;
}

std::vector<glm::vec3> TerrainSystem::calculateNormals(const std::vector<glm::vec3>& vertices, const std::vector<unsigned int>& indices) {
    std::vector<glm::vec3> normals(vertices.size(), glm::vec3(0.0f, 0.0f, 0.0f));

    for (size_t i = 0; i < indices.size(); i += 3) {
        unsigned int i0 = indices[i];
        unsigned int i1 = indices[i + 1];
        unsigned int i2 = indices[i + 2];

        glm::vec3 v0 = vertices[i0];
        glm::vec3 v1 = vertices[i1];
        glm::vec3 v2 = vertices[i2];

        // Calculate the normal for this face
        glm::vec3 edge1 = v1 - v0;
        glm::vec3 edge2 = v2 - v0;
        glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));

        // Accumulate the normal for each vertex
        normals[i0] += normal;
        normals[i1] += normal;
        normals[i2] += normal;
    }

    // Normalize all accumulated normals
    for (auto& normal : normals) {
        normal = glm::normalize(normal);
    }

    return normals;
}

