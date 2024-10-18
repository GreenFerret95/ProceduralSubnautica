#pragma once

#include <vector>       // Include the vector library
#include <glm/glm.hpp>  // Include GLM to use glm::mat4

class RenderSystem {
public:
    void init();     // Initializes OpenGL and GLEW
    void render(const glm::mat4& viewMatrix);   // Renders the scene with a view matrix  
    void setTerrainData(const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<unsigned int>& indices);  
    void cleanup();  // Cleans up resources

private:
    unsigned int VAO, VBO, EBO, normalVBO;  // Declare VAO, VBO, EBO, and normal VBO
    unsigned int shaderProgram;
    std::vector<glm::vec3> terrainVertices;
    std::vector<glm::vec3> terrainNormals;
    std::vector<unsigned int> terrainIndices;
};
