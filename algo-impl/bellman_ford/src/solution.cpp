#include <math.h>
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

srand(time(0));

typedef leda::node node;
typedef leda::edge edge;
struct NodeInfo
{
    unsigned int distance;
    bool visited;
    // TODO
    // predecessor
    // label
};
struct EdgeInfo
{
    int weight;
};

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
typedef boost::property_map<BGraph, unsigned int NodeInfo::*>::type DistanceMap;
typedef boost::property_map<BGraph, bool NodeInfo::*>::type VisitedMap;

void copy_graph(const LGraph &LG, BGraph &BG, edge_array<int> &l_weight, WeightPrMap &b_weight,
                node_array<NodeInfo> &nodeInfo, DistanceMap &b_dist, VisitedMap &b_visit);

void bf_impl(){
    
}

void benchmark()
{

}

void tests()
{
    leda::random_source S;
    std::cout << "Weakly Connected Graphs" << std::endl << std::endl;

    /* random, at least weakly connected graphs */
    int n[] = {1000, 4000, 8000};
    for (int i = 0; i < 3; i++)
    {
        std::cout << "Weakly Connected (" << n << "," << m << ")" << std::endl;
        std::cout << "Constructing Graph" << std::endl;
        int m = 20 * n[i] * log(n[i]);
        LGraph LG;
        edge_array<int> l_weight(LG);

        BGraph BG;
        WeightPrMap b_weight = get(&EdgeInfo::weight, BG);
        DistanceMap b_dist = get(&NodeInfo::distance, BG);
        VisitedMap b_visit = get(&NodeInfo::visited, BG);

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

        nodeInfo[0] = 0; // start node

        std::cout << "Copying to BGL" << std::endl;
        copy_graph(LG, BG, l_weight, b_weight, nodeInfo, b_dist, b_visit);

        std::cout << "Running Benchmarks" << std::endl;

        // run leda benchmark
        // run boost benchmark
        // run our benchmark
    }

    /* grid graphs */
    std::cout << "Grid Graphs" << std::endl << std::endl;
    int grid_n = {100, 200, 300};
    for (int l = 0; l < 3; l++)
    {
        int k = grid_n[l]; // grid size
        std::cout << "Grid (" << k << "," << k << ")" << std::endl;
        std::cout << "Constructing Graph" << std::endl;

        /* START BOILERPLATE */

        /* END BOILERPLATE */

        int num_nodes = k * k;
        
        leda::node node_tracker[num_nodes];

        // populate vertices
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
                        l_weight[e] = w_right;
                    }

                    // connect to bottom (x, y) -> (x, y + 1)
                    int trg_bot = x * k + y + 1;
                    int w_bot = S() % 10000;
                    edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_bot]);
                    l_weight[e] = w_bot;
                }
                else if (y == k / 2)
                { // middle row | only right
                    if (x != k - 1)
                    { // not rightmost limit
                        // connect to right (x, y) -> (x + 1, y)
                        int trg = (x + 1) * k + y;
                        int w = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg]);
                        l_weight[e] = w;
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
                            l_weight[e] = w_spec;
                        }

                        // random direction
                        // random left
                        if (x != 0)
                        { // not leftmost
                            int randomval = rand() % 2;
                            if (randomval == 1){
                                int w_l = (S() % (10000 + 100)) - 100 int trg_l = (x - 1) * k + y;
                                int trg_l = (x - 1) * k + y;

                                edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_l]);
                                l_weight[e] = w_l;
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
                                l_weight[e] = w_b;
                            }
                        }

                        // random up
                        int randomval = rand() % 2;
                        if (randomval == 1){
                            int w_u = (S() % (10000 + 100)) - 100 int trg_u = x * k + y - 1;
                            int trg_u = x * k + y - 1;

                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_u]);
                            l_weight[e] = w_u;
                        }
                        // random right
                        int randomval = rand() % 2;
                        if (randomval == 1){
                            int w_r = (S() % (10000 + 100)) - 100 int trg_r = (x + 1) * k + y;
                            int trg_r = (x + 1) * k + y;

                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_r]);
                            l_weight[e] = w_r;
                        }
                    }
                    else if (x == k / 2)
                    { // center bottom column
                        int trg_bot = x * k + y + 1;
                        int w_bot = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_bot]);
                        l_weight[e] = w_bot;

                        if (y == (k / 2) + 1)
                        { // special target
                            int w_spec = -100000;
                            int trg_spec = (x - 1) * k + y;

                            edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_spec]);
                            l_weight[e] = w_spec;
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
                            l_weight[e] = w_bot;
                        }

                        // connect to right (x, y) -> (x + 1, y)
                        int trg_r = (x + 1) * k + y;
                        int w_r = S() % 10000;

                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_r]);
                        l_weight[e] = w_r;
                    }
                }
            }
        } // end populate edges

        int source_idx = 0;
        nodeInfo[source_idx] = 0; // start node

        std::cout << "Copying to BGL" << std::endl;
        copy_graph(LG, BG, l_weight, b_weight, nodeInfo, b_dist, b_visit);

        std::cout << "Running Benchmarks" << std::endl;
        benchmark();
    }
}

int main()
{
    return tests(); // <-- This runs 5-node graph for testing
    // return experiments(); // <-- This runs benchmarks
}

void copy_graph(const LGraph &LG, BGraph &BG, edge_array<int> &l_weight, WeightPrMap &b_weight,
                node_array<NodeInfo> &nodeInfo, DistanceMap &b_dist, VisitedMap &b_visit)
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
    }
    bool isAdded;
    forall_edges(e, LG)
    {
        tie(a, isAdded) = add_edge(copy_in_BG[source(e)], copy_in_BG[target(e)], BG);
        b_weight[a] = l_weight[e];
    }
}
