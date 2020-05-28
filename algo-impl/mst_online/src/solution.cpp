#define BENCH_TIMES 1 // how many runs per edge cost updates that get generated (per graph)
#define DIFFERENT_EDGE_COST_TIMES 1 // how many times to make different edge cost update stream

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
#include "LEDA/core/map.h"

using namespace leda;
static int z = 3;

#define EDGE_COST_MIN 1
#define EDGE_COST_MAX 10000

template<typename T>
struct triplet
{
    T first, middle, last;
};

template<typename T>
triplet<T> make_triplet(const T &m1, const T &m2, const T &m3) 
{
    triplet<T> ans;
    ans.first = m1;
    ans.middle = m2;
    ans.last = m3;
    return ans;
}

triplet<float, float, float> MST_STATIC(const graph &G, edge_array<int> &cost, std::vector<std::pair<edge,int>> &cost_updates){
    // make mst
    timer mst_t;
    mst_t.start();
    list<edge> mst = MIN_SPANNING_TREE(g, cost);

    edge_array<bool> in_mst;
    edge e;
    forall(e, mst) {
        in_mst[e] = true;
    }

    // build dynamic tree
    dynamic_trees D;

    node_array<vertex> vertices(G);
    node n;
    // generate vertices
    forall_nodes(n, G) {
        vertices[n] = D.make();
    }

    // construct dynamic tree
    edge e;
    forall(e, T) {
        node source = G.source(e);
        node target = G.target(e);

        vertex vs = vertices[source];
        vertex vt = vertices[target];

        D.link(vs, vt, cost[e]);
    }
    mst_t.stop();

    // edge modifications
    timer per_edge;

    per_edge.start();
    std::vector<std::pair<edge, int>>::iterator it;
    for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
        std::pair<edge, int> pair = *it;
        edge to_update = pair.first;
        int new_cost = pair.second;

        bool is_in_mst = in_mst[to_update];
        int prev_cost = cost[to_update];

        if (is_in_mst && prev_cost >= new_cost || !is_in_mst && new_cost > prev_cost) {
            cost[to_update] = new_cost;
            continue;
        }

        if (!is_in_mst && new_cost < prev_cost) {
            cost[to_update] = new_cost;
            in_mst[to_update] = true;
            mst.append(to_update)

            vertex v = vertices[G.source(to_update)];
            vertex w = vertices[G.target(to_update)];

            D.link(v, w, new_cost);

            D.evert(v);
            // find (x, y) edge on tree that got maximum cost on the cycle created

            // TODO (x, y) = D.findmax(w);
            // traverse parents till from v till reached w
            // find maximum cost edge e
            
            // remove x, y edge from dynamic tree
            D.cut(x, y);

            // remove corresponding edge x, y from mst
            mst.del(e);
            in_mst[e] = false;
        }

        if (is_in_mst && new_cost > prev_cost) {
            cost[to_update] = new_cost;
            mst.del(to_update);
            in_mst[to_update] = false;

            vertex v = vertices[G.source(to_update)];
            vertex w = vertices[G.target(to_update)];

            D.cut(v, w);
            
            // TODO perform bfs/dfs to find out connected component nodes

            // for each node and edge, if on other connected component
            // find min cost and edge

            edge min_cost_edge;
            int min_cost;

            // add min cost into mst
            mst.append(min_cost_edge);
            in_mst[min_cost_edge] = true;
            // link dynamic tree too
             D.link(vertices[G.source(min_cost_edge)], vertices[G.target(min_cost_edge)], min_cost);
        }
    }
    per_edge.stop();

    return make_triplet<mst_t.elapsed_time(), 0, per_edge.elapsed_time()>;
}

