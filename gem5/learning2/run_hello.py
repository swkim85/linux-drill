
import m5

from m5.objects import *

root = Root(full_system=False)

# root.hello = HelloObject( )
#  #goodbye_object=GoodbyeObject(),
#  time_to_wait = '2us',
#  number_of_fires = '4'

root.hello = HelloObject( )
root.hello.time_to_wait = '1us'
root.hello.number_of_fires = '4'
#root.hello.goodbye_object = GoodbyeObject(buffer_size = '100B')


m5.instantiate()

print(f"Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")

