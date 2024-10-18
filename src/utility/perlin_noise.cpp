#include "perlin_noise.h"
#include <cmath>
#include <vector>

// Gradients for 3D Perlin noise
static const int grad3[12][3] = {
    {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
    {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
    {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
};

// Permutation table
static const int perm[512] = {
    151,160,137,91,90,15, // Skipping some values here for brevity
    // Repeat to extend the hash lookup
    151,160,137,91,90,15 // ... and repeated
};

static int hash(int i) {
    return perm[i & 255];
}

// Dot product for the gradients
static float dot(const int* g, float x, float y, float z) {
    return g[0] * x + g[1] * y + g[2] * z;
}

// Fade function to ease coordinate values
static float fade(float t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

// Linear interpolation function
static float lerp(float t, float a, float b) {
    return a + t * (b - a);
}

float PerlinNoise::noise(float x, float y, float z) {
    // Calculate unit cube containing point
    int X = static_cast<int>(floor(x)) & 255;
    int Y = static_cast<int>(floor(y)) & 255;
    int Z = static_cast<int>(floor(z)) & 255;

    // Calculate relative x, y, z in cube
    x -= floor(x);
    y -= floor(y);
    z -= floor(z);

    // Compute fade curves for x, y, z
    float u = fade(x);
    float v = fade(y);
    float w = fade(z);

    // Hash coordinates of the 8 cube corners
    int aaa = hash(X    + hash(Y    + hash(Z   )));
    int aba = hash(X    + hash(Y + 1 + hash(Z   )));
    int aab = hash(X    + hash(Y    + hash(Z + 1)));
    int abb = hash(X    + hash(Y + 1 + hash(Z + 1)));
    int baa = hash(X + 1 + hash(Y    + hash(Z   )));
    int bba = hash(X + 1 + hash(Y + 1 + hash(Z   )));
    int bab = hash(X + 1 + hash(Y    + hash(Z + 1)));
    int bbb = hash(X + 1 + hash(Y + 1 + hash(Z + 1)));

    // And add blended results from 8 corners of the cube
    float x1, x2, y1, y2;
    x1 = lerp(u, dot(grad3[aaa % 12], x, y, z), dot(grad3[baa % 12], x - 1, y, z));
    x2 = lerp(u, dot(grad3[aba % 12], x, y - 1, z), dot(grad3[bba % 12], x - 1, y - 1, z));
    y1 = lerp(v, x1, x2);

    x1 = lerp(u, dot(grad3[aab % 12], x, y, z - 1), dot(grad3[bab % 12], x - 1, y, z - 1));
    x2 = lerp(u, dot(grad3[abb % 12], x, y - 1, z - 1), dot(grad3[bbb % 12], x - 1, y - 1, z - 1));
    y2 = lerp(v, x1, x2);

    // Finally, blend along z axis
    return (lerp(w, y1, y2) + 1.0f) / 2.0f; // Normalize to the range [0, 1]
}
