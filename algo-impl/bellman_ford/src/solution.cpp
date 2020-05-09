#define V_PLUS 0
#define V_MINUS 1
#define V_F 2

#define SKIP_RANDOM_GRAPHS 1
#define SKIP_GRID_GRAPHS 0

#include <cmath>
#include <utility>
#include <random>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/leda_graph.hpp>

#include "LEDA/graph/graph.h"
#include "LEDA/core/random_source.h"
#include "LEDA/core/p_queue.h"
#include "LEDA/graph/node_partition.h"
#include "LEDA/graph/graph_alg.h"
#include "LEDA/graph/graph_gen.h"
#include "LEDA/system/timer.h"
#include "LEDA/core/dynamic_trees.h"
#include "LEDA/graph/min_span.h"

#include <ctime>
#include <cstdlib>

typedef leda::node node;
typedef leda::edge edge;

typedef struct NodeInfo
{
    int distance; // was unsigned int
    bool visited;
    int parent;
    int label;
} NodeInfo;

typedef struct EdgeInfo
{
    int weight;
} EdgeInfo;

typedef leda::graph LGraph;
using leda::edge_array;
using leda::node_array;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS, NodeInfo, EdgeInfo> BGraph;
typedef boost::graph_traits<BGraph>::vertex_descriptor vertex;
typedef boost::graph_traits<BGraph>::edge_descriptor arc;
typedef boost::graph_traits<BGraph>::edge_iterator arcIt;
typedef boost::graph_traits<BGraph>::vertex_iterator vertexIt;
typedef boost::graph_traits<BGraph>::out_edge_iterator outArcIt;
typedef boost::graph_traits<BGraph>::adjacency_iterator aIt;

typedef boost::property_map<BGraph, int EdgeInfo::*>::type WeightPrMap;
typedef boost::property_map<BGraph, int NodeInfo::*>::type DistanceMap; // was unsigned int
typedef boost::property_map<BGraph, bool NodeInfo::*>::type VisitedMap;
typedef boost::property_map<BGraph, int NodeInfo::*>::type LabelMap;
typedef boost::property_map<BGraph, int NodeInfo::*>::type ParentMap;

void copy_graph(const LGraph &LG, BGraph &BG, edge_array<int> &l_weight, WeightPrMap &b_weight,
                node_array<NodeInfo> &nodeInfo, DistanceMap &b_dist, VisitedMap &b_visit,
                LabelMap &b_label, ParentMap &b_parent);

