#ifndef ESPERATTO_MOVEMENT
#define ESPERATTO_CAMERA
#include "../../Node/Node.h"

namespace Esperatto
{
    class RateOfChange
    {
    private:
        shared_ptr<RateOfChange> dynamicRateOfChange;
        double flatRateOfChange;

    public:
        RateOfChange(double rateOfChange);
        RateOfChange(RateOfChange &rateOfChange);
        function<double(double time)> integrate(int startingPower = 0);
        double getResult(double time);
    };

    class Physics
    {
    private:
        double lastTimestamp;

    public:
        double xVelocity = 0;
        double yVelocity = 0;
        double xAcceleration = 0;
        double yAcceleration = 0;
        Physics();
        Coordinates getDifference();
    };
}

#endif