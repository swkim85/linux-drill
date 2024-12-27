
import m5

from m5.objects import *

root = Root(full_system=False)

root.simple_object = SimpleObject()

m5.instantiate()

print(f"Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")

