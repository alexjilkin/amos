from igraph import Graph
import itertools
import random

from igraph import Graph

uccgs = []


def all_uccgs(n, k, G, vs, seen=set()):
    adj_matrix = tuple(map(tuple, G.get_adjacency().data))
    if adj_matrix in seen:
        return
    seen.add(adj_matrix)

    if len(vs) == k:
        uccgs.append(G.copy())

    for v in vs:
        neighbors_of_v = set(G.neighbors(v))

        if len(vs) == k:
            for u in vs - neighbors_of_v - {v}:
                G.add_edge(v, u)

                if G.is_chordal():
                    all_uccgs(n, k, G, vs | {u}, seen)

                G.delete_edges([(v, u)])

        else:
            for u in set(range(n)) - vs - neighbors_of_v - {v}:
                G.add_edge(v, u)
                all_uccgs(n, k, G, vs | {u}, seen)
                G.delete_edges([(v, u)])


n = 20
k = 5
G = Graph()
G.add_vertices(n)
vs = set()
vs.add(random.randint(0, n - 1))

all_uccgs(n, k, G, vs)
print("Number of UCCGs found:", len(uccgs))
if uccgs:
    print("Edges in the first UCCG:")
    print([(e.source, e.target) for e in uccgs[0].es])
# print([(e.source, e.target) for e in uccgs[2].es])
# print([[(e.source, e.target) for e in g.es] for g in uccgs])
