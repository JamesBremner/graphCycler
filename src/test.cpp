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
            visited.push_back( v->userName() );
        });

    for( int k = 0; k < expected.size(); k++ )
        CHECK_EQUAL( visited[k], expected[k] );

}
TEST( dijsktra )
{
    // construct test graph
    cGraph graph;
    graph.setEdges("a b\nb c\nc d\nd a\na x\nx y");

    std::vector< std::string > expected1 { "a", "x", "y"  };
    std::vector< std::string > visited;

    auto p1 = graph.path("a", "y" );

    for( int k = 0; k < expected1.size(); k++ )
        CHECK_EQUAL( p1[k]->userName(), expected1[k] );

    std::vector< std::string > expected2 { "a", "b", "c", "d"  };
    auto p2 = graph.path("a", "d" );
    for( int k = 0; k < expected1.size(); k++ )
        CHECK_EQUAL( p2[k]->userName(), expected2[k] );
}

main()
{
    return raven::set::UnitTest::RunAllTests();
}