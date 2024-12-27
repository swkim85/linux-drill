
import m5

from m5.objects import *

m5.util.addToPath("/home/ubuntu/gem5/configs")
from common.FileSystemConfig import config_filesystem

from msi_caches import MyCacheSystem

system = System()

system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "1GHz"
system.clk_domain.voltage_domain = VoltageDomain()

system.mem_mode = "timing"  # Use timing accesses
system.mem_ranges = [AddrRange("512MB")]  # Create an address range

system.cpu = [X86TimingSimpleCPU() for i in range(2)]

system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]

for cpu in system.cpu:
    cpu.createInterruptController()

system.caches = MyCacheSystem()
system.caches.setup(system, system.cpu, [system.mem_ctrl])

thispath = os.path.dirname(os.path.realpath(__file__))
binary = "threads"

process = Process()
process.cmd = [binary]
for cpu in system.cpu:
    cpu.workload = process
    cpu.createThreads()

system.workload = SEWorkload.init_compatible(binary)

config_filesystem(system)

root = Root(full_system=False, system=system)

m5.instantiate()

print("Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")
