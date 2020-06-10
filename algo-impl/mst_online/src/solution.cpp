#define BENCH_TIMES 1 // how many runs per edge cost updates that get generated (per graph)
#define DIFFERENT_EDGE_COST_TIMES 1 // how many times to make different edge cost update stream

#include <math.h>
#include <vector>
#include <stack>
#include <set>
#include <algorithm>
#include <utility>

#include "LEDA/graph/graph.h"
#include "LEDA/graph/graph_misc.h"
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

// copies a leda graph to another, edge costs included
void copy_graph(graph& G, graph& H, edge_array<int>& cost_g, edge_array<int> &cost_h, 
                    std::vector<std::pair<edge, int>> &updates_g, std::vector<std::pair<edge, int>> &updates_h){
	node_array<node> copy_in_h(G);
    edge_array<edge> e_copy_in_h(G);

	edge e;
	node n;
	
    //std::cout << "copying nodes" << std::endl;
	forall_nodes(n, G){
        node s = H.new_node();
		copy_in_h[n] = s;
	}

    //std::cout << "copying edges" << std::endl;
	forall_edges(e, G){
        node h_src = copy_in_h[G.source(e)];
        node h_trg = copy_in_h[G.target(e)];

        edge h_e = H.new_edge(h_src, h_trg);
        e_copy_in_h[e] = h_e;
	}

    edge_array<int> _cost(H); // re-init the list, nodes were missing on the first one

    //std::cout << "copying edge costs" << std::endl;
    forall_edges(e, G) {
        _cost[e_copy_in_h[e]] = cost_g[e];
    }

    cost_h = _cost; // assign new edge_array

    //std::cout << "copying edge cost updates" << std::endl;
    std::vector<std::pair<edge, int>>::iterator it;
    for (it = updates_g.begin(); it != updates_g.end(); ++it) {
        std::pair<edge, int> pair = *it;
        updates_h.push_back(std::make_pair(e_copy_in_h[pair.first], pair.second));
    }
}

