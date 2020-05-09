#define V_PLUS 0
#define V_MINUS 1
#define V_F 2

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
    std::cout << "Weakly Connected Graphs" << std::endl << std::endl;

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
