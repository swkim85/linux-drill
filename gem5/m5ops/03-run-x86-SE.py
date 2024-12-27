# system components
from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.components.memory.single_channel import SingleChannelDDR4_2400
from gem5.components.cachehierarchies.classic.private_l1_cache_hierarchy import PrivateL1CacheHierarchy

# simulation components
from gem5.components.processors.cpu_types import CPUTypes
from gem5.resources.resource import BinaryResource
from gem5.simulate.exit_event import ExitEvent
from gem5.simulate.simulator import Simulator
from gem5.isas import ISA
from pathlib import Path
import m5.debug

'''
usage 
gem5 -re 03-run-x86-SE.py
'''


binary_path = Path("02-annotate-this")

cache_hierarchy = PrivateL1CacheHierarchy(
    l1d_size="64kB",
    l1i_size="64kB",
)

memory = SingleChannelDDR4_2400("1GB")

processor = SimpleProcessor(
    cpu_type = CPUTypes.TIMING,
    num_cores = 1,
    isa = ISA.X86
)

board = SimpleBoard(
    clk_freq="1GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy,
)

board.set_se_binary_workload(
    binary = BinaryResource(
        local_path=binary_path.as_posix()
    )
)
# define a workbegin handler
def workbegin_handler():
    print("Workbegin handler")
    m5.debug.flags["ExecAll"].enable()
    yield False

# define a workend handler
def workend_handler():
    print("Workend handler")
    m5.debug.flags["ExecAll"].disable()
    yield False

simulator = Simulator(
    board=board,
    # setup handler for ExitEvent.WORKBEGIN and ExitEvent.WORKEND
    on_exit_event= {
        ExitEvent.WORKBEGIN: workbegin_handler(),
        ExitEvent.WORKEND: workend_handler()
    }
)

print(simulator)
print(dir(simulator))

simulator.run()

print("Simulation Done")

