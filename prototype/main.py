from igraph import Graph
import itertools
import random
import time
from igraph import Graph
from amos import get_amos


from encode import lehmer_code

uccgs = []


def uccgs_brute(k):
    uccgs = []
    all_possible_edges = list(itertools.combinations(range(k), 2))

    for r in range(k - 1, len(all_possible_edges) + 1):
        for edge_subset in itertools.combinations(all_possible_edges, r):
            G = Graph()
            G.add_vertices(k)
            G.add_edges(edge_subset)

            if G.is_connected() and G.is_chordal():
                uccgs.append(G.copy())

    return uccgs


def uccgs_iterative(k, G, vs, seen=set()):
    edge_representation = tuple(sorted((e.source, e.target) for e in G.es))
    if edge_representation in seen:
        return

    seen.add(edge_representation)

    if len(vs) == k:
        uccgs.append(G.copy())
        return  # Stop recursion at the base case

    for v in vs:
        neighbors_of_v = set(G.neighbors(v))

        if len(vs) == k:
            for u in vs - neighbors_of_v - {v}:
                G.add_edge(v, u)

                if G.is_chordal():
                    uccgs_iterative(k, G, vs | {u}, seen)

                G.delete_edges([(v, u)])

        else:
            for u in set(range(k)) - vs - neighbors_of_v - {v}:
                G.add_edge(v, u)
                uccgs_iterative(k, G, vs | {u}, seen)
                G.delete_edges([(v, u)])


k = 3
G = Graph()
G.add_vertices(k)
vs = set()
vs.add(random.randint(0, k - 1))

start_time = time.time()
uccgs = uccgs_brute(k)
print("Brute UCCGs:", len(uccgs), time.time() - start_time)

for G in uccgs:
    print(G.get_adjacency())
start_time = time.time()
all_amos = []
G = uccgs[3]

print(
    "Edges:", ", ".join(f"({source}, {target})" for source, target in G.get_edgelist())
)
AMOs = []
get_amos(G, [set(v.index for v in G.vs)], AMOs)
# AMOs = [lehmer_code(AMO) for AMO in AMOs]
print(AMOs)
# all_amos.append(AMOs)

print(f"Enumrated AMOs:", len(all_amos), time.time() - start_time)
