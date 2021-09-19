#include "./pointer-tracker.h"

namespace Esperatto
{

    static map<TrackingKey, bool> trackingMap;
    static unsigned int internalCount;

    PointerTracker::PointerTracker(void *ptr)
    {
        this->key.count = internalCount++;
        this->key.ptr = ptr;
        trackingMap.insert(
            std::pair<TrackingKey, bool>(this->key, true));
    }

    bool PointerTracker::isValid()
    {
        return trackingMap.count(this->key);
    }

    void PointerTracker::remove()
    {
        trackingMap.erase(this->key);
    }
}