triplet<float> MST_STATIC(const graph &G, edge_array<int> &cost, std::vector<std::pair<edge,int>> &cost_updates){
    std::cout << "MST_STATIC" << std::endl;
    // make mst
    timer mst_t;

    mst_t.start();
    list<edge> mst = MIN_SPANNING_TREE(G, cost);

    edge_array<bool> in_mst(G);
    edge e;

    forall(e, mst) {
        in_mst[e] = true;
    }

    std::cout << std::endl;
    // build dynamic tree
    dynamic_trees D;

    node_array<vertex> vertices(G);
    node n;
    // generate vertices
    forall_nodes(n, G) {
        vertices[n] = D.make();
    }

    map<vertex, node> vertex_to_node;

    // construct dynamic tree
    forall(e, mst) {
        node source = G.source(e);
        node target = G.target(e);

        vertex vs = vertices[source];
        vertex vt = vertices[target];

        vertex_to_node[vs] = source;
        vertex_to_node[vt] = target;

        D.link(vs, vt, cost[e]);
    }
    mst_t.stop();

    // edge modifications
    timer per_edge;

    std::cout << "edge modifications" << std::endl;
    per_edge.start();
    std::vector<std::pair<edge, int>>::iterator it;
    int i = 0;
    for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {

        std::pair<edge, int> pair = *it;
        edge to_update = pair.first;
        int new_cost = pair.second;
        bool is_in_mst = in_mst[to_update];
        int prev_cost = cost[to_update];

        if (is_in_mst && prev_cost >= new_cost || !is_in_mst && new_cost > prev_cost) {
            std::cout << "Skip" << std::endl;

            cost[to_update] = new_cost;
            continue;
        }

        if (!is_in_mst && new_cost < prev_cost) {
            std::cout << "D.findmax" << std::endl;

            cost[to_update] = new_cost;
            in_mst[to_update] = true;
            mst.append(to_update);

            vertex v = vertices[G.source(to_update)];
            vertex w = vertices[G.target(to_update)];

            std::cout << "link(" << v << "," << w << "," << new_cost << ")" << std::endl;
            D.link(v, w, new_cost);

            std::cout << "evert(" << v << ")" << std::endl;
            D.evert(v);

            vertex max_cost_vertex = w;
            int max_cost = new_cost;

            // find edge on tree that got maximum cost on the cycle created
            std::cout << "findmax in cycle" << std::endl;
            vertex tmp = w; // D.findmax(w)
            while (tmp != v){ // crash when no parent found -> graph not complete
                vertex parent = D.parent(tmp);
                // D.cost(v) = cost of v -> parent(v)
                int cost = std::round(D.cost(tmp)); // D returns double but we only assign ints

                if (cost > max_cost) {
                    max_cost = cost;
                    max_cost_vertex = tmp; // current one
                }

                tmp = parent;
            }

            node corresponding_target = vertex_to_node[D.parent(tmp)];
            node corresponding_source = vertex_to_node[tmp];

            edge e;
            std::cout << "cut the edge" << std::endl;
            // idk if there is a more efficient way to get an edge between 
            // a source and target node, if it exists, null otherwise
            forall_adj_edges(e, corresponding_source) {
                if (in_mst[e] && G.opposite(e, corresponding_target) == corresponding_target) {
                    // remove corresponding edge x, y from mst
                    mst.remove(e);
                    in_mst[e] = false;
                    break;
                }
            }

            // remove edge from dynamic tree too
            D.cut(tmp); // cuts (v -> parent(v))
        }

        if (is_in_mst && new_cost > prev_cost) {
            std::cout << "CC Cycle" << std::endl;

            // temporarily remove current edge from mst
            cost[to_update] = new_cost;
            mst.remove(to_update);
            in_mst[to_update] = false;

            // dfs to find connected component
            std::cout << "find component" << std::endl;
            node_array<bool> reached(G);
            list<node> stack;
            node s;
            stack.push(G.source(to_update)); 
            while (!stack.empty()) 
            { 
                s = stack.pop(); 
        
                if (reached[s]) 
                { 
                    continue;
                } 
        
                reached[s] = true;
                edge e;
                forall_adj_edges(e, s){
                    if (in_mst[e]) { // search only for edges inside mst
                        node k = G.opposite(e, s);
                        stack.push(k);
                    }
                }
            } 
            
            edge min_cost_edge;
            int min_cost = -1;

            std::cout << "find min cost edge" << std::endl;
            edge e;
            forall_edges(e, G){
                if (reached[G.source(e)] == reached[G.target(e)]) {
                    continue; // should be in different connected component
                }
                
                if (min_cost == -1) {
                    min_cost = cost[e];
                    min_cost_edge = e;
                    continue;
                }

                int cost_e = cost[e];
                if (cost_e < min_cost) {
                    min_cost = cost_e;
                    min_cost_edge = e;
                }
            }
            std::cout << "append to mst" << std::endl;

            // add min cost into mst
            mst.append(min_cost_edge);
            in_mst[min_cost_edge] = true;
            
            // do not forget to update-link dynamic tree too
            if (min_cost_edge != to_update){
                // update only if it is a different edge
                // cut to_update
                // => cut (v -> w)
                vertex v = vertices[G.source(to_update)];
                vertex w = vertices[G.target(to_update)];

                // cut the v->w corresponding in D
                if (D.root(v) != v && D.parent(v) == w){
                    D.cut(v);
                    std::cout << "cut v" << std::endl;
                } else if(D.root(w) != w && D.parent(w) == v){
                    D.cut(w);
                    std::cout << "cut w" << std::endl;
                } else {
                    D.evert(v);
                    
                    if (D.parent(w) == v) {
                        D.cut(w);                    
                        std::cout << "evert v cut w" << std::endl;
                    } else {
                        std::cout << "evert w cut v" << std::endl;

                        D.evert(w);
                        D.cut(v);
                    }
                }

                // add the new one
                std::cout << "link mce" << std::endl;
                D.link(vertices[G.source(min_cost_edge)], vertices[G.target(min_cost_edge)], min_cost);
            }
        }
    }
    per_edge.stop();

    std::cout << "return MST_STATIC" << std::endl;

    return make_triplet(mst_t.elapsed_time(), (float) 0, per_edge.elapsed_time());
}

