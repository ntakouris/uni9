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

typedef boost::mt19937 RNGType;
RNGType rng;

struct EdgeProperties {
  int weight;
};

int experiments(){
    typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS, 
    boost::no_property,
    EdgeProperties> boost_Graph;

    boost_Graph g;

    int min = -100;
    int max = 10000;
    boost::uniform_int<> unif_int(min, max);
    boost::variate_generator< RNGType, boost::uniform_int<> > rndgen(rng, unif_int);

    int n = 200;
    boost::generate_random_graph(g, n, 20 * n * log(n), rndgen);

    std::vector<int> distance(n, (std::numeric_limits < int >::max)());
    std::vector<std::size_t> parent(n);
    for (int i = 0; i < n; ++i){
        parent[i] = i;
        distance[i] = 0; // distance[z]
    }

    boost::property_map<boost_Graph, int EdgeProperties::*>::type weight_pmap = boost::get(&EdgeProperties::weight, g);

    bool r = bellman_ford_shortest_paths(g, n, 
        boost::weight_map(weight_pmap).distance_map(&distance[0]).
        predecessor_map(&parent[0]));

    std::cout << "bellman_ford_shortest_paths: " << r << std::endl;

    return 0;
}

int main(){
    // return tests(); // <-- This runs 5-node graph for testing 
    return experiments(); // <-- This runs benchmarks
}