// performs the transformation from Go -> G
// so that no vertex got more than 3 degree
// page 4 of original document, page 7 of scanned pdf from purdue university
// we dont care about costs, we just want to fix the degrees
// we will assign random costs later
void degree_fix(const Graph& G, edge_array<int> &cost){
    std::vector<node> to_delete;

    node anchor;
    forall_nodes(anchor, G) {
        int d = G.degree(anchor);
        if (d > 3) { 
            int n_to_add = 3;

            std::vector<node> adjacent_w;
            std::vector<node> edges_w;
            std::vector<node> adjacent_v;
            node w;
            edge e;
            forall_edges(e, anchor) {
                edges_w.push_back(e);
                adjacent_w.push_back(G.opposite(anchor, e));
            }

            // replace w with new v
            std::vector<node>::iterator it;
            for (it = adjacent_w.begin(); it != adjacent_w.end(); ++it) {
                w = *it;
                // add new node v
                node v = G.new_node();
                adjacent_v.push_back(v);
            }

            // connect v[i] with v[i+1 mod d] with cost 0
            for (int i = 0; i < d; i++) {
                int idx_src = i;
                int idx_trg = (i + 1) % d;
                node src = adjacent_v[idx_src];
                node trg = adjacent_v[idx_end];
                e = G.new_edge(src, trg);

                cost[e] = 0;
            }

            // add w[i] -> anchor with w[i] -> v[i] with the same cost
            for (int i = 0; i < d; i++) {
                edge original = edges_w[i];
                int c = cost[original];
                e = G.new_edge(adjacent_w[i], adjacent_v[i]);

                cost[e] = c;

                /* Update cost changes stream */
                std::vector<std::pair<edge, int>>::iterator it;
                for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
                    std::pair p = *it;

                    if (p.first == original) {
                        p.first = e;
                    }
                }
            }

            // remove adjacent_w edges and anchor
            for (int i = 0; i < d; i++) {
                G.hide_edge(edges_w[i]); // you could also hide
            }

            G.hide_node(anchor); //otherwise possible iteration exception? idk
        }
    }
}

static std::vector<std::set<node>> printed;

