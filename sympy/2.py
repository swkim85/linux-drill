import sympy
from sympy import symbols

print (sympy.sqrt(3))
print (sympy.sqrt(8))

x, y = symbols('x y')
expr = x + 2*y
print(expr)
print(expr + 1)
print(expr - x)
print(x * expr)
