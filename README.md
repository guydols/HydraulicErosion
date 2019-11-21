# HydraulicErosion

This implementation is inspired by [Hans Theobald Beyer](https://www.firespark.de/resources/downloads/implementation%20of%20a%20methode%20for%20hydraulic%20erosion.pdf), [Sebastian Lague](https://www.youtube.com/watch?v=eaXk97ujbPQ) and a few others. I combined all the elements from other designs and ideas to make a version that I was happy with.

* Implemented in c++ (without any outside dependencies)
* Made for 2d maps of floats ( `std::vector<std::vector<float> >` )
* The map can be any width and height (not limited to a square)

## TODO

* Make distribution of droplets configurable (uniform, normal, ...)
* Memory and CPU cycle optimizations
* Design a multi threaded solution (press x to doubt)
