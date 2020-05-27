#define BENCH_TIMES 1
#define DIFFERENT_EDGE_COST_TIMES 1

#include <math.h>

#include "LEDA/graph/graph.h"
#include "LEDA/graph/graph/graph_misc.h"
#include "LEDA/core/random_source.h"
#include "LEDA/core/p_queue.h"
#include "LEDA/graph/node_partition.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/graph/graph_gen.h"
#include "LEDA/system/timer.h"
#include "LEDA/core/dynamic_trees.h"
#include "LEDA/graph/min_span.h"

using namespace leda;
static int z = 3;

#define EDGE_COST_MIN -10000
#define EDGE_COST_MAX 10000

void BENCH_MST_LEDA(const graph &G, const edge_array<int> &cost) {
    timer dfs;
    float elapsed_time = 0;
    list<edge> ledas;

    for (int i = 0; i < BENCH_TIMES; i++) {
        dfs.start();

        ledas = MIN_SPANNING_TREE(g, cost);
        
        dfs.stop();
        elapsed_time += dfs.elapsed_time();
    }

    std::cout << "LEDA's time: " << (elapsed_time / BENCH_TIMES) << std::endl;
}

// performs the transformation from Go -> G
// so that no vertex got more than 3 degree
// page 4 of original document, page 7 of scanned pdf from purdue university
void degree_fix(const Graph& G, const edge_array<int> &cost){
    node anchor;
    forall_nodes(anchor, G) {
        if (G.degree(anchor) > 3) { 
            // we will generate 
        }
    }
}

static std::vector<std::set<node>> printed;

std::set<node> csearch(const graph &G, node &v, list<edge> &mst){
    std::set<node> cluster;
    cluster.insert(v);
    node w;
    //for all child nodes w of v
    forall_adj_nodes(w, v) { // TODO: fix this to include only edges on the mst   
        std::set<node> subcluster = csearch(G, w, mst);
        std::set<node>::iterator it;
        for (it = subcluster.begin(); it != subcluster.end(); ++it) {
            node n = *it;
            cluster.insert(n);
        }
    }

    if (cluster.size() < z) {
        return cluster;
    } else {
        printed = cluster;
        std::set<node> empty;
        return empty; // supposed returned empty set
    }

}


// builds the mst in a streaming-iterative fashion
list<edge> MST_ONLINE(const graph &G, const edge_array<int> &cost){
    degree_fix(G, cost);

    // the initial tree that will get updated
    // in an iterative fashion
    list<edge> mst;
    edge_array<bool> in_mst(G);

    // build an initial mst

    // add rest of the vertices and edges
    

    /* FINDCLUSTERS */
    node root; // = some leaf node of mst

    std::vector<std::set<node>> clusters;
    printed = clusters

    // essentially a dfs variant
    std::set<node> cluster = csearch(G, root, mst);
    if (clusters.size() > 0) {
        // merge with last printed 
        std::set<node> last_printed = printed.back();

        std::set<node>::iterator it;
        for (it = printed.begin(); it != printed.end(); ++it) {
            node n = *it;
            last_printed.insert(n);
        }
    }

    clusters = printed;
    // make map from clusters to idx

    // partition edges to E_i,j sets

    // map E_i,j sets to min cost map

    /* END FINDCLUSTERS */

    // edge modifications

    /* case: in mst and cost increased */

    /* case: not in mst and cost decreased */
}


