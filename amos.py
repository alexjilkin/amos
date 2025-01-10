import igraph as ig
import random
from copy import deepcopy


def get_amos(G: ig.Graph, A, amos, to=[]):
    n = len(G.vs)
    if len(to) == n:
        amos.append(to)
        return

    # Last not empty
    i = len(A) - 1 - next((idx for idx, x in enumerate(reversed(A)) if x), len(A))

    v = next(iter(A[i]))
    x = v
    R = set()

    while True:
        A[i] -= {x}
        to.append(x)
        x_neighbors = set(G.neighbors(x))
        for w in x_neighbors - set(to):
            j = next(i for i, s in enumerate(A) if w in s)
            A[j] -= {w}

            if j + 1 >= len(A) or not A[j + 1]:
                A.append(set())

            A[j + 1].add(w)

        get_amos(G, A, amos, to)

        for w in x_neighbors - set(to):
            j = next(i for i, s in enumerate(A) if w in s)
            A[j] -= {w}
            A[j - 1].add(w)

        A[i].add(x)
        x = to.pop()

        if x == v:
            R = reachable_in_subset(G, v, A[i]) - {v}

        if len(R) == 0:
            break
        x = R.pop()


def reachable_in_subset(G: ig.Graph, v: int, subset: set[int]):
    """
    Find all vertices reachable from vertex `v` within the subgraph induced by `subset`.
    """
    visited = set()
    stack = [v]

    while stack:
        current = stack.pop()
        if current in visited:
            continue
        visited.add(current)

        # Traverse only neighbors within the subset
        for neighbor in G.neighbors(current):
            if neighbor in subset and neighbor not in visited:
                stack.append(neighbor)

    return visited
