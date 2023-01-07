#include <iostream>
#include "cutest.h"
#include "cGraph.h"

TEST( breadth_first_search)
{
    // construct test graph
    cGraph graph;
    graph.setEdges("a b\nb c\nc d\nd a\na x\nx y");

    std::vector< std::string > expected { "a", "b", "x", "c", "y", "d" };
    std::vector< std::string > visited;

    graph.bfs(
        "a",
        [&]( vertex_t v ) {
            visited.push_back( v->userName() );
        });

    for( int k = 0; k < expected.size(); k++ )
        CHECK_EQUAL( visited[k], expected[k] );

}
TEST( depth_first_search)
{
    // construct test graph
    cGraph graph;
    graph.setEdges("a b\nb c\nc d\nd a\na x\nx y");

    std::vector< std::string > expected { "a", "x", "y", "b", "c", "d"  };
    std::vector< std::string > visited;

    graph.dfs(
        "a",
        [&]( vertex_t v ) {
                        std::cout << v->userName() << " ";
            visited.push_back( v->userName() );
        });

    for( int k = 0; k < expected.size(); k++ )
        CHECK_EQUAL( visited[k], expected[k] );

}

main()
{
    return raven::set::UnitTest::RunAllTests();
}