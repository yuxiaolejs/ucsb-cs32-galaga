#ifndef EVENT_HPP
#define EVENT_HPP
#include <queue>
#include <vector>
#include <cstddef>
namespace game::EVENT
{
    struct Event
    {
        Event();
        enum EventType
        {
            NONE,
            KEYBOARD_EVENT,
            KEYBOARD_UP_EVENT,
        };
        EventType type;

        std::vector<int> data;
    };
    struct EventQueue
    {
        const size_t MAX_SIZE = 10;
        bool block = false;
        std::queue<Event> queue;
        bool overflow();
        void push(Event event);
        Event pop();
        bool empty();
    };
}
#endif