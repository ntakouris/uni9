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
#include "LEDA/core/dictionary.h"

using namespace leda;
// used for h_array<std::pair<int, int>>
namespace leda {
// we only use this for >= 0 integers and they need to be 32 bits
// https://stackoverflow.com/questions/919612/mapping-two-integers-to-one-in-a-unique-and-deterministic-way
int Hash(const std::pair<int, int>& x) 
{
    int a = x.first;
    int b = x.second;
    
    // szudik' s function
    return a >= b ? a * a + a + b : a + b * b;
}
};

#include "LEDA/core/h_array.h"


static int z = 3;

// #define RUN_STATIC
#define RUN_ONLINE 

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
	
    // copy nodes
	forall_nodes(n, G){
        node s = H.new_node();
		copy_in_h[n] = s;
	}

    // copy edges
	forall_edges(e, G){
        node h_src = copy_in_h[G.source(e)];
        node h_trg = copy_in_h[G.target(e)];

        edge h_e = H.new_edge(h_src, h_trg);
        e_copy_in_h[e] = h_e;
	}

    edge_array<int> _cost(H); // re-init the list, nodes were missing on the first one

    // copy-map costs
    forall_edges(e, G) {
        _cost[e_copy_in_h[e]] = cost_g[e];
    }

    cost_h = _cost; // assign the new edge_array

    // map edge update edges to new H edges
    std::vector<std::pair<edge, int>>::iterator it;
    for (it = updates_g.begin(); it != updates_g.end(); ++it) {
        std::pair<edge, int> pair = *it;
        updates_h.push_back(std::make_pair(e_copy_in_h[pair.first], pair.second));
    }
}

triplet<float> MST_STATIC(graph &G, edge_array<int> &cost, std::vector<std::pair<edge,int>> &cost_updates){
    timer mst_t;

    mst_t.start();
    list<edge> mst = MIN_SPANNING_TREE(G, cost);

    edge_array<bool> in_mst(G);
    edge e;

    forall(e, mst) {
        in_mst[e] = true;
    }

    // std::cout << std::endl;
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

    // std::cout << "edge modifications" << std::endl;
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
            // std::cout << "Skip" << std::endl;

            cost[to_update] = new_cost;
            continue;
        }

        if (!is_in_mst && new_cost < prev_cost) {
            // std::cout << "D.findmax" << std::endl;

            cost[to_update] = new_cost;

            in_mst[to_update] = true;
            mst.append(to_update);

            vertex v = vertices[G.source(to_update)];
            vertex w = vertices[G.target(to_update)];

            D.evert(v);

            vertex max_cost_vertex = w;
            int max_cost = new_cost;

            // find edge on tree that got maximum cost on the cycle created
            // std::cout << "findmax in cycle" << std::endl;
            vertex tmp = w; // D.findmax(w)
            vertex tmp_kid = NULL;
            while (tmp != v){ // from w to root v
                vertex parent = D.parent(tmp);

                // D.cost(v) = cost of v -> parent(v)
                int cost = std::round(D.cost(tmp)); // D returns double but we only assign ints

                if (cost > max_cost) {
                    max_cost = cost;
                    max_cost_vertex = tmp; // current one
                }

                if (parent == NULL) {
                    break;
                }

                tmp = parent;
                tmp_kid = tmp;
            }

            // std::cout << its << " end findmax in cycle " << tmp << std::endl;

            // if there is no change, the above part is going to restore the previously deleted edge (in mst)
            // else, its going to add the correct new one
            node corresponding_target = vertex_to_node[D.parent(tmp)];
            node corresponding_source = vertex_to_node[tmp];

            // std::cout << "fix mst " << corresponding_source << " " << corresponding_target << std::endl;
            // idk if there is a more efficient way to get an edge between 
            // a source and target node, if it exists, null otherwise
            edge e;
            forall_adj_edges(e, corresponding_source) {
                if (in_mst[e] && G.opposite(e, corresponding_target) == corresponding_target) {
                    // remove corresponding edge x, y from mst list
                    mst.remove(e);
                    in_mst[e] = false;
                    break;
                }
            }

            if (tmp != v) {
                // std::cout << "D.link " << v << " " << w << " " << new_cost << std::endl;
                D.link(v, w, new_cost);

                // std::cout << "D.cut " << tmp << std::endl;
                D.cut(tmp);
            }

        }

        if (is_in_mst && new_cost > prev_cost) {
            // std::cout << "CC Cycle" << std::endl;

            // temporarily remove current edge from mst
            // to find connected components
            cost[to_update] = new_cost;
            mst.remove(to_update);
            in_mst[to_update] = false;

            // dfs to find connected component
            // std::cout << "find component" << std::endl;
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

            // std::cout << "find min cost edge" << std::endl;
            edge e;
            forall_edges(e, G){
                if (reached[G.source(e)] == reached[G.target(e)]) {
                    continue; // should be in different connected components
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
            // std::cout << "append to mst" << std::endl;

            // add min cost into mst
            mst.append(min_cost_edge);
            in_mst[min_cost_edge] = true;
        }
    }
    per_edge.stop();

    return make_triplet(mst_t.elapsed_time(), (float) 0, per_edge.elapsed_time() / cost_updates.size());
}

