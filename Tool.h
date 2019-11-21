//
// Created by guy on 9/22/19.
//

#ifndef WORLDGENERATOR_VECTORTOOL_H
#define WORLDGENERATOR_VECTORTOOL_H

#include <vector>
#include <limits>
#include <random>

class Tool {
public:
    static void resize2DVector(std::vector<std::vector<float>> &vector, int xSize, int ySize);
    static std::vector<std::vector<float>> shift2DVector(std::vector<std::vector<float>> &r1, float r2min, float r2max);

    static std::vector<float> convert2DVectorToVector(std::vector<std::vector<float>> &map);
    static std::vector<std::vector<float>> convertVectorTo2DVector(std::vector<float> &map, int ySize);

    static int randomIntBetween(int seed, int min, int max);
    static std::vector<int> randomIntsBetween(int pSeed, int pNumber, int pMin, int pMax);

    static float randomFloatBetween(int pSeed, float pMin, float pMax);
    static std::vector<float> randomFloatsBetween(int pSeed, int pNumber, float pMin, float pMax);
};


#endif //WORLDGENERATOR_VECTORTOOL_H
