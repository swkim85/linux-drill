
from sympy import *
x, t, z, nu = symbols('x t z nu')

init_printing(use_unicode=True)

diff(sin(x)*exp(x), x)

integrate(exp(x)*sin(x) + exp(x)*cos(x), x)

integrate(sin(x**2), (x, -oo, oo))

