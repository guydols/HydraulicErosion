#include <iostream>

#include "HydraulicErosion.h"
#include "Tool.h"

int SEED = 1337;
int WIDTH = 256;
int HEIGHT = 256;

int main() {

  std::vector<std::vector<float> > map;
  Tool::resize2DVector(map, WIDTH, HEIGHT);

  for (int y = 0; y < HEIGHT; ++y) {
    map[y] =  Tool::randomFloatsBetween(SEED, WIDTH, 0.0, 1.0);
  }

  HydraulicErosionConfig eroderConfig;

  eroderConfig.seed = SEED;
  eroderConfig.numIterations = 10000;
  eroderConfig.erosionRadius = 3;
  eroderConfig.inertia = .3f;
  eroderConfig.sedimentCapacityFactor = 8;
  eroderConfig.minSedimentCapacity = .01f;
  eroderConfig.erodeSpeed = .7f;
  eroderConfig.depositSpeed = .2f;
  eroderConfig.evaporateSpeed = .02f;
  eroderConfig.gravity = 10;
  eroderConfig.maxDropletLifetime = 64;
  eroderConfig.initialWaterVolume = 1;
  eroderConfig.initialVelocity = 1;
  eroderConfig.mapWidth = WIDTH;
  eroderConfig.mapHeight = HEIGHT;

  HydraulicErosion eroder = HydraulicErosion(eroderConfig);
  eroder.Simulate(map);

  map = Tool::shift2DVector(map, 0.0, 1.0);

;}