// performs the transformation from Go -> G
// so that no vertex got more than 3 degree
// page 4 of original document, page 7 of scanned pdf from purdue university
// we dont care about costs, we just want to fix the degrees
// we will assign random costs later
// void degree_fix(const graph& G, edge_array<int> &cost, std::vector<std::pair<edge, int>> &cost_updates){
//     std::vector<node> to_delete;

//     node anchor;
//     forall_nodes(anchor, G) {
//         int d = G.degree(anchor);
//         if (d > 3) { 
//             int n_to_add = 3;

//             std::vector<node> adjacent_w;
//             std::vector<node> edges_w;
//             std::vector<node> adjacent_v;
//             node w;
//             edge e;
//             forall_adj_edges(e, anchor) {
//                 edges_w.push_back(e);
//                 adjacent_w.push_back(G.opposite(anchor, e));
//             }

//             // replace w with new v
//             std::vector<node>::iterator it;
//             for (it = adjacent_w.begin(); it != adjacent_w.end(); ++it) {
//                 w = *it;
//                 // add new node v
//                 node v = G.new_node();
//                 adjacent_v.push_back(v);
//             }

//             // connect v[i] with v[i+1 mod d] with cost 0
//             for (int i = 0; i < d; i++) {
//                 int idx_src = i;
//                 int idx_trg = (i + 1) % d;
//                 node src = adjacent_v[idx_src];
//                 node trg = adjacent_v[idx_trg];
//                 e = G.new_edge(src, trg);

//                 cost[e] = 0;
//             }

//             // add w[i] -> anchor with w[i] -> v[i] with the same cost
//             for (int i = 0; i < d; i++) {
//                 edge original = edges_w[i];
//                 int c = cost[original];
//                 e = G.new_edge(adjacent_w[i], adjacent_v[i]);

//                 cost[e] = c;

//                 /* Update cost changes stream */
//                 std::vector<std::pair<edge, int>>::iterator it;
//                 for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
//                     std::pair<edge, int> p = *it;

//                     if (p.first == original) {
//                         p.first = e;
//                     }
//                 }
//             }

//             // remove adjacent_w edges and anchor
//             for (int i = 0; i < d; i++) {
//                 G.hide_edge(edges_w[i]); // you could also hide
//             }

//             G.hide_node(anchor); //otherwise possible iteration exception? idk
//         }
//     }
// }

// static std::vector<std::set<node>> printed;

// std::set<node> csearch(const graph &G, node &v, edge_array<bool> &in_mst){
//     std::set<node> cluster;
//     cluster.insert(v);
//     node w;
//     //for all child nodes w of v
//     edge e;
//     forall_adj_edges(e, v) {
//         if (in_mst[e]) {
//             node w = G.opposite(v, e);

//             std::set<node> subcluster = csearch(G, w, in_mst);
//             std::set<node>::iterator it;
//             for (it = subcluster.begin(); it != subcluster.end(); ++it) {
//                 node n = *it;
//                 cluster.insert(n);
//             }
//         }
//     }

//     if (cluster.size() < z) {
//         return cluster;
//     } else {
//         printed = cluster;
//         std::set<node> empty;
//         return empty; // supposed returned empty set
//     }

// }

// builds the mst in a streaming-iterative fashion
// triplet<float> MST_ONLINE(const graph &G, edge_array<int> &cost, std::vector<std::pair<edge,int>> &cost_updates){
//     // make degree fix and make mst
//     timer mst_t;
//     mst_t.start();
//     degree_fix(G, cost, cost_updates);
//     list<edge> mst = MIN_SPANNING_TREE(G, cost);
//     mst_t.stop();

//     edge_array<bool> in_mst(G);
//     edge e;
//     forall(e, mst) {
//         in_mst[e] = true;
//     }

//     timer cluster_timer;
//     cluster_timer.start();
//     /* FINDCLUSTERS */
//     // find some leaf node of mst and assign as root
//     node root;

//     forall(e, G) {
//         node n = G.source(e);
//         int deg = 0;
//         edge ae;
//         forall_adj_edges(ae, n){
//             if (in_mst[ae]) {
//                 deg++;
//             }
//         }

//         if (deg == 1) {
//             root = n;
//             break;
//         }
//     }

//     std::vector<std::set<node>> clusters;
//     printed = clusters

