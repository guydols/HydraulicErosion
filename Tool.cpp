#include "Tool.h"

std::vector<std::vector<float>> Tool::shift2DVector(std::vector<std::vector<float>> &r1, float r2min, float r2max) {

    std::vector<std::vector<float>> r2;
    float r1min = std::numeric_limits<float>::max();
    float r1max = std::numeric_limits<float>::min();

    // Find min/max of source vector
    for (long unsigned int x = 0; x < r1.size(); ++x) {
        for (long unsigned int y = 0; y < r1[x].size(); ++y) {
            if (r1[x][y] < r1min) {
                r1min = r1[x][y];
            }
            if (r1[x][y] > r1max) {
                r1max = r1[x][y];
            }
        }
    }

    // resize destination vector
    r2.resize(r1.size());
    for (long unsigned int x = 0; x < r2.size(); ++x) {
        r2[x].resize(r1[x].size());
    }

    // Calculate the shift of the vector
    for (long unsigned int x = 0; x < r1.size(); ++x) {
        for (long unsigned int y = 0; y < r1[x].size(); ++y) {
            r2[x][y] = r2min + (( (r2max - r2min) / (r1max - r1min) ) * (r1[x][y] - r1min));
        }
    }

    return r2;
}

void Tool::resize2DVector(std::vector<std::vector<float>> &vector, int xSize, int ySize) {
    vector.resize(xSize);
    for (int x = 0; x < xSize; ++x) {
        vector[x].resize(ySize);
    }
}

std::vector<float> Tool::convert2DVectorToVector(std::vector<std::vector<float>> &map) {
    std::vector<float> newMap;
    for (long unsigned int y = 0; y < map[0].size(); ++y) {
        for (long unsigned int x = 0; x < map.size(); ++x) {
            newMap.push_back(map[x][y]);
        }
    }
    return newMap;
}

std::vector<std::vector<float>> Tool::convertVectorTo2DVector(std::vector<float> &map, int ySize) {
    std::vector<std::vector<float>> newMap;
    int xSize = map.size() / ySize;
    Tool::resize2DVector(newMap, xSize, ySize);
    int index = 0;
    for (int y = 0; y < ySize; ++y) {
        for (int x = 0; x < xSize; ++x) {
            newMap[x][y] = map[index];
            index++;
        }
    }
    return newMap;
}

int Tool::randomIntBetween(int pSeed, int pMin, int pMax) {
    std::srand(pSeed);
    int randomInt = std::rand() % pMax + pMin;
    return randomInt;
}

std::vector<int> Tool::randomIntsBetween(int pSeed, int pNumber, int pMin, int pMax) {
    std::vector<int> randomInts;
    std::srand(pSeed);
    for (int i = 0; i < pNumber; ++i) {
        int randomInt = std::rand() % pMax + pMin;
        randomInts.push_back(randomInt);
    }
    return randomInts;
}

float Tool::randomFloatBetween(int pSeed, float pMin, float pMax) {
    std::srand(pSeed);
    float randomFloat = pMax + (std::rand() / (RAND_MAX / (pMin - pMax)));
    return randomFloat;
}

std::vector<float> Tool::randomFloatsBetween(int pSeed, int pNumber, float pMin, float pMax) {
    std::vector<float> randomFloats;
    std::srand(pSeed);
    for (int i = 0; i < pNumber; ++i) {
        float randomFloat = pMax + (std::rand() / (RAND_MAX / (pMin - pMax)));
        randomFloats.push_back(randomFloat);
    }
    return randomFloats;
}

