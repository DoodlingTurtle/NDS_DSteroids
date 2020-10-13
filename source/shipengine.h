#ifndef SHIPENGINE_H
#define SHIPENGINE_H


class ShipEngine
{
    public:
        float acceleration;

        void accelerate(float deltaTime);
        void decerlerate(float deltaTime);
};

#endif // SHIPENGINE_H