std::set<node> csearch(const graph &G, node &v, list<edge> &mst){
    std::set<node> cluster;
    cluster.insert(v);
    node w;
    //for all child nodes w of v
    edge e;
    forall_edges(e, mst) {
        if (G.source(e) == v || G.target(e) == v) { // only include mst edges
            node w = G.opposite(v, e);

            std::set<node> subcluster = csearch(G, w, mst);
            std::set<node>::iterator it;
            for (it = subcluster.begin(); it != subcluster.end(); ++it) {
                node n = *it;
                cluster.insert(n);
            }
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
triplet<float, float, float> MST_ONLINE(const graph &G, edge_array<int> &cost, std::vector<std::pair<edge,int>> &cost_updates){
    // make degree fix and make mst
    timer mst_t;
    mst_t.start();
    degree_fix(g, cost, cost_updates);
    list<edge> mst = MIN_SPANNING_TREE(g, cost);
    mst_t.stop();

    edge_array<bool> in_mst(G);
    edge e;
    forall(e, mst) {
        in_mst[e] = true;
    }

    timer cluster_timer;
    cluster_timer.start();
    /* FINDCLUSTERS */
    // find some leaf node of mst and assign as root
    node root;

    forall_edges(e, mst) {
        node n = G.source(e);
        int deg = 0;
        edge ae;
        forall_adj_edges(ae, n){
            if (in_mst[ae]) {
                deg++;
            }
        }

        if (deg == 1) {
            root = n;
            break;
        }
    }

    std::vector<std::set<node>> clusters;
    printed = clusters

    // essentially a dfs variant
    std::set<node> cluster = csearch(G, root, mst);
    if (clusters.size() > 0) { // merge with last printed if empty is returned
        std::set<node> last_printed = printed.back();

        std::set<node>::iterator it;
        for (it = printed.begin(); it != printed.end(); ++it) {
            node n = *it;
            last_printed.insert(n);
        }
    }

    clusters = printed;
    // make map from clusters to idx
    node_array<int> cluster_to_index(G);
    int total_clusters = clusters.size();

    for (int i = 0; i < clusters.size(); i++) {
        std::set<node> current_cluster = clusters[i];

        std::set<node>::iterator it;
        for (it = current_cluster.begin(); it != current_cluster.end(); ++it) {
            cluster_to_index[*it] = i;
        }
    }

    map<std::pair<int, int>, edge> min_set_cost;
    edge_array<std::pair<int, int>> edge_set(G);
    // partition edges to E_i,j sets
    // and map E_i,j sets to min cost map
    forall_edges(e, mst) {
        node src = G.source(e);
        node trg = G.target(e);

        std::pair set_descriptor = std::make_pair<cluster_to_index[src], cluster_to_index[trg]>;
        edge_set[e] = set_descriptor;
        if (!min_set_cost.defined(set_descriptor)) {
            min_set_cost[set_descriptor] = e;
        } else if (cost[min_set_cost[set_descriptor]] > cost[e]) {
            min_set_cost[set_descriptor] = e;
        }
    }
    cluster_timer.stop();
    /* END FINDCLUSTERS */

    // edge modifications
    timer per_edge;

    per_edge.start();
    std::vector<std::pair<edge, int>>::iterator it;
    for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
        std::pair<edge, int> pair = *it;
        edge to_update = pair.first;
        int cluster_src = cluster_to_index[G.source(to_update)];
        int cluster_trg = cluster_to_index[G.target(to_update)];

        int new_cost = pair.second;

        bool is_in_mst = in_mst[to_update];
        int prev_cost = cost[to_update];

        if (is_in_mst && prev_cost >= new_cost || !is_in_mst && new_cost > prev_cost) {
            cost[to_update] = new_cost;
            continue;
        }

        // non tree edge decreased
        if (!is_in_mst && new_cost < prev_cost) {
            cost[to_update] = new_cost;
            // add edge to mst
            // find cycle

            // remove max edge cost from there

            // if edge (v, w) is removed and (x, y) added

            // if x and y are in different clusters
            // or x, y, v, in same cluster, continue

            // else
            // split cluster i
            // update all sets with i-j (for j)
            
            // if new clusters got less than z vertices
            // combine with neighbour
            
            // if neighbouring cluster got more than 3z-2 vertices
            // split to 2 clusters using csearch
        }

        // tree edge increased
        if (is_in_mst && new_cost > prev_cost) {
            cost[to_update] = new_cost;

            in_mst[to_update] = false;
            mst.del(to_update);

            // remove from mst
            // find edge that connects the 2 connected components - clusters
            if (cluster_src == cluster_trg) {
                int clust = cluster_src;
                // split clust into 2 clusters
                    // figure out where the 2 clusters split
                    // make one clust A
                    // make one clust B

                    // make clust A = clust

                    // move each clust_i to clust_i + 1, for i > clust
                    // make clust b clust + 1

                    // increment clust size;

                // adjust minimum cost sets by incrementing all > clust by 1
            }

            // do a bfs to find out connected components

            edge edge_min;
            // find edge_min

            // if minimum cost edge is found which is not the same as the one to update
            mst.append(edge_min);
            in_mst[edge_min] = true;
        }
    }
    per_edge.stop();

    make_triplet<mst_t.elapsed_time(), cluster_timer.elapsed_time(), per_edge.elapsed_time()>;
}


void benchmark(const graph &G, edge_array<int> &cost, int n, int m){
    random_source S;

    std::vector<edge> edges;
    edge e;
    forall_edges(e, G) {
        edges.push_back(e);
    }

    float mst_online_preproc = 0;
    float mst_static_preproc = 0;

    float mst_online_time = 0;
    float mst_static_time = 0;  

    float cluster_time = 0;
    float mst_online_per_edge = 0;
    float mst_static_per_edge = 0;

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

            // copy edge stream too - it gets affected by degree_fix()
            std::vector<std::pair<edge, int>> h_cost_updates(cost_updates);

            t.start();
            
            triplet<float, float, float> p = MST_ONLINE(h, h_cost, h_mst, h_cost_updates);

            mst_online_preproc += p.first;
            cluster_time += p.middle;
            elapsed_per_edge += p.last;
            t.stop();

            elapsed_time += t.elapsed_time();
        }

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
            
            triplet<float, float, float> p = MST_STATIC(h, h_cost, h_mst, cost_updates);

            mst_static_preproc += p.first;
            elapsed_per_edge += p.last;

            t.stop();
            elapsed_time += t.elapsed_time();
        }

        mst_static_time += elapsed_time;
        mst_static_per_edge += elapsed_per_edge;
    }

    std::cout << std::endl << std::endl;

    int t = DIFFERENT_EDGE_COST_TIMES + BENCH_TIMES;
    std::cout << "MST_ONLINE: " << (mst_online_time / t) << ", preproc: " << (mst_online_preproc / t) << " , per edge: " << (mst_online_per_edge / t) << " cluster time: " << (cluster_time / t) << std::endl;
    std::cout << "MST_STATIC: " << (mst_static_time / t) << ", preproc: " << (mst_static_preproc / t) << " , per edge: " << (mst_static_per_edge / t) << std::endl;
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