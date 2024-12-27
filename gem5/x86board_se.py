import argparse
import time

import m5
from m5.objects import Root
from m5.stats.gem5stats import get_simstat
from m5.util import warn

#from gem5.components.boards.x86_board import X86Board
#from gem5.components.processors.cpu_types import CPUTypes
from gem5.resources.resource import Resource
#from gem5.simulate.simulator import Simulator


from gem5.coherence_protocol import CoherenceProtocol

from gem5.components.boards.x86_board import X86Board
from gem5.components.processors.simple_processor import SimpleProcessor
#from gem5.components.memory import DualChannelDDR4_2400
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
#from gem5.components.processors.simple_switchable_processor import ( SimpleSwitchableProcessor)
from gem5.isas import ISA
from gem5.resources.resource import obtain_resource
from gem5.simulate.simulator import (
    ExitEvent,
    Simulator,
)
#from gem5.utils.requires import requires

#requires(
#    isa_required=ISA.X86,
#    coherence_protocol_required=CoherenceProtocol.MESI_TWO_LEVEL
#)


from gem5.components.cachehierarchies.ruby.mesi_two_level_cache_hierarchy import (
    MESITwoLevelCacheHierarchy,
)

cache_hierarchy = MESITwoLevelCacheHierarchy(
    l1d_size="32kB",
    l1d_assoc=8,
    l1i_size="32kB",
    l1i_assoc=8,
    l2_size="256kB",
    l2_assoc=16,
    num_l2_banks=2,
)
# Memory: Dual Channel DDR4 2400 DRAM device.
# The X86 board only supports 3 GB of main memory.

#memory = DualChannelDDR4_2400(size="3GB")

memory = SingleChannelDDR3_1600("1GiB")
processor = SimpleProcessor(
  cpu_type=CPUTypes.ATOMIC,
  isa=ISA.X86,
  num_cores=1
)

board = X86Board(
  clk_freq="1GHz",
  processor=processor,
  memory=memory,
  cache_hierarchy=cache_hierarchy,
)

#board.set_workload(obtain_resource(args.benchmark))

#binary = Resource("cpuid")
board.set_workload(obtain_resource("npb-bt-a"))
#board.set_se_binary_workload(binary)


#binary = "cpuid"
#system.workload = SEWorkload.init_compatible(binary)


# The first exit_event ends with a `workbegin` cause. This means that the
# system started successfully and the execution on the program started.
def handle_workbegin():
    print("Done booting Linux")
    print("Resetting stats at the start of ROI!")

    m5.stats.reset()

    # We have completed up to this step using KVM cpu. Now we switch to timing
    # cpu for detailed simulation.

    # # Next, we need to check if the user passed a value for --ticks. If yes,
    # then we limit out execution to this number of ticks during the ROI.
    # Otherwise, we simulate until the ROI ends.
    processor.switch()
    if args.ticks:
        # schedule an exit event for this amount of ticks in the future.
        # The simulation will then continue.
        m5.scheduleTickExitFromCurrent(args.ticks)
    yield False


# The next exit_event is to simulate the ROI. It should be exited with a cause
# marked by `workend`.


# We exepect that ROI ends with `workend` or `simulate() limit reached`.
def handle_workend():
    print("Dump stats at the end of the ROI!")

    m5.stats.dump()
    yield True


simulator = Simulator(
    board=board,
    on_exit_event={
        ExitEvent.WORKBEGIN: handle_workbegin(),
        ExitEvent.WORKEND: handle_workend(),
    },
)

# We maintain the wall clock time.

globalStart = time.time()

print("Running the simulation")
print("Using KVM cpu")

# We start the simulation.
simulator.run()

# We need to note that the benchmark is not executed completely till this
# point, but, the ROI has. We collect the essential statistics here before
# resuming the simulation again.

# Simulation is over at this point. We acknowledge that all the simulation
# events were successful.
print("All simulation events were successful.")
# We print the final simulation statistics.

print("Done with the simulation")
print()
print("Performance statistics:")

# manually calculate ROI time if ticks arg is used in case the
# entire ROI wasn't simulated
if args.ticks:
    print(f"Simulated time in ROI (to tick): {args.ticks/ 1e12}s")
else:
    print(f"Simulated time in ROI: {simulator.get_roi_ticks()[0] / 1e12}s")

print(
    f"Ran a total of {simulator.get_current_tick() / 1e12} simulated seconds"
)
print(
    "Total wallclock time: %.2fs, %.2f min"
    % (time.time() - globalStart, (time.time() - globalStart) / 60)
)
