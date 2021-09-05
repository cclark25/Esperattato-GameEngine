#ifndef ESPERATTO_MOVEMENT
#define ESPERATTO_CAMERA
#include "../../Node/Node.h"

namespace Esperatto
{

    class Physics
    {
        double timeStampOfLastChange;

        double initialX = 0;
        double initialY = 0;
        double initialXVelocity = 0;
        double initialYVelocity = 0;
        double initialXAcceleration = 0;
        double initialYAcceleration = 0;

        void takeSnapshot();
    public:
        Physics();
        Coordinates getDifference();
        void setX(double value);
        void setY(double value);
        void setXVelocity(double value);
        void setYVelocity(double value);
        void setXAcceleration(double value);
        void setYAcceleration(double value);

        void addX(double value);
        void addY(double value);
        void addXVelocity(double value);
        void addYVelocity(double value);
        void addXAcceleration(double value);
        void addYAcceleration(double value);
    };
}

#endif