void benchmark(const graph &G, const edge_array<int> &cost, int n, int m){
    random_source S;

    timer t;
    float elapsed_time = 0;
    list<edge> mst;

    for (int i = 0; i < BENCH_TIMES; i++) {
        t.start();

        mst = MIN_SPANNING_TREE(g, cost);
        
        t.stop();
        elapsed_time += t.elapsed_time();
    }

    std::cout << "MST construction time: " << (elapsed_time / BENCH_TIMES) << std::endl;

    std::vector<edge> edges;
    edge e;
    forall_edges(e, G) {
        edges.push_back(e);
    }

    // create random stream of edge costs to update
    for (int i = 1; i < DIFFERENT_EDGE_COST_TIMES; i++) {
        int s = m * i; // how many random edge-cost updates to do

        std::vector<std::pair<edge, int>> cost_updates;

        // generate random edge-cost pairs
        int edge_size = edges.size();
        for (int __x = 0; __x < s; __x++){
            
            int rand_idx;
            S >> rand_idx;
            rand_idx = rand_idx % edge_size;

            int c;
            S >> c;
            int rand_cost = (c % (EDGE_COST_MAX – EDGE_COST_MIN + 1)) + EDGE_COST_MIN)

            cost_updates.push_back(std::make_pair<edges[rand_idx], rand_cost>);
        }

        // bechmarks
        float mst_online_time = 0;
        float mst_static_time = 0;  

        float mst_online_per_edge = 0;
        float mst_static_per_edge = 0;

        elapsed_time = 0;
        float elapsed_per_edge = 0;

        for (int i = 0; i < BENCH_TIMES; i++) {
            // copy mst, G and edge costs so that there is a fresh copy
            graph h;
            CopyGraph(h, G);
            edge_array<int> h_cost(h);
            edge e;
            forall_edges(e, h) {
                // http://www.algorithmic-solutions.info/leda_manual/graph_misc.html
                // ^ docs say that h[e] corresponds to G[e]
                h_cost[e] = cost[h[e]];
            }

            // https://www.leda-tutorial.org/en/official/ch02s03s06.html
            // ^ copy list
            list<edge> h_mst = mst;

            t.start();
            
            elapsed_per_edge += MST_ONLINE(h, h_cost, h_mst, cost_updates);
            
            t.stop();
            elapsed_time += t.elapsed_time();
        }

        // std::cout << "MST_ONLINE time: " << (elapsed_time / BENCH_TIMES) << std::endl;
        mst_online_time += elapsed_time;
        mst_online_per_edge += elapsed_per_edge;

        /* mst static */
        elapsed_time = 0;
        elapsed_per_edge = 0;

        for (int i = 0; i < BENCH_TIMES; i++) {
            // copy mst, G and edge costs so that there is a fresh copy
            graph h;
            CopyGraph(h, G);
            edge_array<int> h_cost(h);
            edge e;
            forall_edges(e, h) {
                // http://www.algorithmic-solutions.info/leda_manual/graph_misc.html
                // ^ docs say that h[e] corresponds to G[e]
                h_cost[e] = cost[h[e]];
            }

            // https://www.leda-tutorial.org/en/official/ch02s03s06.html
            // ^ copy list
            list<edge> h_mst = mst;

            t.start();
            
            elapsed_per_edge += MST_STATIC(h, h_cost, h_mst, cost_updates);
            
            t.stop();
            elapsed_time += t.elapsed_time();
        }

        // std::cout << "MST_STATIC time: " << (elapsed_time / BENCH_TIMES) << std::endl;
        mst_static_time += elapsed_time;
        mst_static_per_edge += elapsed_per_edge;
    }

    std::cout << std::endl << std::endl;

    std::cout << "MST_ONLINE: " << (mst_online_time / DIFFERENT_EDGE_COST_TIMES) << " , per edge: " << (mst_online_per_edge / DIFFERENT_EDGE_COST_TIMES) << std::endl;
    std::cout << "MST_STATIC: " << (mst_static_time / DIFFERENT_EDGE_COST_TIMES) << " , per edge: " << (mst_static_per_edge / DIFFERENT_EDGE_COST_TIMES) << std::endl;
}

int tests()
{
    random_source S;

    std::cout << "Connected Graphs" << std::endl << std::endl;

    int n_sizes[] = {100};
    for (int i = 0; i < 1; i++) {
        int n = n_sizes[i];
        int m = 2 * n * log(n);

        // generate connected graph with n nodes and m edges
        graph g;
        random_loop_free_graph(g, n, m);
        Make_Connected(g);
        g.make_undirected();

        edge_array<int> cost(g);
        edge e;
        forall_edges(e, g) {
            int c;
            S >> c;
            cost[e] = (c % (EDGE_COST_MAX – EDGE_COST_MIN + 1)) + EDGE_COST_MIN);
        }

        z = max(1, pow(m, 2/5)); // see paper

        std::cout << "Random Graph (" << n << ", " << m << ") " << i << std::endl;
        benchmark(g, cost, n, m);
    }

    return 0;
}

int main(){
    tests();
    return 0;
}