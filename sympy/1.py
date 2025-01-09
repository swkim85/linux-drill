
from sympy import symbols, Eq, solve

# Define the symbol
x = symbols('x')

# Define the equation
equation = Eq(x * (2/5), 1760)

# Solve the equation
solution = solve(equation)

print(f'The solution is: {solution}')

