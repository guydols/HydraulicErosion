#include "HydraulicErosion.h"

HydraulicErosion::HydraulicErosion(HydraulicErosionConfig eroderConfig) {
    this->config = eroderConfig;

}

HydraulicErosion::~HydraulicErosion(){

}

void HydraulicErosion::Simulate(std::vector<std::vector<float>> &map) {

//  seed the random number generator
    std::srand(this->config.seed);

//  calculate the max value for both axis of the heightmap
    float xMax = this->config.mapWidth - 1.0;
    float yMax = this->config.mapHeight - 1.0;

    for (int iteration = 0; iteration < this->config.numIterations; iteration++) {

        Droplet droplet; // Make new droplet, with random position
        droplet.position.x = (xMax - this->config.erosionRadius) +
                             (std::rand() / (RAND_MAX / ((0 + this->config.erosionRadius) -
                             (xMax - this->config.erosionRadius))));
        droplet.position.y = (yMax - this->config.erosionRadius) +
                             (std::rand() / (RAND_MAX / ((0 + this->config.erosionRadius) -
                             (yMax - this->config.erosionRadius))));
        droplet.direction.x = 0.0;
        droplet.direction.y = 0.0;
        droplet.velocity = this->config.initialVelocity;
        droplet.water = this->config.initialWaterVolume;
        droplet.sediment = 0.0;

        for (int lifetime = 0; lifetime < this->config.maxDropletLifetime; lifetime++) {

            HeightAndGradient heightAndGradient;

            // Cast position floats to ints
            droplet.node.x = static_cast<int>(droplet.position.x);
            droplet.node.y = static_cast<int>(droplet.position.y);

            // Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
            droplet.cellOffset.x = droplet.position.x - droplet.node.x;
            droplet.cellOffset.y = droplet.position.y - droplet.node.y;

            // Calculate droplet's height and direction of flow with bilinear interpolation of surrounding heights
            heightAndGradient = this->CalculateHeightAndGradient(map, droplet);

            // Update the droplet's direction and position (move position 1 unit regardless of speed)
            droplet.direction.x = (droplet.direction.x * this->config.inertia -
                                   heightAndGradient.gradient.x * (1 - this->config.inertia));
            droplet.direction.y = (droplet.direction.y * this->config.inertia -
                                   heightAndGradient.gradient.y * (1 - this->config.inertia));

            // Normalize direction
            float len = std::sqrt(droplet.direction.x * droplet.direction.x +
                                  droplet.direction.y * droplet.direction.y);
            if (len != 0) {
                droplet.direction.x /= len;
                droplet.direction.y /= len;
            }
            droplet.position.x += droplet.direction.x;
            droplet.position.y += droplet.direction.y;

            // Stop simulating droplet if it's not moving or has flowed over edge of map
            if ((droplet.direction.x == 0 && droplet.direction.y == 0) ||
            droplet.position.x < 0 ||
            droplet.position.x >= xMax ||
            droplet.position.y < 0 ||
            droplet.position.y >= yMax ||
            droplet.node.x < 0 ||
            droplet.node.y < 0) {
                break;
            }

            // Find the droplet's new height and calculate the deltaHeight
            float newHeight = this->CalculateHeightAndGradient(map, droplet).height;
            float oldHeight = heightAndGradient.height;
            float deltaHeight = newHeight - oldHeight;

            // Calculate the droplet's sediment capacity (higher when moving fast down a slope and contains lots of water)
            float sedimentCapacity = std::max(-deltaHeight * droplet.velocity * droplet.water * this->config.sedimentCapacityFactor, this->config.minSedimentCapacity);

            // If carrying more sediment than capacity, or if flowing uphill:
            if (droplet.sediment > sedimentCapacity || deltaHeight > 0) {
                this->Deposit(map, droplet, deltaHeight, sedimentCapacity);
            } else {
                this->Erode(map, droplet, deltaHeight, sedimentCapacity);
            }

            // Update droplet's speed and water content
            droplet.velocity = std::sqrt(std::abs(droplet.velocity * droplet.velocity + deltaHeight * this->config.gravity));
            droplet.water *= (1 - this->config.evaporateSpeed);

        }
    }
}

