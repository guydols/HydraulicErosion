#ifndef HYDRAULIC_EROSION_H
#define HYDRAULIC_EROSION_H

#include <vector>
#include <cmath>
#include <random>

#include "Tool.h"

struct Vec2{
    float x;
    float y;
};

struct Node {
    int x;
    int y;
};

struct HeightAndGradient {
    float height;
    Vec2 gradient;
};

struct Droplet {
    Vec2 position;
    Vec2 direction;
    Node node;
    Vec2 cellOffset;
    float velocity;
    float sediment;
    float water;
};

struct Brush {
    Node start;
    Node end;
    std::vector<std::vector<float>> weights;
    float weightSum;
};

struct HydraulicErosionConfig {
    int seed;
    int numIterations;
    int erosionRadius;
    float inertia;
    float sedimentCapacityFactor;
    float minSedimentCapacity;
    float erodeSpeed;
    float depositSpeed;
    float evaporateSpeed;
    float gravity;
    int maxDropletLifetime;
    float initialWaterVolume;
    float initialVelocity;
    int mapWidth;
    int mapHeight;
};

class HydraulicErosion {

public:
    HydraulicErosion(HydraulicErosionConfig eroderConfig);
    ~HydraulicErosion();
    void Simulate(std::vector<std::vector<float>> &map);

private:
    HydraulicErosionConfig config;
    HeightAndGradient CalculateHeightAndGradient(std::vector<std::vector<float>> &map, Droplet &droplet);
    void Deposit(std::vector<std::vector<float>> &map, Droplet &droplet, float deltaHeight, float sedimentCapacity);
    void Erode(std::vector<std::vector<float>> &map, Droplet &droplet, float deltaHeight, float sedimentCapacity);

};

#endif //HYDRAULIC_EROSION_H