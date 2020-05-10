#define PRINT_LABELS 0

#define SKIP_RANDOM_GRAPHS 0
#define SKIP_GRID_GRAPHS 1

#include <cmath>
#include <utility>
#include <random>
#include <queue>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/random.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/graph/bellman_ford_shortest_paths.hpp>
#include <boost/graph/leda_graph.hpp>
#include <boost/graph/depth_first_search.hpp>

#include "LEDA/core/stack.h"
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

enum
{
    NEG_CYCLE = -2,
    ATT_TO_CYCLE = -1,
    FINITE = 0,
    PLUS = 1,
    CYCLE = 2,
    ON_CUR_PATH = 3,
    UNKNOWN = 4
};

typedef struct NodeInfo
{
    int distance; // was unsigned int
    bool visited;
    int pred;
    int label;
    bool in_q;
    bool in_r;
    bool reached_from_node_in_U;
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
typedef boost::property_map<BGraph, int NodeInfo::*>::type PredMap;
typedef boost::property_map<BGraph, bool NodeInfo::*>::type InQMap;
typedef boost::property_map<BGraph, bool NodeInfo::*>::type InRMap;
typedef boost::property_map<BGraph, bool NodeInfo::*>::type ReachedFromNodeInUMap;

typedef boost::vertex_index_t vertex_index;
typedef boost::property_map<BGraph, boost::vertex_index_t>::type IndexMap;

typedef boost::graph_traits<BGraph>::vertex_iterator vertex_iter;

void copy_graph(const LGraph &LG, BGraph &BG, edge_array<int> &l_weight, WeightPrMap &b_weight,
                node_array<NodeInfo> &nodeInfo, DistanceMap &b_dist, VisitedMap &b_visit,
                LabelMap &b_label, PredMap &b_parent);

inline void Update_pred(const BGraph &BG, vertex v,
                        InRMap &in_R,
                        ReachedFromNodeInUMap &reached_from_node_in_U,
                        PredMap &pred, IndexMap &index)
{
    reached_from_node_in_U[v] = true;
    // for all out edges *ei of v

    outArcIt ei, ei_end;
    for (boost::tie(ei, ei_end) = out_edges(v, BG); ei != ei_end; ++ei)
    {
        vertex w = boost::target(*ei, BG);
        if (!reached_from_node_in_U[w])
        {
            if (in_R[w])
            {
                pred[w] = index[v];
            }
            Update_pred(BG, w, in_R, reached_from_node_in_U, pred, index);
        }
    }
}

class DFSVisitor;
class DFSVisitor : public boost::default_dfs_visitor
{
public:
    InRMap rmap;
    DFSVisitor(InRMap &_rmap) : rmap{_rmap}
    {
    }
    void discover_vertex(vertex v, const BGraph &g) const
    {
        this->rmap[v] = true;
    }
};

bool our_bellman(BGraph &BG, int n, WeightPrMap &c, DistanceMap &distmap, PredMap &predmap)
{
    int phase_count = 0;
    std::queue<vertex> Q;

    vertex u, v;

    IndexMap index = get(boost::vertex_index, BG);
    LabelMap labelmap = get(&NodeInfo::label, BG);
    InQMap qmap = get(&NodeInfo::in_q, BG);

    std::pair<vertex_iter, vertex_iter> vp;
    vertex s;
    for (vp = vertices(BG); vp.first != vp.second; ++vp.first)
    {
        vertex v = *vp.first;

        predmap[v] = NULL;
        labelmap[v] = UNKNOWN; /* this is for tagging */
        qmap[v] = false;

        if (distmap[v] == 0)
        {
            s = v;
        }
    }

    distmap[s] = 0;
    qmap[s] = true;
    Q.push(s);
    Q.push(NULL);

    while (phase_count < n)
    {
        u = Q.front();
        Q.pop();
        if (u == NULL)
        {
            phase_count++;
            if (Q.empty())
            {
                return true;
            }
            Q.push(NULL);
            continue;
        }
        else
        {
            qmap[u] = false;
        }
        int du = distmap[u];
        // forall_adj_edges e (of u)
        outArcIt ei, ei_end;
        for (boost::tie(ei, ei_end) = out_edges(u, BG); ei != ei_end; ++ei)
        {
            // e = *ei
            vertex source = boost::source(*ei, BG);
            vertex target = boost::target(*ei, BG);

            //returns target(e) if u = source(e) and source(e) oth- erwise.
            //v = G.opposite(u, e);
            v = source;
            if (u == source)
            {
                v = target;
            }

            int d = du + c[*ei];
            if ((predmap[v] == NULL && v != s) || d < distmap[v])
            {
                distmap[v] = d;
                predmap[v] = u;

                if (!qmap[v])
                {
                    Q.push(v);
                    qmap[v] = true;
                }
            }
        }
    }

    // BF: postprocessing
    if (predmap[s] != NULL)
    {
        return false;
    }

    InRMap rmap = get(&NodeInfo::in_r, BG);
    // for all edges of BG
    arcIt ei, ei_end;
    for (boost::tie(ei, ei_end) = edges(BG); ei != ei_end; ei++)
    {
        vertex target = boost::target(*ei, BG);
        vertex source = boost::source(*ei, BG);

        if (index[source] != index[predmap[target]])
        {
            // hide *ei
        }
    }

    DFSVisitor vis(rmap);
    boost::depth_first_search(BG, boost::root_vertex(s).visitor(vis));
    // ((graph *)&G)->restore_all_edges();

    ReachedFromNodeInUMap reached_from_node = get(&NodeInfo::reached_from_node_in_U, BG);

    // for all nodes v of G
    for (vp = vertices(BG); vp.first != vp.second; ++vp.first)
    {
        vertex v = *vp.first;

        if (qmap[v] && !reached_from_node[v])
        {
            Update_pred(BG, v, rmap, reached_from_node, predmap, index);
        }
    }

    // labels = unknown from first loop of this function
    // reachable = reached_from_node
    ReachedFromNodeInUMap reachable = reached_from_node;
    LabelMap label = labelmap;
    PredMap pred = predmap;
    // condition (1)

    // forall_nodes v of G
    for (vp = vertices(BG); vp.first != vp.second; ++vp.first)
    {
        if (v != s)
        {
            assert((pred[v] == NULL) == (reachable[v] == false));
            if (reachable[v] == false)
            {
                labelmap[v] = PLUS;
            }
        }
    }

    // classification of nodes
    if (pred[s] == NULL)
    {
        label[s] = FINITE;
    }

    // for all nodes again follow the path and label
    for (vp = vertices(BG); vp.first != vp.second; ++vp.first)
    {
        vertex v = *vp.first;
        if (label[v] == UNKNOWN)
        {
            leda::stack<vertex> S;
            vertex w = v;
            while (label[w] == UNKNOWN)
            {
                label[w] = ON_CUR_PATH;
                S.push(w);
                w = pred[w];
            }

            // label all nodes on current path
            int t = label[w];
            if (t == ON_CUR_PATH)
            {
                vertex z;
                do
                {
                    z = S.pop();
                    label[z] = CYCLE;
                } while (z != w);
                while (!S.empty())
                {
                    label[S.pop()] = ATT_TO_CYCLE;
                }
            }
            else // t is CYCLE, ATT_TO_CYCLE, or FINITE
            {
                if (t == CYCLE)
                {
                    t = ATT_TO_CYCLE;
                }
                while (!S.empty())
                {
                    label[S.pop()] = t;
                }
            }

        }
    }
    // condition (2)
    for (vp = vertices(BG); vp.first != vp.second; ++vp.first)
    {
        vertex v = *vp.first;

        if (label[v] == CYCLE)
        {
            vertex w = v;
            int cycle_length = 0;
            do
            {
                outArcIt ei, ei_end;
                for (boost::tie(ei, ei_end) = out_edges(w, BG); ei != ei_end; ++ei)
                {
                    // e = *ei
                    vertex source = boost::source(*ei, BG);
                    vertex target = boost::target(*ei, BG);

                    if (source == pred[w] && target == w){
                        cycle_length += c[*ei];
                        label[w] = NEG_CYCLE;
                        w = pred[w];

                        break;
                    }
                }
            } while (w != v);
            assert(cycle_length < 0);
        }
    }

    //conditions (3), (4), and (5)
    if (label[s] == FINITE){
        assert(distmap[s] == 0);
    }

    for (boost::tie(ei, ei_end) = edges(BG); ei != ei_end; ei++)
    {
        vertex v = boost::target(*ei, BG);
        vertex w = boost::source(*ei, BG); 

        if (label[w] == FINITE)
        {
            assert(label[v] == FINITE || label[v] == PLUS);
            if (label[v] == FINITE)
            {
                assert(distmap[v] + c[*ei] >= distmap[w]);
                if (w == pred[w]){ // if is self
                    assert(distmap[v] + c[*ei] == distmap[w]);
                }
            }
        }
    }

    if (PRINT_LABELS == 1) {

    }


    return false;
}

    void tests()
    {
        leda::random_source S;
        if (SKIP_RANDOM_GRAPHS == 0)
        {
            std::cout << "Random Weakly Connected Graphs" << std::endl
                      << std::endl;

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
                PredMap b_parent = get(&NodeInfo::pred, BG);
                vertex b_s;

                copy_graph(LG, BG, l_weight, b_weight, nodeInfo, b_dist, b_visit, b_label, b_parent);

                std::cout << "Running Benchmarks" << std::endl;

                // run leda benchmark
                std::cout << "OURS" << std::endl;
                bool r = our_bellman(BG, n[i], b_weight, b_dist, b_parent);
                std::cout << "r = " << r << std::endl;

                std::cout << "LEDA" << std::endl;
                node_array<edge> pred(LG);
                node_array<int> distance(LG);
                r = BELLMAN_FORD_B_T(LG, s, l_weight, distance, pred);
                std::cout << "r = " << r << std::endl;
                // run boost benchmark

                if (r == 1)
                {
                    std::cout << "BOOST" << std::endl;
                    r = bellman_ford_shortest_paths(BG, n[i], weight_map(b_weight).distance_map(b_dist).predecessor_map(b_parent));
                    std::cout << "r = " << r << std::endl;
                }
                else
                {
                    std::cout << "not running for boost because leda found negative cycle" << std::endl;
                }
                // run our benchmark
            }
        }

        /* grid graphs */
        if (SKIP_GRID_GRAPHS == 0)
        {
            std::cout << "Grid Graphs" << std::endl
                      << std::endl;
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
                                    if (randomval == 1)
                                    {
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
                                    if (randomval == 1)
                                    {
                                        int w_b = (S() % (10000 + 100)) - 100;
                                        int trg_b = x * k + y + 1;

                                        edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_b]);
                                        weights.push_back(w_b);
                                    }
                                }

                                // random up
                                int randomval = rand() % 2;
                                if (randomval == 1)
                                {
                                    int w_u = (S() % (10000 + 100)) - 100;
                                    int trg_u = x * k + y - 1;

                                    edge e = LG.new_edge(node_tracker[cur], node_tracker[trg_u]);
                                    weights.push_back(w_u);
                                }
                                // random right
                                randomval = rand() % 2;
                                if (randomval == 1)
                                {
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
                forall_edges(_e, LG)
                {
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
                PredMap b_parent = get(&NodeInfo::pred, BG);
                vertex b_s;

                std::cout << "Copying to BGL" << std::endl;
                copy_graph(LG, BG, l_weight, b_weight, nodeInfo, b_dist, b_visit, b_label, b_parent);

                std::cout << "Running Benchmarks" << std::endl;

                // run leda benchmark
                std::cout << "LEDA" << std::endl;
                node_array<edge> pred(LG);
                node_array<int> distance(LG);
                bool r = BELLMAN_FORD_B_T(LG, s, l_weight, distance, pred);
                std::cout << "r = " << r << std::endl;
                // run boost benchmark

                if (r == 1)
                {
                    std::cout << "BOOST" << std::endl;
                    r = bellman_ford_shortest_paths(BG, k, weight_map(b_weight).distance_map(b_dist).predecessor_map(b_parent));
                    std::cout << "r = " << r << std::endl;
                }
                else
                {
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
                    LabelMap &b_label, PredMap &b_parent)
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
            b_parent[copy_in_BG[v]] = nodeInfo[v].pred;
            //b_label[copy_in_BG[v]] = nodeInfo[v].label;
        }
        bool isAdded;
        forall_edges(e, LG)
        {
            tie(a, isAdded) = add_edge(copy_in_BG[source(e)], copy_in_BG[target(e)], BG);
            b_weight[a] = l_weight[e];
        }
    }
