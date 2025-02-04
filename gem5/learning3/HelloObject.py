
from m5.params import *
from m5.SimObject import SimObject


class HelloObject(SimObject):
    type = "HelloObject"
    cxx_header = "learning_gem5/part2/hello_object.hh"
    cxx_class = "gem5::HelloObject"

    time_to_wait = Param.Latency("Time before firing the event")
    number_of_fires = Param.Int(
        1, "Number of times to fire the event before goodbye"
    )


