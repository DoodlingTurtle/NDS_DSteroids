#include "shipengine.h"

#include <math.h>

void ShipEngine::accelerate(float deltaTime) {
    acceleration = fmin(1, acceleration + (deltaTime * deltaTime));
}
void ShipEngine::decerlerate(float deltaTime) {
    acceleration = fmax(0, acceleration - (deltaTime * deltaTime));
}
