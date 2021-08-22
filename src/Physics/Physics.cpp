#include "./headers/Physics.h"
#include <allegro5/allegro.h>
#include <cmath>

namespace Esperatto
{
    Physics::Physics()
    {
        this->timeStampOfLastChange = al_get_time();
    }

    Coordinates Physics::getDifference()
    {
        double currentTime = al_get_time();
        double timePeriod = currentTime - this->timeStampOfLastChange;

        return {
            initialX + initialXVelocity * timePeriod + initialXAcceleration * (timePeriod * timePeriod) / 2,
            initialY + initialYVelocity * timePeriod + initialYAcceleration * (timePeriod * timePeriod) / 2};
    }

    void Physics::takeSnapshot()
    {
        double currentTime = al_get_time();
        double timePeriod = currentTime - this->timeStampOfLastChange;
        this->timeStampOfLastChange = currentTime;

        this->initialX = initialX + initialXVelocity * timePeriod + initialXAcceleration * (timePeriod * timePeriod) / 2;
        this->initialY = initialY + initialYVelocity * timePeriod + initialYAcceleration * (timePeriod * timePeriod) / 2;

        this->initialXVelocity = initialXVelocity + initialXAcceleration * timePeriod;
        this->initialYVelocity = initialYVelocity + initialYAcceleration * timePeriod;
    }

    void Physics::setX(double value)
    {
        this->takeSnapshot();
        this->initialX = value;
    }
    void Physics::setY(double value)
    {
        this->takeSnapshot();
        this->initialY = value;
    }
    void Physics::setXVelocity(double value)
    {
        this->takeSnapshot();
        this->initialXVelocity = value;
    }
    void Physics::setYVelocity(double value)
    {
        this->takeSnapshot();
        this->initialYVelocity = value;
    }
    void Physics::setXAcceleration(double value)
    {
        this->takeSnapshot();
        this->initialXAcceleration = value;
    }
    void Physics::setYAcceleration(double value)
    {
        this->takeSnapshot();
        this->initialYAcceleration = value;
    }

    void Physics::addX(double value){
        this->takeSnapshot();
        this->initialX += value;
    }
    void Physics::addY(double value){
        this->takeSnapshot();
        this->initialY += value;
    }
    void Physics::addXVelocity(double value){
        this->takeSnapshot();
        this->initialXVelocity += value;
    }
    void Physics::addYVelocity(double value){
        this->takeSnapshot();
        this->initialYVelocity += value;
    }
    void Physics::addXAcceleration(double value){
        this->takeSnapshot();
        this->initialXAcceleration += value;
    }
    void Physics::addYAcceleration(double value){
        this->takeSnapshot();
        this->initialYAcceleration += value;
    }
}