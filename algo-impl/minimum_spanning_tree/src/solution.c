#include <math.h>

#include "LEDA/graph/graph.h"
#include "LEDA/core/random_source.h"
#include "LEDA/core/p_queue.h"
#include "LEDA/graph/node_partition.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/graph/graph_gen.h"
#include "LEDA/system/timer.h"
#include "LEDA/core/dynamic_trees.h"
#include "LEDA/graph/min_span.h"

using namespace leda;

typedef struct ListNode {
    ListNode* lists_first;
    int size;
    ListNode* next;
    ListNode* lists_last;
    int i;
} ListNode;

list<edge> certify(const graph &G, const edge_array<int> &cost)
{
    list<edge> T;

    p_queue<int, edge> PQ;
    edge e;
    forall_edges(e, G) 
    {
        PQ.insert(cost[e], e);
    }

    node_array<ListNode*> listnodes(G);
    // std::cout << "Making listnodes" << std::endl;
    node n;
    forall_nodes(n, G){
        // std::cout << "new alloc" << std::endl;
        ListNode* listnode = new ListNode;
        // std::cout << "lists_first" << std::endl;
        listnode->lists_first = listnode;
        listnode->size = 1;
        // std::cout << "next" << std::endl;
        listnode->next = NULL;
        listnode->lists_last = listnode;

        listnodes[n] = listnode;
    }

    // std::cout << "Making MST" << std::endl;
    while (!PQ.empty())
    {
        e = PQ.inf(PQ.find_min());
        PQ.del_min();

        node u = G.source(e);
        node v = G.target(e);
        
        ListNode* list_u = listnodes[u]->lists_first;
        ListNode* list_v = listnodes[v]->lists_first;

        if (list_u != list_v)
        { // if not cycle
            // std::cout << "No cycle" << std::endl;

            T.append(e);

            ListNode* small = NULL;
            ListNode* big = NULL;
            if (list_u->size > list_v->size) {
                small = list_v;
                big = list_u;
            }else{
                small = list_u;
                big = list_v;
            }

            // std::cout << "Merging listnodes" << std::endl;
            // merge small into big list
            big->size += small->size;

            ListNode* curtail = big->lists_last;
            curtail->next = small;

            ListNode* elem = small;
            while(elem != NULL) {
                elem->lists_first = big; // update head ptr
                
                curtail = elem; // move to next
                elem = elem->next;
            }

            big->lists_last = curtail;
            // std::cout << "big size: " << big->size << " small size: " << small->size << std::endl;
        } // end if in same cycle
    }

    forall_nodes(n, G) {
        delete listnodes[n]; // ptr cleanups
    }

    return T;
}

bool checker(const graph &G, const list<edge> &T, const edge_array<int> &cost)
{
    dynamic_trees D;

    node_array<vertex> vertices(G);
    node n;
    // generate vertices
    // std::cout << "Making Vertices" << std::endl;
    int cnt = 0;
    forall_nodes(n, G) {
        vertices[n] = D.make();
    }

    // construct dynamic tree
    // std::cout << "Constructing Dynamic Tree" << std::endl;
    edge e;
    int i = 0;
    forall(e, T) {
        node source = G.source(e);
        node target = G.target(e);
        vertex vs = vertices[source];
        vertex vt = vertices[target];
        int c = cost[e];
        D.link(vs, vt, c);
    }

    // for all edges (e) in G and not in T
    forall_edges(e, G)
    {
        if (T.search(e) != nil) {
            continue;
        }

        int curr_cost = cost[e];

        node source = G.source(e);
        node target = G.target(e);

        vertex corresponding_src = vertices[source];
        vertex corresponding_trg = vertices[target];

        D.evert(corresponding_src);

        // G.print_edge(e);
        // std::cout << std::endl;

        // traverse path from source to target
        vertex tmp = corresponding_trg;
        while (tmp != corresponding_src && tmp != nil){
            vertex parent = D.parent(tmp);
            int cost = std::round(D.cost(tmp)); // D returns double but we only assign ints

            if (cost > curr_cost) {
                return false;
            }

            tmp = parent;
        }
    }

    return true;
}

void run_tests(const graph& g)
{
    random_source S;
    edge_array<int> cost(g);
    edge e;
    forall_edges(e, g) {
        int c;
        S >> c;
        cost[e] = 10 + c % 10000; // random cost range
    }

    int times = 1;
    float elapsed_time = 0;
    timer dfs;
    list<edge> tree;
    for (int i = 0; i < times; i++) {
        dfs.start();
        tree = certify(g, cost);
        dfs.stop();
        elapsed_time += dfs.elapsed_time();
    }
    std::cout << "Our benchmark time: " << (elapsed_time / times) << std::endl;

    elapsed_time = 0;
    list<edge> ledas;
    for (int i = 0; i < times; i++) {
        dfs.start();
        ledas = MIN_SPANNING_TREE(g, cost);
        
        dfs.stop();
        elapsed_time += dfs.elapsed_time();
    }
    std::cout << "LEDA's time: " << (elapsed_time / times) << std::endl;

    // forall(e, tree){
    //     g.print_edge(e);
    // }
    
    bool verification = checker(g, ledas, cost);
    std::cout << "LEDA: Verification: " << (verification == 1 ? "true" : "false") << std::endl;

    verification = checker(g, tree, cost);
    std::cout << "OURS: Verification: " << (verification == 1 ? "true" : "false") << std::endl;

    std::cout << "=============" << std::endl << std::endl;
}

int experiments(){
    graph g;
    random_simple_loopfree_graph(g, 5, 5);
    Make_Connected(g);
    g.make_undirected();

    run_tests(g);

    return 0;
}

int tests()
{

    // graph ge; // graph >= ~ 400 makes segmentation
    // grid_graph(ge, 500);
    // Make_Connected(ge);
    // ge.make_undirected();
    // run_tests(ge);

    std::cout << "Connected Graphs" << std::endl << std::endl;

    int n_sizes[] = {4000, 8000, 16000};
    for (int i = 0; i < 3; i++) {
        int n = n_sizes[i];
        int m = 2 * n * log(n);

        // generate connected graph with n nodes and m edges
        graph g;
        random_simple_loopfree_graph(g, n, m);
        Make_Connected(g);
        g.make_undirected();

        std::cout << "Random Graph (" << n << ", " << m << ") " << i << std::endl;
        run_tests(g);
    }
    std::cout << std::endl << std::endl;

    std::cout << "Grid Graphs" << std::endl << std::endl;

    int g_sizes[] = {200, 300, 400};
    for (int i = 0; i < 2; i++) {
        int n = g_sizes[i];

        graph g;
        grid_graph(g, n);
        Make_Connected(g); // No need
        g.make_undirected();

        std::cout << "Grid Graph (" << n << ", " << n << ")" << std::endl;
        run_tests(g);
    }
    std::cout << std::endl << std::endl;

    std::cout << "Custom Graphs" << std::endl << std::endl;

    std::cout << "Complete Graph (2000, with make_undirected)" << std::endl;
    graph g_c;
    g_c.make_undirected();
    complete_graph(g_c, 2000);
    run_tests(g_c);

    std::cout << "Complete Graph (2000, without make_undirected)" << std::endl;
    graph g;
    complete_graph(g, 2000);
    run_tests(g);

    return 0;
}

int main(){
    return tests(); // <-- This runs 5-node graph for testing 
    // return experiments(); // <-- This runs benchmarks
}