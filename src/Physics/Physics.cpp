#include "./headers/Physics.h"
#include <allegro5/allegro.h>
#include <cmath>

namespace Esperatto
{
    RateOfChange::RateOfChange(double rateOfChange)
    {
        this->flatRateOfChange = rateOfChange;
    }
    RateOfChange::RateOfChange(RateOfChange &rate)
    {
        if (rate.dynamicRateOfChange == nullptr)
        {
            this->dynamicRateOfChange = shared_ptr<RateOfChange>(new RateOfChange(rate.flatRateOfChange));
        }
        else
        {
            this->dynamicRateOfChange = make_shared<RateOfChange>(rate.flatRateOfChange);
        }
    }

    function<double(double time)> RateOfChange::integrate(int startingPower = 0)
    {
        double &flatRateOfChange = this->flatRateOfChange;
        shared_ptr<RateOfChange> dynamicRateOfChange = this->dynamicRateOfChange;

        return [&flatRateOfChange, dynamicRateOfChange, startingPower](double time)
        {
            if (dynamicRateOfChange == nullptr)
            {
                return flatRateOfChange * time;
            }
            else
            {
                return dynamicRateOfChange->integrate(startingPower + 1)(time) * time / (startingPower);
            }
        };
    }

    double RateOfChange::getResult(double time)
    {
        if (this->dynamicRateOfChange == nullptr)
        {
            return this->flatRateOfChange * time;
        }
        else
        {
            return this->dynamicRateOfChange->getResult(time) * (time) / 2;
        }
    }

    Physics::Physics()
    {
        this->lastTimestamp = al_get_time();
    };

    Coordinates Physics::getDifference()
    {
        double currentTimestamp = al_get_time();
        double timeDifference = currentTimestamp - this->lastTimestamp;
        this->lastTimestamp = currentTimestamp;

        return {this->xVelocity * timeDifference, this->yVelocity * timeDifference};
    }
}