
import atexit

def exit_function1():
  print("프로그램이 종료됩니다! 1")
def exit_function2():
  print("프로그램이 종료됩니다! 2")

atexit.register(exit_function1)
atexit.register(exit_function2)

name = 'Bob'
print(f"Hello, {name}")