//     // essentially a dfs variant
//     std::set<node> cluster = csearch(G, root, in_mst);
//     if (clusters.size() > 0) { // merge with last printed if empty is returned
//         std::set<node> last_printed = printed.back();

//         std::set<node>::iterator it;
//         for (it = printed.begin(); it != printed.end(); ++it) {
//             node n = *it;
//             last_printed.insert(n);
//         }
//     }

//     clusters = printed;
//     // make map from clusters to idx
//     node_array<int> cluster_to_index(G);
//     int total_clusters = clusters.size();

//     for (int i = 0; i < clusters.size(); i++) {
//         std::set<node> current_cluster = clusters[i];

//         std::set<node>::iterator it;
//         for (it = current_cluster.begin(); it != current_cluster.end(); ++it) {
//             cluster_to_index[*it] = i;
//         }
//     }

//     map<std::pair<int, int>, edge> min_set_cost;
//     edge_array<std::pair<int, int>> edge_set(G);
//     // partition edges to E_i,j sets
//     // and map E_i,j sets to min cost map
//     forall(e, mst) {
//         node src = G.source(e);
//         node trg = G.target(e);

//         std::pair<int, int> set_descriptor = std::make_pair(cluster_to_index[src], cluster_to_index[trg]);
//         edge_set[e] = set_descriptor;
//         if (!min_set_cost.defined(set_descriptor)) {
//             min_set_cost[set_descriptor] = e;
//         } else if (cost[min_set_cost[set_descriptor]] > cost[e]) {
//             min_set_cost[set_descriptor] = e;
//         }
//     }
//     cluster_timer.stop();
//     /* END FINDCLUSTERS */

//     // edge modifications
//     timer per_edge;

//     per_edge.start();
//     std::vector<std::pair<edge, int>>::iterator it;
//     for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
//         std::pair<edge, int> pair = *it;
//         edge to_update = pair.first;
//         int cluster_src = cluster_to_index[G.source(to_update)];
//         int cluster_trg = cluster_to_index[G.target(to_update)];

//         int new_cost = pair.second;

//         bool is_in_mst = in_mst[to_update];
//         int prev_cost = cost[to_update];

//         if (is_in_mst && prev_cost >= new_cost || !is_in_mst && new_cost > prev_cost) {
//             cost[to_update] = new_cost;
//             continue;
//         }

//         // non tree edge decreased
//         if (!is_in_mst && new_cost < prev_cost) {
//             cost[to_update] = new_cost;
//             // add edge to mst
//             // find cycle

//             // remove max edge cost from there

//             // if edge (v, w) is removed and (x, y) added

//             // if x and y are in different clusters
//             // or x, y, v, in same cluster, continue

//             // else
//             // split cluster i
//             // update all sets with i-j (for j)
            
//             // if new clusters got less than z vertices
//             // combine with neighbour
            
//             // if neighbouring cluster got more than 3z-2 vertices
//             // split to 2 clusters using csearch
//         }

//         // tree edge increased
//         if (is_in_mst && new_cost > prev_cost) {
//             cost[to_update] = new_cost;

//             in_mst[to_update] = false;
//             mst.del(to_update);

//             // remove from mst
//             // find edge that connects the 2 connected components - clusters
//             if (cluster_src == cluster_trg) {
//                 int clust = cluster_src;
//                 // split clust into 2 clusters
//                     // figure out where the 2 clusters split
//                     // make one clust A
//                     // make one clust B

//                     // make clust A = clust

//                     // move each clust_i to clust_i + 1, for i > clust
//                     // make clust b clust + 1

//                     // increment clust size;

//                 // adjust minimum cost sets by incrementing all > clust by 1
//             }

//             // do a bfs to find out connected components

//             edge edge_min;
//             // find edge_min

//             // if minimum cost edge is found which is not the same as the one to update
//             mst.append(edge_min);
//             in_mst[edge_min] = true;
//         }
//     }
//     per_edge.stop();

//     float mst_time = mst_t.elapsed_time();
//     float cluster_time = cluster_timer.elapsed_time();
//     float per_edge_time = per_edge.elapsed_time(); 

//     make_triplet(mst_time, cluster_time, per_edge_time);
// }


