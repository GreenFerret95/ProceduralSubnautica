#include "./window/window.h"
#include "./ecs/components/camera_component.h"
#include "./ecs/components/transform_component.h"
#include "./ecs/systems/input_system.h"
#include "./ecs/systems/camera_system.h"
#include "./ecs/systems/render_system.h"
#include "./ecs/systems/terrain_system.h"
#include "./ecs/components/terrain_component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

int main() {
    Window window(800, 600, "OpenGL ECS Engine");

    // Initialize window
    if (!window.init()) {
        return -1;
    }

    // Initialize render system
    RenderSystem renderSystem;
    renderSystem.init();  // Initialize OpenGL and GLEW

    // Initialize camera and input systems
    CameraComponent camera = {0.0f, 0.0f, 3.0f, 0.0f, 0.0f, 0.05f};
    TransformComponent transform = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
    InputSystem inputSystem;
    CameraSystem cameraSystem;

    // Initialize terrain system
    TerrainSystem terrainSystem;
    TerrainComponent terrain;

    // Generate heightmap, vertices, and indices for terrain
    terrain.heightmap = terrainSystem.generateHeightmap(100, 100, 0.1f);
    terrain.vertices = terrainSystem.generateVertices(terrain.heightmap);
    terrain.indices = terrainSystem.generateIndices(100, 100);

    std::vector<glm::vec3> normals = terrainSystem.calculateNormals(terrain.vertices, terrain.indices);

    renderSystem.setTerrainData(terrain.vertices, normals, terrain.indices);    
    
    // Main loop
    while (!window.shouldClose()) {
        window.clear();

        // Process input
        inputSystem.processInput(window.getGLFWwindow(), camera, transform);

        // Update camera
        glm::mat4 view = cameraSystem.getViewMatrix(camera);

        // Render the triangle with the updated view matrix
        renderSystem.render(view);

        // Swap buffers and poll for events
        window.update();
    }

    // Cleanup resources
    renderSystem.cleanup();
    window.cleanup();

    return 0;
}