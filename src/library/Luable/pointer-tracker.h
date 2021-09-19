#ifndef ESPERATTO_LUA_POINTER_TRACKER
#define ESPERATTO_LUA_POINTER_TRACKER

#include <map>
#include <memory>
#include <allegro5/allegro5.h>

using namespace std;
namespace Esperatto
{
    struct TrackingKey
    {
        void *ptr;
        unsigned int count;

        bool operator<(const TrackingKey &other) const
        {
            return this->ptr < other.ptr && this->count < other.count;
        }

        bool operator>(const TrackingKey &other) const
        {
            return this->ptr > other.ptr && this->count > other.count;
        }

        bool operator==(const TrackingKey &other) const
        {
            return this->ptr == other.ptr && this->count == other.count;
        }
    };

    class PointerTracker
    {
    private:
        TrackingKey key;

    public:
        PointerTracker(void *ptr);

        bool isValid();
        void remove();
    };
}

#endif