
#ifndef __LEARNING_GEM5_HELLO_OBJECT_HH__
#define __LEARNING_GEM5_HELLO_OBJECT_HH__

#include <string>

#include "learning_gem5/part2/goodbye_object.hh"
#include "params/HelloObject.hh"
#include "sim/sim_object.hh"

namespace gem5
{

class HelloObject : public SimObject
{
  private:
    void processEvent();

    /// An event that wraps the above function
    EventFunctionWrapper event;

    /// Pointer to the corresponding GoodbyeObject. Set via Python
    GoodbyeObject* goodbye;

    const std::string myName;

    const Tick latency;

    /// Number of times left to fire the event before goodbye
    int timesLeft;

  public:
    HelloObject(const HelloObjectParams &p);

    /**
     * Part of a SimObject's initilaization. Startup is called after all
     * SimObjects have been constructed. It is called after the user calls
     * simulate() for the first time.
     */
    void startup() override;
};

} // namespace gem5

#endif // __LEARNING_GEM5_HELLO_OBJECT_HH__
