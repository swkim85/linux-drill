# jacobi1.py
import numpy as np
from scipy.linalg import lu

ITERATION_LIMIT = 1000

# initialize the matrix
A = np.array([[10., -1., 2., 0.],
              [-1., 11., -1., 3.],
              [2., -1., 10., -1.],
              [0.0, 3., -1., 8.]])
# initialize the RHS vector
b = np.array([6., 25., -11., 15.])


# prints the system
print("System:")
for i in range(A.shape[0]):
    row = [f"{A[i, j]}*x{j + 1}" for j in range(A.shape[1])]
    print(f'{" + ".join(row)} = {b[i]}')
print()

L = np.tril(A, k=-1)
U = np.triu(A, k=1)
diagaonal_elements = 1/np.diag(A)
#print(diagaonal_elements)
Dinv = np.diag(diagaonal_elements)
#print(L)
#print(U)
#print(Dinv)
DinvLU = np.dot(Dinv, L+U)

def jacobi_step(x, b):
  result = -np.dot(DinvLU, x) + np.dot(Dinv, b)
  #print(result)
  return result


x_new = x = np.zeros_like(b)
for it_count in range(ITERATION_LIMIT):
  print(f"Iteration {it_count}: {x}")

  x_new = jacobi_step(x, b)

  if np.allclose(x, x_new, atol=1e-10, rtol=0.):
    break

  x = x_new

print("Solution: ")
print(x)

error = np.dot(A, x) - b
print("Error:")
print(error)


