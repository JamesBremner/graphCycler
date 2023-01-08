
#include "cGraph.h"

main()
{
    // construct test graph
    cGraph graph;
    graph.setEdges("a b\nb c\nc d\nd a\na x\nx y");

    // display graph edges
    std::cout << graph.text();

    // run the cycle detector
    graph.dfs_cycle_finder("a");

    return 0;

}
