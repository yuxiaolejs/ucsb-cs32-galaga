#include "event.hpp"
#include <iostream>

game::EVENT::Event::Event()
{
    type = EventType::NONE;
}

bool game::EVENT::EventQueue::overflow()
{
    return queue.size() >= MAX_SIZE;
}

void game::EVENT::EventQueue::push(Event event)
{
    if (block)
        return;
    if (overflow())
    {
        std::cout << "EventQueue overflow" << std::endl;
        while (overflow())
        {
            queue.pop();
        }
    }
    queue.push(event);
}

game::EVENT::Event game::EVENT::EventQueue::pop()
{
    Event event;
    if (empty())
        return event;
    event = queue.front();
    queue.pop();
    return event;
}

bool game::EVENT::EventQueue::empty()
{
    return queue.empty();
}
