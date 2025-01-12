import numpy as np
from scipy.special import factorial


def lehmer_code(perm):
    """Compute the lexicographic index of a permutation directly."""
    n = len(perm)
    index = 0
    used = [False] * n
    factorials = factorial(range(n), exact=True)[::-1]  # Factorials in reverse

    for i in range(n):
        smaller = sum(1 for j in range(perm[i]) if not used[j])
        index += smaller * factorials[i]
        used[perm[i]] = True

    return code_to_bits(index, n)


def code_to_bits(code, k):
    """Encode permutation to binary form with k-dependent length."""
    max_bits = int(np.ceil(np.log2(factorial(k))))
    return bin(code)[2:].zfill(max_bits)
