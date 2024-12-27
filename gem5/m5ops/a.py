
import m5
from m5.objects import *
from m5.util import addToPath

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



# Add gem5 paths (if necessary)
#addToPath('/path/to/gem5/configs')


system = System()
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "1GHz"
system.clk_domain.voltage_domain = VoltageDomain()

system.mem_mode = "timing"  # Use timing accesses
system.mem_ranges = [AddrRange("512MB")]  # Create an address range

system.cpu = X86TimingSimpleCPU()
system.membus = SystemXBar()

system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

system.cpu.createInterruptController()
system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

system.system_port = system.membus.cpu_side_ports


# Define exit event handlers
#def workbegin_handler():
#    print("------------Work unit has begun.")
#    m5.debug.flags["ExecAll"].enable()  # Enable detailed execution tracing if needed

#def workend_handler():
#    print("------------- Work unit has ended.")
#    #m5.exit(0)  # Exit the simulation gracefully

# define a workbegin handler
def workbegin_handler():
    print("------------- Workbegin handler")
    m5.debug.flags["ExecAll"].enable()
    yield False

# define a workend handler
def workend_handler():
    print("------------- Workend handler")
    m5.debug.flags["ExecAll"].disable()
    yield False


# Set up the simulation environment
root = Root(full_system=False, system=system)

# Add event handlers for work begin and end

#binary = "m5exit"
binary = "02-annotate-this"
system.workload = SEWorkload.init_compatible(binary)

process = Process()
process.cmd = [binary]
system.cpu.workload = process
system.cpu.createThreads()

simulator = Simulator(
    board=system,
    on_exit_event= {
        ExitEvent.WORKBEGIN: workbegin_handler(),
        ExitEvent.WORKEND: workend_handler()
    }
)

# Start the simulation
print("Starting simulation...")

#m5.instantiate()  # Prepare for simulation
simulator.run()

#exit_event = m5.simulate()  # Run the simulation

print("Exiting @ tick {} because {}".format(m5.curTick(), exit_event.getCause()))
print("Simulation finished.")
    


