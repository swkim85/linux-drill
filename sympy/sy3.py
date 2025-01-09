
import sympy as sy

sy.init_printing(use_unicode=True)

a11, a12, a13, b1 = sy.symbols('a11 a12 a13 b1')
a21, a22, a23, b2 = sy.symbols('a21 a22 a23 b2')
a31, a32, a33, b3 = sy.symbols('a31 a32 a33 b3')
x, y, z = sy.symbols('x y z')

# 방정식 정의
eq1 = sy.Eq(a11*x + a12*y + a13*z, b1)
eq2 = sy.Eq(a21*x + a22*y + a23*z, b2)
eq3 = sy.Eq(a31*x + a32*y + a33*z, b3)

solution = sy.solve((eq1, eq2, eq3), (x, y, z))

print("Solution: ", solution)

