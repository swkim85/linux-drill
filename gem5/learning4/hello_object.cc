
#include "learning_gem5/part2/hello_object.hh"

#include "base/logging.hh"
#include "base/trace.hh"
#include "debug/HelloExample.hh"

namespace gem5
{

HelloObject::HelloObject(const HelloObjectParams &params) :
    SimObject(params),
    event([this]{ processEvent(); }, name() + ".event"),
    myName(params.name),
    latency(params.time_to_wait),
    timesLeft(params.number_of_fires),
    goodbye(params.goodbye_object)
    // Note: This is not needed as you can *always* reference this->name()
{
    DPRINTF(HelloExample, "Created the hello object with the name %s\n", myName);
    DPRINTF(HelloExample, "timesLeft = %d\n", timesLeft);
    panic_if(!goodbye, "HelloObject must have a non-null GoodbyeObject");
}

void
HelloObject::startup()
{
    // Before simulation starts, we need to schedule the event
    schedule(event, latency);
    //schedule(event, 100);
}

void
HelloObject::processEvent()
{
    timesLeft--;
    DPRINTF(HelloExample, "Hello world! Processing the event! %d left\n", timesLeft);
    if (timesLeft <= 0) {
        DPRINTF(HelloExample, "Done firing!\n");
        goodbye->sayGoodbye(myName);
    } else {
        schedule(event, curTick() + latency);
    }
}

} // namespace gem5