void benchmark(graph &G, edge_array<int> &cost, int n, int m){
    random_source S;
    edge e;

    float elapsed_time = 0;
    float mst_online_preproc = 0;
    float mst_static_preproc = 0;

    float mst_online_time = 0;
    float mst_static_time = 0;  

    float cluster_time = 0;
    float mst_online_per_edge = 0;
    float mst_static_per_edge = 0;
    float elapsed_per_edge = 0;

    // create random stream of edge costs to update
    for (int i = 1; i < DIFFERENT_EDGE_COST_TIMES+1; i++) {
        int s = m * 3; // how many random edge-cost updates to do

        std::vector<std::pair<edge, int>> cost_updates;

        // generate random edge-cost pairs
        //std::cout << "making edge updates: " << std::endl;
        for (int b = 0; b < s; b++){
            int c;
            S >> c;
            int rand_cost = ((c % (EDGE_COST_MAX - EDGE_COST_MIN + 1)) + EDGE_COST_MIN);
            
            cost_updates.push_back(std::make_pair(G.choose_edge(), rand_cost));
            //std::cout << rand_cost << ":" << rand_idx << " ";
        }

        std::cout << std::endl << "cost_updates len: " << cost_updates.size() << std::endl;

    //     // bechmarks    
    //     elapsed_time = 0;
    //     elapsed_per_edge = 0;
    //     timer t;
    //     for (int i = 0; i < BENCH_TIMES; i++) {
    //         // copy mst, G and edge costs so that there is a fresh copy
    //         graph h;
    //         CopyGraph(h, G);
    //         edge_array<int> h_cost(h);
    //         edge e;
    //         forall_edges(e, h) {
    //             // http://www.algorithmic-solutions.info/leda_manual/graph_misc.html
    //             // ^ docs say that h[e] corresponds to G[e]
    //             h_cost[e] = cost[h[e]];
    //         }

    //         // https://www.leda-tutorial.org/en/official/ch02s03s06.html
    //         // ^ copy list
    //         list<edge> h_mst = mst;

    //         // copy edge stream too - it gets affected by degree_fix()
    //         std::vector<std::pair<edge, int>> h_cost_updates(cost_updates);

    //         t.start();
            
    //         triplet<float, float, float> p = MST_ONLINE(h, h_cost, h_mst, h_cost_updates);

    //         mst_online_preproc += p.first;
    //         cluster_time += p.middle;
    //         elapsed_per_edge += p.last;
    //         t.stop();

    //         elapsed_time += t.elapsed_time();
    //     }

    //     mst_online_time += elapsed_time;
    //     mst_online_per_edge += elapsed_per_edge;

        /* mst static */
        elapsed_time = 0;
        elapsed_per_edge = 0;

        timer t;
        for (int i = 0; i < BENCH_TIMES+1; i++) {
            // copy mst, G and edge costs so that there is a fresh copy
            graph h;
            edge_array<int> h_cost(h);
            std::vector<std::pair<edge, int>> h_cost_updates;
            copy_graph(G, h, cost, h_cost, cost_updates, h_cost_updates);

            t.start();
            
            triplet<float> p = MST_STATIC(h, h_cost, h_cost_updates);

            mst_static_preproc += (float) p.first;
            elapsed_per_edge += (float) p.last;

            t.stop();
            elapsed_time += t.elapsed_time();
        }

        mst_static_time += elapsed_time;
        mst_static_per_edge += elapsed_per_edge;
    }

    std::cout << std::endl << std::endl;

    int t = DIFFERENT_EDGE_COST_TIMES * BENCH_TIMES;
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
        random_simple_loopfree_graph(g, n, m);
        Make_Connected(g);
        g.make_undirected();

        edge_array<int> cost(g);
        edge e;
        forall_edges(e, g) {
            int c;
            S >> c;
            cost[e] = ((c % (EDGE_COST_MAX - EDGE_COST_MIN + 1)) + EDGE_COST_MIN);
        }

        z = max(1.0, pow(m, 2/5)); // see paper

        std::cout << "Random Graph (" << n << ", " << m << ") " << std::endl;
        benchmark(g, cost, n, m);
    }

    return 0;
}

int main(){
    tests();
    return 0;
}