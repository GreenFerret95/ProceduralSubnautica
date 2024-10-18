#pragma once

class SimplexNoise {
public:
    float noise(float x, float y, float z);

private:
    int fastfloor(float x);
    float dot(const int* g, float x, float y, float z);
};