HeightAndGradient HydraulicErosion::CalculateHeightAndGradient(std::vector<std::vector<float>> &map, Droplet &droplet) {

    // Calculate droplet's offset inside the cell (0,0) = at NW node, (1,1) = at SE node
    float x = droplet.position.x - droplet.node.x;
    float y = droplet.position.y - droplet.node.y;

    // Get heights of the four nodes of this cell
    float heightNW = map[droplet.node.y][droplet.node.x];
    float heightNE = map[droplet.node.y][droplet.node.x + 1];
    float heightSW = map[droplet.node.y + 1][droplet.node.x];
    float heightSE = map[droplet.node.y + 1][droplet.node.x + 1];

    // Calculate droplet's direction of flow with bilinear interpolation of height difference along the edges
    float gradientX = (heightNE - heightNW) * (1 - y) +
                      (heightSE - heightSW) * y;
    float gradientY = (heightSW - heightNW) * (1 - x) +
                      (heightSE - heightNE) * x;

    // Calculate height with bilinear interpolation of the heights of the nodes of the cell
    float height = heightNW * (1 - x) * (1 - y) + heightNE * x * (1 - y) + heightSW * (1 - x) * y + heightSE * x * y;

    HeightAndGradient returnVar;
    returnVar.height = height;
    returnVar.gradient.x = gradientX;
    returnVar.gradient.y = gradientY;
    return returnVar;
}

void HydraulicErosion::Deposit(std::vector<std::vector<float>> &map, Droplet &droplet, float deltaHeight, float sedimentCapacity) {

    // If moving uphill (deltaHeight > 0) try fill up to the current height, otherwise deposit a fraction of the excess sediment
    float amountToDeposit = (deltaHeight > 0) ? std::min(deltaHeight, droplet.sediment) : (droplet.sediment - sedimentCapacity) * this->config.depositSpeed;
    droplet.sediment -= amountToDeposit;

    // Add the sediment to the four nodes of the current cell using bilinear interpolation
    // Deposition is not distributed over a radius (like erosion) so that it can fill small pits
    map[droplet.node.y][droplet.node.x] += amountToDeposit * (1 - droplet.cellOffset.x) * (1 - droplet.cellOffset.y);
    map[droplet.node.y][droplet.node.x + 1] += amountToDeposit * droplet.cellOffset.x * (1 - droplet.cellOffset.y);
    map[droplet.node.y + 1][droplet.node.x] += amountToDeposit * (1 - droplet.cellOffset.x) * droplet.cellOffset.y;
    map[droplet.node.y + 1][droplet.node.x + 1] += amountToDeposit * droplet.cellOffset.x * droplet.cellOffset.y;

}

void HydraulicErosion::Erode(std::vector<std::vector<float>> &map, Droplet &droplet, float deltaHeight, float sedimentCapacity) {

    // Erode a fraction of the droplet's current carry capacity.
    // Clamp the erosion to the change in height so that it doesn't dig a hole in the terrain behind the droplet
    float amountToErode = std::min((sedimentCapacity - droplet.sediment) * this->config.erodeSpeed, -deltaHeight);

    // Create a new brush for the erosion process
    int radius = this->config.erosionRadius;
    int brushSize = radius * 2 + 1;

    Brush brush;
    brush.weightSum = 0;
    Tool::resize2DVector(brush.weights, brushSize, brushSize);

    brush.start.x = std::max(0, (int)droplet.position.x - radius);
    brush.start.y = std::max(0, (int)droplet.position.y - radius);
    brush.end.x = std::min(this->config.mapWidth - 1, (int)droplet.position.x + radius);
    brush.end.y = std::min(this->config.mapHeight - 1, (int)droplet.position.y + radius);

    // Calculate the weights of each point on the brush
    for (int y = brush.start.y; y <= brush.end.y; y++) {
        for (int x = brush.start.x; x <= brush.end.x; x++) {
            float sqrDst = (x - (int)droplet.position.x) * (x - (int)droplet.position.x) +
                           (y - (int)droplet.position.y) * (y - (int)droplet.position.y);
            if (sqrDst <= radius * radius) {
                float weight = 1 - std::sqrt(sqrDst) / radius;
                brush.weightSum += weight;
                brush.weights[x - brush.start.x][y - brush.start.y] = weight;
            }
        }
    }

    // Erode the map proporsional to the weights in the brush
    for (int y = brush.start.y; y <= brush.end.y; y++) {
        for (int x = brush.start.x; x <= brush.end.x; x++) {
            brush.weights[x - brush.start.x][y - brush.start.y] /= brush.weightSum;
            float weighedErodeAmount = amountToErode * brush.weights[x - brush.start.x][y - brush.start.y];
            float deltaSediment = (map[x][y] < weighedErodeAmount) ? map[x][y] : weighedErodeAmount;
            map[x][y] -= deltaSediment;
            droplet.sediment += deltaSediment;
        }
    }

}