// performs the transformation from Go -> G
// so that no vertex got more than 3 degree
// page 4 of original document, page 7 of scanned pdf from purdue university
void degree_fix(graph& G, edge_array<int> &cost, std::vector<std::pair<edge, int>> &cost_updates){
    /* G's original edges to new mapped edges (used for cost_updates) */
    dictionary<edge, edge> edge_map;

    /* use this instead of the cost edge_array, issues when G.new_edge or node is added */
    /* will be mapped to a new cost edge_array in the end */
    dictionary<edge, int> temp_cost;

    edge e;
    forall_edges(e, G){
        temp_cost.insert(e, cost[e]);
    }

    int nodes_added_cnt = 0;
    int edges_added_cnt = 0;

    node anchor;
    //std::cout << "forall anchors" << std::endl;
    forall_nodes(anchor, G) {
        int d = G.outdeg(anchor);
        node w;
        // std::cout << "node: " << anchor << std::endl;
        if (d > 3) { 
            // std::cout << anchor << " : deg > 3 = " << d << std::endl;

            int n_to_add = 3;

            std::vector<node> adjacent_w;
            std::vector<edge> edges_w; // edges_w[i] would be (anchor -> adjacent_w[i])
            std::vector<node> adjacent_v;

            // std::cout << "push adj_edjes" << std::endl;
            forall_adj_edges(e, anchor) {
                edges_w.push_back(e);
                node x = G.opposite(anchor, e);
                // std::cout << "adjacent_w: " << x << std::endl;
                adjacent_w.push_back(x);

                // std::cout << "edge: " << e << std::endl; 
            }

            // replace w with new v
            std::vector<node>::iterator it;
            // std::cout << "make a new node for each adj one" << std::endl;
            for (it = adjacent_w.begin(); it != adjacent_w.end(); ++it) {
                w = *it;
                // add new node v
                node v = G.new_node();
                nodes_added_cnt++;
                adjacent_v.push_back(v);
                // std::cout << "adjacent_v: " << v << std::endl;
            }

            // connect v[i] with v[i+1 mod d] with cost 0
            // std::cout << "connect new nodes " << adjacent_v.size() << std::endl;
            for (int i = 0; i < adjacent_v.size(); i++) {
                int idx_src = i;
                int idx_trg = (i + 1) % adjacent_v.size();

                // std::cout << "src["<< idx_src << "] = ";
                node src = adjacent_v[idx_src];
                // std::cout << src << std::endl;

                // std::cout << "trg[" << idx_trg << "] = ";
                node trg = adjacent_v[idx_trg];
                // std::cout << trg << std::endl;
                
                e = G.new_edge(src, trg);
                edges_added_cnt++;

                // std::cout << "new edge: " << e << ", cost -> " << 0 << std::endl;
                temp_cost.insert(e, 0);
            }

            // replace w[i] -> anchor with w[i] -> v[i] with the same cost
            // std::cout << "update weights" << std::endl;
            for (int i = 0; i < adjacent_v.size(); i++) {
                // std::cout << "original[" << i << "] = " << std::endl;
                edge original = edges_w[i];
                // std::cout << original << std::endl;

                // std::cout << "original cost" << std::endl;

                dic_item dit = temp_cost.lookup(original);
                int c = dit == NULL ? 0 : temp_cost[dit]; // always guaranteed to be non-null though

                // std::cout << "_w[" << i << "] = ";
                node _w = adjacent_w[i];
                // std::cout << _w << " - _v[" << i << "] = ";
                node _v = adjacent_v[i];
                // std::cout << _v << std::endl;

                // std::cout << "adding edge " << std::endl;
                edges_added_cnt++;
                e = G.new_edge(_w, _v);

                // std::cout << "new edge: " << e << ", cost -> " << c << ", dit: " << dit << std::endl;

                temp_cost.insert(e, c);
                // std::cout << "edge map" << std::endl;
                edge_map.insert(original, e);
                // std::cout << "edges_to_delete" << std::endl;
                G.del_edge(original);
                // std::cout << i << " iteration done" << std::endl;
            }

            G.del_node(anchor);
        } else { // if d > 3
            // std::cout << "not deleting" << std::endl;
            
        }
    }


    /* Update cost updates stream based on new edges */
    std::vector<std::pair<edge, int>>::iterator it;
    // std::cout << "update cost changes" << std::endl;
    for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
        std::pair<edge, int> p = *it;
        edge curr = p.first;
        dic_item item = edge_map.lookup(curr);

        if (item != NULL) {
            // std::cout << "from " << p.first << " to " << edge_map[item] << std::endl;
            p.first = edge_map[item];
        }
    }

    // std::cout << "added " << nodes_added_cnt << " nodes and " << edges_added_cnt << " edges" << std::endl;

    // make new edge_array cost
    // to make allocation for deleted new edges
    cost.init(G);

    // int null_dit = 0;
    forall_edges(e, G){
        dic_item dit = temp_cost.lookup(e);
        int c = 0;
        if (dit != NULL) {
            c = temp_cost[dit];
        }
        // else {
        //     null_dit++;
        // }

        cost[e] = c;
        // std::cout << "edge " << e << " with cost " << c << std::endl;
    }

    // std::cout << "null dit lookups: " << null_dit << std::endl;
}

