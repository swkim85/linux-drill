
#include "learning_gem5/part2/simple_object.hh"

#include <iostream>

namespace gem5
{

SimpleObject::SimpleObject(const SimpleObjectParams &params) :
    SimObject(params)
{
    std::cout << "Hello World! From a SimObject!" << std::endl;
}

} // namespace gem5
