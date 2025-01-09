
import sympy as sy

# 변수 정의
x, y = sy.symbols('x y')

# 방정식 정의
eq1 = sy.Eq(2*y + 7*x, -5)
eq2 = sy.Eq(5*y - 7*x, 12)

# 연립 방정식 풀기
solution = sy.solve((eq1, eq2), (x, y))

print("해: ", solution)