void tests()
{
    leda::random_source S;
    if (SKIP_RANDOM_GRAPHS == 0){
    std::cout << "Random Weakly Connected Graphs" << std::endl << std::endl;

    /* random, at least weakly connected graphs */
    int n[] = {1000, 4000, 8000};
    for (int i = 0; i < 3; i++)
    {
        int m = 20 * n[i] * ((int)log10(n[i]));

        std::cout << "Weakly Connected (" << n[i] << "," << m << ")" << std::endl;
        std::cout << "Constructing Graph" << std::endl;
        LGraph LG;
        edge_array<int> l_weight(LG);

        random_graph(LG, n[i], m);

        Make_Connected(LG);

        l_weight.init(LG);
        edge e;
        forall_edges(e, LG)
        {
            l_weight[e] = (S() % (10000 + 100)) - 100;
        }

        node_array<NodeInfo> nodeInfo(LG);
        node v;
        int i = 0;
        forall_nodes(v, LG)
        {   
            nodeInfo[v].visited = false;
            nodeInfo[v].distance = std::numeric_limits<int>::max();
        }

        node s = LG.choose_node();
        nodeInfo[s].distance = 0; // start node  - needed for boost

        BGraph BG;
        WeightPrMap b_weight = get(&EdgeInfo::weight, BG);
        DistanceMap b_dist = get(&NodeInfo::distance, BG);
        VisitedMap b_visit = get(&NodeInfo::visited, BG);
        LabelMap b_label = get(&NodeInfo::label, BG);
        ParentMap b_parent = get(&NodeInfo::parent, BG);
        vertex b_s;

        std::cout << "Copying to BGL" << std::endl;
        copy_graph(LG, BG, l_weight, b_weight, nodeInfo, b_dist, b_visit, b_label, b_parent);

        std::cout << "Running Benchmarks" << std::endl;

        // run leda benchmark
        std::cout << "LEDA" << std::endl;
        node_array<edge> pred(LG);
	    node_array<int> distance(LG);
        bool r = BELLMAN_FORD_B_T(LG,s,l_weight,distance,pred);
        std::cout << "r = " << r << std::endl;
        // run boost benchmark

        if (r == 1){
        std::cout << "BOOST" << std::endl;
        r = bellman_ford_shortest_paths(BG, n, weight_map(b_weight).
                                distance_map(b_dist).
                                predecessor_map(b_parent));
        std::cout << "r = " << r << std::endl;
        } else {
            std::cout << "not running for boost because leda found negative cycle" << std::endl;
        }
        // run our benchmark
    }
    }

    /* grid graphs */
    if(SKIP_GRID_GRAPHS == 0) {
    std::cout << "Grid Graphs" << std::endl << std::endl;
    int grid_n[] = {100, 200, 300};
    for (int l = 0; l < 3; l++)
    {
        int k = grid_n[l]; // grid size
        std::cout << "Grid (" << k << "," << k << ")" << std::endl;
        std::cout << "Constructing Graph" << std::endl;

        int num_nodes = k * k;

        LGraph LG;

        leda::node node_tracker[num_nodes];
        std::vector<int> weights;
        // populate nodes
        for (int x = 0; x < k; x++)
        {
            for (int y = 0; y < k; y++)
            {
                int cur = x * k + y;
                node_tracker[cur] = LG.new_node();
            }
        }

        // populate edges
        for (int x = 0; x < k; x++)
        {
            for (int y = 0; y < k; y++)
            {
                // node index calculation
                int cur = x * k + y;

                if (y < k / 2)
                { // top part | bottom and right
                    if (x != k - 1)
                    { // not rightmost limit
                        // connect to right (x, y) -> (x + 1, y)
                        int trg_right = (x + 1) * k + y;
                        int w_right = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_right]);
                        weights.push_back(w_right);
                    }

                    // connect to bottom (x, y) -> (x, y + 1)
                    int trg_bot = x * k + y + 1;
                    int w_bot = S() % 10000;
                    edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_bot]);
                    weights.push_back(w_bot);
                }
                else if (y == k / 2)
                { // middle row | only right
                    if (x != k - 1)
                    { // not rightmost limit
                        // connect to right (x, y) -> (x + 1, y)
                        int trg = (x + 1) * k + y;
                        int w = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg]);
                        weights.push_back(w);
                    }
                }
                else
                { // bottom part
                    if (x < k / 2)
                    { // bottom left
                        if (y == (k / 2) + 1 && x == (k / 2) - 1)
                        { // special target
                            int w_spec = -100000;
                            int trg_spec = x * k + (y - 1);
                            
                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_spec]);
                            weights.push_back(w_spec);
                        }

                        // random direction
                        // random left
                        if (x != 0)
                        { // not leftmost
                            int randomval = rand() % 2;
                            if (randomval == 1){
                                int w_l = (S() % (10000 + 100)) - 100;
                                int trg_l = (x - 1) * k + y;

                                edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_l]);
                        weights.push_back(w_l);
                            }
                        }

                        // random bottom
                        if (y != k - 1)
                        { // not bottom most
                            int randomval = rand() % 2;
                            if (randomval == 1){
                                int w_b = (S() % (10000 + 100)) - 100;
                                int trg_b = x * k + y + 1;

                                edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_b]);
                        weights.push_back(w_b);
                            }
                        }

                        // random up
                        int randomval = rand() % 2;
                        if (randomval == 1){
                            int w_u = (S() % (10000 + 100)) - 100;
                            int trg_u = x * k + y - 1;

                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_u]);
                        weights.push_back(w_u);
                        }
                        // random right
                        randomval = rand() % 2;
                        if (randomval == 1){
                            int w_r = (S() % (10000 + 100)) - 100;
                            int trg_r = (x + 1) * k + y;

                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_r]);
                        weights.push_back(w_r);
                        }
                    }
                    else if (x == k / 2)
                    { // center bottom column
                        int trg_bot = x * k + y + 1;
                        int w_bot = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_bot]);
                        weights.push_back(w_bot);

                        if (y == (k / 2) + 1)
                        { // special target
                            int w_spec = -100000;
                            int trg_spec = (x - 1) * k + y;

                            edge e2 = LG.new_edge(node_tracker[cur], node_tracker[trg_spec]);
                        weights.push_back(w_spec);
                        }
                    }
                    else
                    { // bottom right
                        if (x == k - 1)
                        {
                            continue; // last node
                        }

                        if (y != k - 1)
                        { // if not bottom most
                            int trg_bot = x * k + y + 1;
                            int w_bot = S() % 10000;
                            
                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_bot]);
                        weights.push_back(w_bot);
                        }

                        // connect to right (x, y) -> (x + 1, y)
                        int trg_r = (x + 1) * k + y;
                        int w_r = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_r]);
                        weights.push_back(w_r);
                    }
                }
            }
        } // end populate edges

        std::cout << "Making extra necessary leda stuff" << std::endl;
        edge_array<int> l_weight(LG);
        edge _e;
        int t = -1;
        forall_edges(_e, LG) {
            t++;
            l_weight[_e] = weights[t];
        }

        node_array<NodeInfo> nodeInfo(LG);
        node v;
        int i = 0;
        forall_nodes(v, LG)
        {   
            nodeInfo[v].visited = false;
            nodeInfo[v].distance = std::numeric_limits<int>::max();
        }

        node s = node_tracker[0];
        nodeInfo[s].distance = 0; // start node  - needed for boost

        BGraph BG;
        WeightPrMap b_weight = get(&EdgeInfo::weight, BG);
        DistanceMap b_dist = get(&NodeInfo::distance, BG);
        VisitedMap b_visit = get(&NodeInfo::visited, BG);
        LabelMap b_label = get(&NodeInfo::label, BG);
        ParentMap b_parent = get(&NodeInfo::parent, BG);
        vertex b_s;

        std::cout << "Copying to BGL" << std::endl;
        copy_graph(LG, BG, l_weight, b_weight, nodeInfo, b_dist, b_visit, b_label, b_parent);

        std::cout << "Running Benchmarks" << std::endl;

        // run leda benchmark
        std::cout << "LEDA" << std::endl;
        node_array<edge> pred(LG);
	    node_array<int> distance(LG);
        bool r = BELLMAN_FORD_B_T(LG,s,l_weight,distance,pred);
        std::cout << "r = " << r << std::endl;
        // run boost benchmark

        if (r == 1){
        std::cout << "BOOST" << std::endl;
        r = bellman_ford_shortest_paths(BG, k, weight_map(b_weight).
                                distance_map(b_dist).
                                predecessor_map(b_parent));
        std::cout << "r = " << r << std::endl;
        } else {
            std::cout << "not running for boost because leda found negative cycle" << std::endl;
        }
    }
    }
}

int main()
{
    tests(); // <-- This runs 5-node graph for testing
    // return experiments(); // <-- This runs benchmarks

    return 0;
}

void copy_graph(const LGraph &LG, BGraph &BG, edge_array<int> &l_weight, WeightPrMap &b_weight,
                node_array<NodeInfo> &nodeInfo, DistanceMap &b_dist, VisitedMap &b_visit,
                LabelMap &b_label, ParentMap &b_parent)
{
    leda::node_array<vertex> copy_in_BG(LG);
    arc a;
    edge e;
    node v;

    forall_nodes(v, LG)
    {
        copy_in_BG[v] = add_vertex(BG);
        b_dist[copy_in_BG[v]] = nodeInfo[v].distance;
        b_visit[copy_in_BG[v]] = nodeInfo[v].visited;
        b_parent[copy_in_BG[v]] = nodeInfo[v].parent;
        //b_label[copy_in_BG[v]] = nodeInfo[v].label;
    }
    bool isAdded;
    forall_edges(e, LG)
    {
        tie(a, isAdded) = add_edge(copy_in_BG[source(e)], copy_in_BG[target(e)], BG);
        b_weight[a] = l_weight[e];
    }
}
