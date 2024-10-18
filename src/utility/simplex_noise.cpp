#include "simplex_noise.h"
#include <cmath>

// Gradient table
static const int grad3[12][3] = {
    {1,1,0}, {-1,1,0}, {1,-1,0}, {-1,-1,0},
    {1,0,1}, {-1,0,1}, {1,0,-1}, {-1,0,-1},
    {0,1,1}, {0,-1,1}, {0,1,-1}, {0,-1,-1}
};

// Permutation table (a 512 element copy of a 256 element permutation table)
static const int perm[512] = {
    151,160,137,91,90,15, // shortened for brevity, but it repeats
    151,160,137,91,90,15 // and repeated to fill all 512 values
};

int SimplexNoise::fastfloor(float x) {
    return (x > 0) ? (int)x : (int)x - 1;
}

float SimplexNoise::dot(const int* g, float x, float y, float z) {
    return g[0] * x + g[1] * y + g[2] * z;
}

float SimplexNoise::noise(float x, float y, float z) {
    // Simplex noise algorithm starts here

    // Skewing and unskewing factors for 3D
    static const float F3 = 1.0f / 3.0f;
    static const float G3 = 1.0f / 6.0f;

    // Skew the input space to determine which simplex cell we're in
    float s = (x + y + z) * F3; // Hairy factor for 3D
    int i = fastfloor(x + s);
    int j = fastfloor(y + s);
    int k = fastfloor(z + s);

    // Unskew the cell origin back to (x,y,z) space
    float t = (i + j + k) * G3;
    float X0 = i - t;
    float Y0 = j - t;
    float Z0 = k - t;
    float x0 = x - X0;  // The x, y, z distances from the cell origin
    float y0 = y - Y0;
    float z0 = z - Z0;

    // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
    // Determine which simplex we are in.
    int i1, j1, k1; // Offsets for the second corner of simplex in (i,j,k) coords
    int i2, j2, k2; // Offsets for the third corner of simplex in (i,j,k) coords

    if (x0 >= y0) {
        if (y0 >= z0) {
            i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
        } else if (x0 >= z0) {
            i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1;
        } else {
            i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1;
        }
    } else {
        if (y0 < z0) {
            i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1;
        } else if (x0 < z0) {
            i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1;
        } else {
            i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
        }
    }

    // Offsets for remaining corners in (x,y,z) coordinates
    float x1 = x0 - i1 + G3;
    float y1 = y0 - j1 + G3;
    float z1 = z0 - k1 + G3;
    float x2 = x0 - i2 + 2.0f * G3;
    float y2 = y0 - j2 + 2.0f * G3;
    float z2 = z0 - k2 + 2.0f * G3;
    float x3 = x0 - 1.0f + 3.0f * G3;
    float y3 = y0 - 1.0f + 3.0f * G3;
    float z3 = z0 - 1.0f + 3.0f * G3;

    // Hash the coordinates of the 4 simplex corners
    int gi0 = perm[(i + perm[(j + perm[k & 255]) & 255]) & 255] % 12;
    int gi1 = perm[(i + i1 + perm[(j + j1 + perm[(k + k1) & 255]) & 255]) & 255] % 12;
    int gi2 = perm[(i + i2 + perm[(j + j2 + perm[(k + k2) & 255]) & 255]) & 255] % 12;
    int gi3 = perm[(i + 1 + perm[(j + 1 + perm[(k + 1) & 255]) & 255]) & 255] % 12;

    // Calculate the contribution from the four corners
    float t0 = 0.6f - x0 * x0 - y0 * y0 - z0 * z0;
    float n0 = (t0 < 0) ? 0.0f : pow(t0, 4) * dot(grad3[gi0], x0, y0, z0);

    float t1 = 0.6f - x1 * x1 - y1 * y1 - z1 * z1;
    float n1 = (t1 < 0) ? 0.0f : pow(t1, 4) * dot(grad3[gi1], x1, y1, z1);

    float t2 = 0.6f - x2 * x2 - y2 * y2 - z2 * z2;
    float n2 = (t2 < 0) ? 0.0f : pow(t2, 4) * dot(grad3[gi2], x2, y2, z2);

    float t3 = 0.6f - x3 * x3 - y3 * y3 - z3 * z3;
    float n3 = (t3 < 0) ? 0.0f : pow(t3, 4) * dot(grad3[gi3], x3, y3, z3);

    // Sum up and scale the result to cover the range [0, 1]
    return 32.0f * (n0 + n1 + n2 + n3);
}
