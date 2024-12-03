
from itertools import combinations, permutations, product, accumulate

# 조합
print(list(combinations(['A', 'B', 'C'], 2)))
# 출력: [('A', 'B'), ('A', 'C'), ('B', 'C')]

# 순열
print(list(permutations(['A', 'B', 'C'], 2)))
# 출력: [('A', 'B'), ('A', 'C'), ('B', 'A'), ('B', 'C'), ('C', 'A'), ('C', 'B')]

# 데카르트곱
print(list(product(['A', 'B'], ['1', '2'])))
# 출력: [('A', '1'), ('A', '2'), ('B', '1'), ('B', '2')]

# 누적합
print(list(accumulate([1, 2, 3, 4, 5])))
# 출력: [1, 3, 6, 10, 15]

