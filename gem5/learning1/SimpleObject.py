
from m5.params import *
from m5.SimObject import SimObject


class SimpleObject(SimObject):
    type = "SimpleObject"
    cxx_header = "learning_gem5/part2/simple_object.hh"
    cxx_class = "gem5::SimpleObject"