std::set<node> csearch(graph &G, node &v, edge_array<bool> &in_mst, std::vector<std::set<node>> &printed, node_array<bool> &visited){
    // std::cout << "csearch " << v << std::endl;
    std::set<node> cluster;
    cluster.insert(v);
    visited[v] = true;

    // std::cout << "inserting " << std::endl;

    //for all child nodes w of v
    node w;
    edge e;
    forall_adj_edges(e, v) {
        if (in_mst[e]) {
            node w = G.opposite(v, e);
            if (!visited[w]){
                std::set<node> subcluster = csearch(G, w, in_mst, printed, visited);
                // std::cout << "csearch" << v << "subcluster " << w << std::endl;

                std::set<node>::iterator it;
                for (it = subcluster.begin(); it != subcluster.end(); ++it) {
                    node n = *it;
                    cluster.insert(n);
                }
            }
        }
    }

    if (cluster.size() < z) {
        return cluster;
    } else {
        printed.push_back(cluster);
        std::set<node> empty;
        return empty; // supposed returned empty set
    }

}

// builds the mst in a streaming-iterative fashion
triplet<float> MST_ONLINE(graph &G, edge_array<int> &cost, std::vector<std::pair<edge,int>> &cost_updates){
    std::vector<std::set<node>> printed;

    edge e;
    node n;
    // make degree fix and make mst
    timer mst_t;
    mst_t.start();

    std::cout << "Before degree fix: nodes = " << G.number_of_nodes() << " edges = " << G.number_of_edges() << std::endl; 
    degree_fix(G, cost, cost_updates);
    std::cout << "After degree fix: nodes = " << G.number_of_nodes() << " edges = " << G.number_of_edges() << std::endl; 

    list<edge> mst = MIN_SPANNING_TREE(G, cost);

    edge_array<bool> in_mst(G);

    forall_edges(e, G) {
        in_mst[e] = false;
    }

    forall(e, mst) {
        in_mst[e] = true;
    }

    int zero_cost_num = 0;
    int in_mst_num = 0;
    int both = 0;
    forall_edges(e, G){
        int x = 0;
        if (cost[e] == 0) {
            zero_cost_num++;
            x=1;
        }

        if(in_mst[e]){
            in_mst_num++;

            if (x == 1){
                both++;
            }
        }
    }

    std::cout << "in mst: " << in_mst_num << ", zero cost: " << zero_cost_num << " both = " << both << std::endl;

    dynamic_trees D;

    node_array<vertex> vertices(G);
    // generate vertices
    forall_nodes(n, G) {
        vertices[n] = D.make();
    }

    std::vector<std::pair<edge, int>>::iterator it;

    for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
        std::pair<edge, int> pair = *it;

        node p = G.source(pair.first);
        if (vertices[p] == NULL) {
            vertices[p] = D.make();
        }
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

    timer cluster_timer;
    cluster_timer.start();
    /* FINDCLUSTERS */
    // find some leaf node of mst and assign as root
    node root;

    // basically for all nodes, pick the first one that
    // is in mst and got deg 1 (=> is a leaf node)
    forall_nodes(n, G) {
        root = n;

        int deg = 0;
        edge ae;
        forall_adj_edges(ae, n){
            if (in_mst[ae]) {
                deg++;
            }
        }

        if (deg == 1) {
            /* root is now where csearch is going to start */
            break;
        }
    }

    // std::cout << "findclusters: csearch, leaf node: " << root << std::endl;

    node_array<bool> _visited(G);
    node x;
    // int ii = 0;
    forall_nodes(x, G) {
        // ii++;
        // std::cout << "node: " << x << std::endl;
        _visited[x] = false;
    }
    // std::cout << "iterated " << ii << " nodes" << std::endl;

    std::set<node> _cluster = csearch(G, root, in_mst, printed, _visited);
    if (_cluster.size() > 0) { // merge with last printed if non empty is returned
        // std::cout << "non empty returned, merging with last_printed" << std::endl;
        std::set<node> last_printed = printed.back();

        std::set<node>::iterator it;
        for (it = _cluster.begin(); it != _cluster.end(); ++it) {
            node n = *it;
            last_printed.insert(n);
        }
    }

    std::vector<std::set<node>> clusters = printed;

    // std::cout << "map node -> clust idx" << std::endl;
    // make map from each node to cluster idx
    node_array<int> cluster_to_index(G);
    map<int, int> cardinalities; // how many nodes in each cluster
    cardinalities.set_default_value(0);

    int total_clusters = clusters.size();

    int i = 0;
    std::vector<std::set<node>>::iterator cit;
    for (cit = clusters.begin(); cit != clusters.end(); ++cit) {
        std::set<node> current_cluster = *cit;

        std::set<node>::iterator it;
        int __i = 0;
        for (it = current_cluster.begin(); it != current_cluster.end(); ++it) {
            cluster_to_index[*it] = i;
            __i++;
        }

        cardinalities[i] = __i;

        i++;
    }

    int max_clust_idx = i-1; // we use this to allocate subcluster ids later

    // std::cout << "partition edges, find min set cost" << std::endl;

    /* minimum cost of edges connecting cluster idx - cluster idx */
    h_array<std::pair<int, int>, edge> min_set_cost;
    /* cluster idx - cluster idx where each edge belongs to */
    edge_array<std::pair<int, int>> edge_set(G);

    // partition edges to E_i,j sets
    // and map E_i,j sets to min cost map
    forall(e, mst) {
        node src = G.source(e);
        node trg = G.target(e);
        
        int src_idx = cluster_to_index[src];
        int trg_idx = cluster_to_index[trg];

        std::pair<int, int> set_descriptor = std::make_pair(src_idx, trg_idx);
        edge_set[e] = set_descriptor;

        /* undirected graph, always from smaller idx to bigger */

        if (!min_set_cost.defined(set_descriptor)) {
            min_set_cost[set_descriptor] = e;
        } else if (cost[min_set_cost[set_descriptor]] > cost[e]) {
            min_set_cost[set_descriptor] = e;
        }
    }
    cluster_timer.stop();
    // std::cout << "FINDCLUSTERS END" << std::endl;

    // edge modifications
    timer per_edge;

    per_edge.start();

    for (it = cost_updates.begin(); it != cost_updates.end(); ++it) {
        std::pair<edge, int> pair = *it;
        edge to_update = pair.first;
        int cluster_src = cluster_to_index[G.source(to_update)];
        int cluster_trg = cluster_to_index[G.target(to_update)];

        int new_cost = pair.second;

        bool is_in_mst = in_mst[to_update];
        int prev_cost = cost[to_update];

        if (is_in_mst && prev_cost >= new_cost || !is_in_mst && new_cost > prev_cost) {
            // std::cout << "skip" << std::endl;
            cost[to_update] = new_cost;
            continue;
        }

        // non tree edge (v, w)
        // tree edge (x, y) is removed
        if (!is_in_mst && new_cost < prev_cost) { // cost of non tree edge decreased
            std::cout << "dynamic tree for cycle" << std::endl;
            cost[to_update] = new_cost;
            in_mst[to_update] = true;
            mst.append(to_update);

            // find cycle using dynamic tree first
            node _v = G.source(to_update);
            node _w = G.target(to_update);

            std::cout << "_v = " << _v << " _w = " << _w << std::endl;

            /* As discussed in section 2 (dynamic trees) */
            vertex v = vertices[_v];
            vertex w = vertices[_w];

            std::cout << "v = " << v << " w = " << w << std::endl;

            D.evert(v);

            vertex max_cost_vertex = w;
            int max_cost = new_cost;

            // find edge on tree that got maximum cost on the cycle created
            std::cout << "findmax in cycle" << std::endl;
            vertex tmp = w; // D.findmax(w)
            vertex tmp_kid = NULL;
            while (tmp != v){ // from w to root v
                vertex parent = D.parent(tmp);

                // D.cost(v) = cost of v -> parent(v)
                int cost = std::round(D.cost(tmp)); // D returns double but we only assign ints

                if (cost > max_cost) {
                    max_cost = cost;
                    max_cost_vertex = tmp; // current one
                }

                if (parent == NULL) {
                    break;
                }

                tmp = parent;
                tmp_kid = tmp;
            }

            std::cout << "end findmax in cycle: " << tmp << std::endl;

            // if there is no change, the above part is going to restore the previously deleted edge (in mst)
            // else, its going to add the correct new one
            node _x = vertex_to_node[D.parent(tmp)];
            node _y = vertex_to_node[tmp];

            std::cout << "fix mst: tmp = " << tmp << std::endl;
            // idk if there is a more efficient way to get an edge between 
            // a source and target node, if it exists, null otherwise
            edge e;
            forall_adj_edges(e, _x) {
                if (in_mst[e] && G.opposite(e, _x) == _y) {
                    // remove corresponding edge x, y from mst list
                    mst.remove(e);
                    in_mst[e] = false;
                    break;
                }
            }

            if (tmp != v) {
                // std::cout << "D.link " << v << " " << w << " " << new_cost << std::endl;
                D.link(v, w, new_cost);

                // std::cout << "D.cut " << tmp << std::endl;
                D.cut(tmp);
            }
            /* end as discussed in 2 */
            std::cout << "clusters after" << std::endl;

            /* we now know x, y, v, w and the dynamic tree as well as mst is updated */

            // if x and y are in different clusters
            // or x, y == v, w
            // or x, y, v, in same cluster, **continue**
            if ( ((_x == _v && _y == _w) || (_x == _w && _y == _v)) 
                    || cluster_to_index[_x] != cluster_to_index[_y]
                    || (cluster_to_index[_x] == cluster_to_index[_y] 
                            && cluster_to_index[_x] == cluster_to_index[_v]
                            && cluster_to_index[_x] == cluster_to_index[_w])){
                std::cout << "no cluster changes" << std::endl;
                continue; /* no cluster changes */
            }

            /*** else (x, y in same cluster without v or without w) ***/
            // split cluster i on (x, y) to Vi' and Vi''
            std::cout << "split cluster to vi', vi''" << std::endl;

            /* CLUSTER SPLIT */
            int clust = cluster_src;
            // split clust into 2 clusters
            
            // 2 dfs to find out each subcluster
            // (from src and trg), using mst edges only
            std::vector<node> clust_i;
            std::vector<node> clust_ii;

            node_array<int> clustid(G); // init = 0 (all nodes), clust i = 1, clust ii = 2
            forall_nodes(n, G) {
                clustid[n] = 0;
            }
            
            std::cout << "clust i dfs" << std::endl;
            list<node> stack;
            node s;
            stack.push(G.source(to_update)); 
            while (!stack.empty()) 
            { 
                s = stack.pop(); 

                if (clustid[s] == 1) 
                { 
                    continue;
                } 
        
                clustid[s] = 1;
                clust_i.push_back(s);

                edge e;
                forall_adj_edges(e, s) {
                    if (in_mst[e] && cluster_to_index[n] == clust) { // search only for edges inside mst
                        node k = G.opposite(e, s);
                        stack.push(k);
                    }
                }
            }

            std::cout << "clust ii dfs" << std::endl;

            stack.clear();
            stack.push(G.target(to_update)); 
            while (!stack.empty()) 
            { 
                s = stack.pop(); 
        
                if (clustid[s] == 2) 
                { 
                    continue;
                } 
        
                clustid[s] = 2;
                clust_ii.push_back(s);

                edge e;
                forall_adj_edges(e, s) {
                    if (in_mst[e] && cluster_to_index[n] == clust) { // search only for edges inside mst
                        node k = G.opposite(e, s);
                        stack.push(k);
                    }
                }
            }

            // then, split to subclusters Vi' Vi''
            // Vi gets the old cluster id
            // vii gets a new id
            int vi_idx = clust;
            int vii_idx = ++max_clust_idx;

            std::cout << "clust i id: " << vi_idx << " - clust ii id: " << vii_idx << std::endl;

            std::cout << "updating clust ids" << std::endl;

            // update clust idx
            std::vector<node>::iterator it;
            // update node cluster ids just for the second cluster whose id changed
            for(it = clust_ii.begin(); it != clust_ii.end(); ++it){
                node _n = *it;
                cluster_to_index[_n] = vii_idx;
                edge __e;
                forall_adj_edges(__e, _n) {
                    int __trg_idx = cluster_to_index[G.opposite(_n, __e)];
                    edge_set[__e] = std::make_pair(vii_idx, __trg_idx);
                }
            }

            std::cout << "invalidate edge pair min costs" << std::endl;
            // remove - invalidate edge pair min costs
            std::vector<std::pair<int, int>> descriptors_to_undefine;
            std::pair<int, int> _d;
            forall_defined(_d, min_set_cost){
                if (_d.first == clust || _d.second == clust) {
                    descriptors_to_undefine.push_back(_d);
                }
            }

            std::vector<std::pair<int, int>>::iterator d_it;
            for(d_it = descriptors_to_undefine.begin(); d_it != descriptors_to_undefine.end(); ++d_it){
                min_set_cost.undefine(*d_it);
            }

            std::cout << "updating clust cardinalities" << std::endl;

            // update cardinalities
            cardinalities[vi_idx] = clust_i.size();
            cardinalities[vii_idx] = clust_ii.size();

            std::cout << "updating min_set_cost" << std::endl;
            // update min_set_cost (minimum cost between clusters)
            forall(e, mst) {
                node src = G.source(e);
                node trg = G.target(e);
                
                int src_idx = cluster_to_index[src];
                int trg_idx = cluster_to_index[trg];

                // if it belongs to the cluster we split
                if (src_idx == vi_idx || src_idx == vii_idx || trg_idx == vi_idx || trg_idx == vii_idx){
                    std::pair<int, int> set_descriptor = std::make_pair(src_idx, trg_idx);
                    edge_set[e] = set_descriptor;

                    if (!min_set_cost.defined(set_descriptor)) {
                        min_set_cost[set_descriptor] = e;
                    } else if (cost[min_set_cost[set_descriptor]] > cost[e]) {
                        min_set_cost[set_descriptor] = e;
                    }
                }
            }

            // update cluster_src and cluster_trg variables
            cluster_src = vi_idx;
            cluster_trg = vii_idx;

            /* END CLUSTER SPLIT */
            int clust_size = clust_i.size();
            clust = vi_idx;

            int neighbouring_clust = -1;

            std::cout << "end cluster split" << std::endl;
            if (clust_size < z) { // if new clusters got less than z vertices
                // find one cluster that clust is connected to
                forall(e, mst) {
                    std::pair<int, int> pair = edge_set[e];
                    if (pair.first == clust) {
                        neighbouring_clust = pair.second;
                        break;
                    }

                    if (pair.second == clust) {
                        neighbouring_clust = pair.first;
                        break;
                    }
                }

                /* BEGIN merge cluster {clust} with {neighbouring_clust} */

                // update cardinalities
                cardinalities[neighbouring_clust] = cardinalities[neighbouring_clust] + cardinalities[clust];
                cardinalities[clust] = 0;

                // update edge_set
                forall(e, mst) {
                    std::pair<int, int> pair = edge_set[e];
                    if (pair.first == clust) {
                        pair.first = neighbouring_clust;
                        continue;
                    }

                    if (pair.second == clust) {
                        pair.second = neighbouring_clust;
                        continue;
                    }
                }

                // update min_set_cost
                std::pair<int, int> _p;
                forall_defined(_p, min_set_cost){

                    if (_p.first == clust) {
                        edge min_set_edge = min_set_cost[_p];
                        int __cost = cost[min_set_edge];
                        
                        // if not defined neighbour - p.second or p.second - neighbour or less cost, update
                    }

                    if (_p.second == clust) {
                        edge min_set_edge = min_set_cost[_p];
                        int __cost = cost[min_set_edge];   

                        // if not defined p.first - neighbour or p.first - neighbour or less cost, update
                    }
                }

                /* END merge cluster */

                if (cardinalities[neighbouring_clust] > 3 * z - 2) {
                    /* CSEARCH SPLIT */

                    /* END CSEARCH SPLIT */
                }
            }
       }

        // tree edge (x, y)
        // replacement (v, w)
        if (is_in_mst && new_cost > prev_cost) { // tree edge cost increased
            std::cout << "Cluster CC" << std::endl;

            cost[to_update] = new_cost;

            in_mst[to_update] = false;
            mst.remove(to_update);

            if (cluster_src == cluster_trg) {
                /* CLUSTER SPLIT */
                int clust = cluster_src;
                // split clust into 2 clusters
                
                // 2 dfs to find out each subcluster
                // (from src and trg), using mst edges only
                std::vector<node> clust_i;
                std::vector<node> clust_ii;

                node_array<int> clustid(G); // init = 0 (all nodes), clust i = 1, clust ii = 2
                forall_nodes(n, G) {
                    clustid[n] = 0;
                }
                
                list<node> stack;
                node s;
                stack.push(G.source(to_update)); 
                while (!stack.empty()) 
                { 
                    s = stack.pop(); 

                    if (clustid[s] == 1) 
                    { 
                        continue;
                    } 
            
                    clustid[s] = 1;
                    clust_i.push_back(s);

                    edge e;
                    forall_adj_edges(e, s) {
                        if (in_mst[e] && cluster_to_index[n] == clust) { // search only for edges inside mst
                            node k = G.opposite(e, s);
                            stack.push(k);
                        }
                    }
                }

                stack.clear();
                stack.push(G.target(to_update)); 
                while (!stack.empty()) 
                { 
                    s = stack.pop(); 
            
                    if (clustid[s] == 2) 
                    { 
                        continue;
                    } 
            
                    clustid[s] = 2;
                    clust_ii.push_back(s);

                    edge e;
                    forall_adj_edges(e, s) {
                        if (in_mst[e] && cluster_to_index[n] == clust) { // search only for edges inside mst
                            node k = G.opposite(e, s);
                            stack.push(k);
                        }
                    }
                }

                // then, split to subclusters Vi' Vi''
                // Vi gets the old cluster id
                // vii gets a new id
                int vi_idx = clust;
                int vii_idx = ++max_clust_idx;

                // update clust idx
                std::vector<node>::iterator it;
                // update node cluster ids just for the second cluster whose id changed
                for(it = clust_ii.begin(); it != clust_ii.end(); ++it){
                    node _n = *it;
                    cluster_to_index[_n] = vii_idx;
                    edge __e;
                    forall_adj_edges(__e, _n) {
                        int __trg_idx = cluster_to_index[G.opposite(_n, __e)];
                        edge_set[__e] = std::make_pair(vii_idx, __trg_idx);
                    }
                }

                // remove - invalidate edge pair min costs
                std::vector<std::pair<int, int>> descriptors_to_undefine;
                std::pair<int, int> _d;
                forall_defined(_d, min_set_cost){
                    if (_d.first == clust || _d.second == clust) {
                        descriptors_to_undefine.push_back(_d);
                    }
                }

                std::vector<std::pair<int, int>>::iterator d_it;
                for(d_it = descriptors_to_undefine.begin(); d_it != descriptors_to_undefine.end(); ++d_it){
                    min_set_cost.undefine(*d_it);
                }

                // update min_set_cost (minimum cost between clusters)
                forall(e, mst) {
                    node src = G.source(e);
                    node trg = G.target(e);
                    
                    int src_idx = cluster_to_index[src];
                    int trg_idx = cluster_to_index[trg];

                    // if it belongs to the cluster we split
                    if (src_idx == vi_idx || src_idx == vii_idx || trg_idx == vi_idx || trg_idx == vii_idx){
                        std::pair<int, int> set_descriptor = std::make_pair(src_idx, trg_idx);
                        edge_set[e] = set_descriptor;

                        if (!min_set_cost.defined(set_descriptor)) {
                            min_set_cost[set_descriptor] = e;
                        } else if (cost[min_set_cost[set_descriptor]] > cost[e]) {
                            min_set_cost[set_descriptor] = e;
                        }
                    }
                }

                // update cluster_src and cluster_trg variables
                cluster_src = vi_idx;
                cluster_trg = vii_idx;
            }

            // do a bfs to find out connected component cluster ids
            // (split with the x, y edge removed)
            node_array<bool> cc(G); // 2 connected components: true and false
            forall_nodes(n, G) {
                cc[n] = false;
            }

            list<node> stack;
            node s;
            stack.push(G.source(to_update)); 
            while (!stack.empty()) 
            { 
                s = stack.pop(); 
        
                if (cc[s]) 
                { 
                    continue;
                } 
        
                cc[s] = true;
                edge e;
                forall_adj_edges(e, s) {
                    if (in_mst[e]) { // search only for edges inside mst
                        node k = G.opposite(e, s);
                        stack.push(k);
                    }
                }
            }

            // find out the minimum edge that connects any cluster
            // where i and j belongs to different CC
            edge edge_min = NULL;
            int _c = 0;

            forall_edges(e, G) {
                std::pair<int, int> cluster_indices = edge_set[e];

                int cl_a = cluster_indices.first;
                int cl_b = cluster_indices.second;

                node _src = G.source(e);
                node _trg = G.target(e);

                if (cc[_src] != cc[_trg]) {
                    if (edge_min == NULL) {
                        edge_min = e;
                        _c = cost[e];
                        continue;
                    }

                    int _curr_cost = cost[e];
                    if (_curr_cost < _c) {
                        _c = _curr_cost;
                        edge_min = e;
                    }
                }
            }

            std::pair<int, int> edge_min_clusters = edge_set[edge_min];
            int edge_min_a = edge_min_clusters.first;
            int edge_min_b = edge_min_clusters.second;

            // update minimun distances between clusters (min_set_cost)
            std::pair<int, int> descriptor = std::make_pair(edge_min_a, edge_min_b);
            if(min_set_cost.defined(descriptor)) {
                int _curr_cost = cost[min_set_cost[descriptor]];

                if (_c < _curr_cost) {
                    min_set_cost[descriptor] = edge_min;
                }
            }else{
                min_set_cost[std::make_pair(edge_min_a, edge_min_b)] = edge_min;
            }
            
            // update mst
            mst.append(edge_min);
            in_mst[edge_min] = true;
        }
    }
    per_edge.stop();

    return make_triplet(mst_t.elapsed_time(), cluster_timer.elapsed_time(), per_edge.elapsed_time());
}


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
        int s = m * 2; // how many random edge-cost updates to do

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

        // std::cout << std::endl << "cost_updates len: " << cost_updates.size() << std::endl;

        // bechmarks    
        elapsed_time = 0;
        elapsed_per_edge = 0;
        timer t;
        #ifdef RUN_ONLINE
        for (int i = 0; i < BENCH_TIMES; i++) {
            std::cout << "MST_ONLINE #" << i << std::endl;
            // copy mst, G and edge costs so that there is a fresh copy
            graph h;
            edge_array<int> h_cost(h);
            std::vector<std::pair<edge, int>> h_cost_updates;
            copy_graph(G, h, cost, h_cost, cost_updates, h_cost_updates);

            t.start();
            
            triplet<float> p = MST_ONLINE(h, h_cost, h_cost_updates);

            mst_online_preproc += p.first;
            cluster_time += p.middle;
            elapsed_per_edge += p.last;
            t.stop();

            elapsed_time += t.elapsed_time();
        }
        #endif

        mst_online_time += elapsed_time;
        mst_online_per_edge += elapsed_per_edge;

        /* mst static */
        elapsed_time = 0;
        elapsed_per_edge = 0;

        #ifdef RUN_STATIC
        for (int i = 0; i < BENCH_TIMES; i++) {
            std::cout << "MST_STATIC #" << i << std::endl;
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
        #endif

        mst_static_time += elapsed_time;
        mst_static_per_edge += elapsed_per_edge;
    }

    std::cout << std::endl << std::endl;

    int t = DIFFERENT_EDGE_COST_TIMES * BENCH_TIMES;
    #ifdef RUN_ONLINE
    std::cout << "MST_ONLINE: " << (mst_online_time / t) << ", preproc: " << (mst_online_preproc / t) << " , per edge: " << (mst_online_per_edge / t) << " cluster time: " << (cluster_time / t) << std::endl;
    #endif

    #ifdef RUN_STATIC
    std::cout << "MST_STATIC: " << (mst_static_time / t) << ", preproc: " << (mst_static_preproc / t) << " , per edge: " << (mst_static_per_edge / t) << std::endl;
    #endif
}

int tests()
{
    random_source S;

    std::cout << "Simple Graphs" << std::endl << std::endl;

    int n_sizes[] = {10};
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

        std::cout << "Random Simple Graph (" << n << ", " << m << ") " << std::endl;
        benchmark(g, cost, n, m);

    }

    std::cout << "Grid Graphs" << std::endl << std::endl;

    std::cout << "Fully Connected Graphs" << std::endl << std::endl;


    return 0;
}

int main(){
    tests();
    return 0;
}