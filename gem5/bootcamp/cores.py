from gem5.components.boards.simple_board import SimpleBoard
from gem5.components.cachehierarchies.classic.private_l1_cache_hierarchy import PrivateL1CacheHierarchy
from gem5.components.memory.single_channel import SingleChannelDDR3_1600
from gem5.components.processors.cpu_types import CPUTypes
from gem5.components.processors.simple_processor import SimpleProcessor
from gem5.isas import ISA
from gem5.resources.resource import obtain_resource
from gem5.simulate.simulator import Simulator


# A simple script to test with different CPU models
# We will run a simple application (matrix-multiply) with AtomicSimpleCPU and TimingSimpleCPU
# using two different cache sizes

# Steps
    # 1. Leave cpu_type as Atomic and run with
        # gem5 --outdir=atomic-normal-cache ./materials/02-Using-gem5/04-cores/cores.py
    # 2. Change cpu_type to Timing and run with
        # gem5 --outdir=timing-normal-cache ./materials/02-Using-gem5/04-cores/cores.py
    # 3. Change l1d_size and l1i_size to 1KiB and run with
        # gem5 --outdir=timing-small-cache ./materials/02-Using-gem5/04-cores/cores.py
    # 4. Change cpu_type to Atomic and run with
        # gem5 --outdir=atomic-small-cache ./materials/02-Using-gem5/04-cores/cores.py
    # 5. Compare the stats.txt file in
        # atomic-normal-cache/
        # timing-normal-cache/
        # timing-small-cache/
        # atomic-small-cache/

# In general: Run with gem5 [optional: --outdir=<cpu_type>-<cache_size>-cache] ./materials/developing-gem5-models/04-cores/cores.py

# Start with 32KiB and change to 1KiB during step 3
cache_hierarchy = PrivateL1CacheHierarchy(l1d_size="32KiB", l1i_size="32KiB")

memory = SingleChannelDDR3_1600("1GiB")

#cpu_type = CPUTypes.ATOMIC
#cpu_type = CPUTypes.TIMING
cpu_type = CPUTypes.O3

processor = SimpleProcessor(cpu_type=cpu_type, isa=ISA.RISCV, num_cores=8)

board = SimpleBoard(
    clk_freq="3GHz",
    processor=processor,
    memory=memory,
    cache_hierarchy=cache_hierarchy
)

# Resources can be found at
    # https://resources.gem5.org/
# riscv-matrix-multiply is obtained from
    # https://resources.gem5.org/resources/riscv-getting-started-benchmark-suite?version=1.0.0

workload = obtain_resource("riscv-matrix-multiply-run")
board.set_workload(workload)
simulator = Simulator(board=board)
simulator.run()
