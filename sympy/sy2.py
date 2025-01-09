
import sympy as sy

a11, a12, b1 = sy.symbols('a11 a12 b1')
a21, a22, b2 = sy.symbols('a21 a22 b2')
x, y = sy.symbols('x y')

# 방정식 정의
eq1 = sy.Eq(a11*x + a12*y, -b1)
eq2 = sy.Eq(a21*x + a22*y, -b2)

solution = sy.solve((eq1, eq2), (x, y))

print("해: ", solution)

