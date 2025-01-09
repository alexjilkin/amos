import igraph as ig
import random
from copy import deepcopy


def enumarate(G: ig.Graph, A, to=[]):
    n = len(G.vs)
    if len(to) == n:
        yield to
        return

    # Last not empty
    i = len(A) - 1 - next((idx for idx, x in enumerate(reversed(A)) if x), len(A))

    v = random.choice(list(A[i]))
    x = v
    R = set()

    while True:
        A[i] -= {x}
        to.append(x)
        for w in set(G.neighbors(x)) - set(to):
            j = next(i for i, s in enumerate(A) if w in s)
            A[j] -= {w}

            if j + 1 >= len(A) or not A[j + 1]:
                A.append(set())

            A[j + 1].add(w)

        yield from enumarate(G, A, to)

        for w in set(G.neighbors(x)) - set(to):
            j = next(i for i, s in enumerate(A) if w in s)
            A[j] -= {w}
            A[j - 1].add(w)

        A[i].add(x)
        x = to.pop()

        if x == v:
            subV = list(A[i])
            subg = G.subgraph(subV)

            subg.vs["orig_id"] = subV
            v_subindex = subV.index(v)
            comp_in_sub = subg.subcomponent(v_subindex)

            R = {subg.vs[idx]["orig_id"] for idx in comp_in_sub} - {v}
            print("R", R)

        if len(R) == 0:
            break
        x = R.pop()
