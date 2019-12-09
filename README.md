# HydraulicErosion

This implementation is inspired by [Hans Theobald Beyer](https://www.firespark.de/resources/downloads/implementation%20of%20a%20methode%20for%20hydraulic%20erosion.pdf), [Sebastian Lague](https://www.youtube.com/watch?v=eaXk97ujbPQ) and a few others. I combined all the elements from other designs and ideas to make a version that I was happy with.

* Implemented in c++ (without any outside dependencies)
* Made for 2d maps of floats ( `std::vector<std::vector<float> >` )
* The map can be any width and height (not limited to a square)

## Example

Input height map (1024x1024) of simplex noise:
![Input for erosion process](https://github.com/guydols/HydraulicErosion/blob/master/img/input.png)

Output height map with erosion config (32768 droplets of rain):

```
seed = 1337;
numIterations = 32768;
erosionRadius = 4;
inertia = .3f;
sedimentCapacityFactor = 4;
minSedimentCapacity = .01f;
erodeSpeed = .8f;
depositSpeed = .8f;
evaporateSpeed = .01f;
gravity = 10;
maxDropletLifetime = 128;
initialWaterVolume = 1;
initialVelocity = 1;
mapWidth = 1024;
mapHeight = 1024;
```

![Output of erosion process](https://github.com/guydols/HydraulicErosion/blob/master/img/output.png)

## TODO

* ~~Make distribution of droplets configurable (uniform, normal, ...)~~
* Memory and CPU cycle optimizations
* Optimize erosion brush (unnecessary building of brush)
