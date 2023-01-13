#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

/// @brief A vertex is an element of a graph with edges connected to other vertices

class cVertex
{
    std::string myUserName;     // name assigned to vertex by user ( input specification )
+

public:
    cVertex() {} // default constructor
    cVertex(const std::string &name);

    std::string userName() const
    {
        return myUserName;
    }

};

typedef std::shared_ptr<cVertex> vertex_t;
typedef std::vector<vertex_t> vVertex_t;

class cGraph
{
public:

//////////  SETTERS ////////////////////////////////

    void clear();

    /** @brief populate the graph with edges
       @param sEdges string specifying the edges

        e.g. "a b\nb c" specifies a graph with two edges a -> b -> c
    */

    /// @brief find vertex by name, add to graph if not found
    /// @param sn vertex name
    /// @return shared pointer to vertex found or added

    vertex_t findorAdd(const std::string &sn);

    void setEdges(const std::string &sEdges);

    void addEdge(const std::string &src, const std::string &dst);
    int addEdge(vertex_t src, vertex_t dst,
        const std::string& sattr = "" );

    /// @brief populate graph with edges that have attributes
    /// @param sEdges string specifying the edges
    /// @param countAttributes number of edge attributes for each edge

    void setEdges(const std::string &sEdges, int countAttributes);
    
//////////  GRAPH THEORY ALGORITHMS ////////////////////////////////

    /// @brief breadth first search
    /// @param start
    /// @param visitor function called when a new node is reached

    void bfs(
        const std::string &start,
        std::function<void(vertex_t)> visitor);

    /// @brief depth first search
    /// @param start
    /// @param visitor function called when a new node is reached

    void dfs(
        const std::string &start,
        std::function<void(vertex_t)> visitor);

    /// @brief Shortest path between vertices using dijsktra algorithm
    /// @param start name of start vertex
    /// @param finish name of finish vertex
    /// @return vector of vertices on path

    vVertex_t path(
        const std::string &start,
        const std::string &finish);

    /// @brief Shortest path between vertices using dijsktra algorithm
    /// @param start start vertex
    /// @param finish finish vertex
    /// @return vector of vertices on path

    vVertex_t path(
        vertex_t start,
        vertex_t finish);

    /// @brief Dijsktra path from start to every node
    /// @param[in] start node
    /// @return vertex index vector of vertice that precedes each vertex

    std::vector<int> dijsktra(
        vertex_t start);

    /// @brief Create a spanning tree
    /// @param start starting vertex name
    /// @return the spanning tree ( cGraph )

    cGraph spanningTree(const std::string &start);

    /// @brief Depth first search, finding cycles in directed graph
    /// @param start name of start vertex
    /// @return vector of vectors, each with the vertices in a cycle

    std::vector<std::vector<vertex_t>>
    dfs_cycle_finder(
        const std::string &start);

    int componentCount();

    //////////  GETTERS ////////////////////////////////

    int vertexCount() const
    {
        return vVertex.size();
    }

    std::vector<int> outEdges( vertex_t v)
    {
        return vOutEdges[index(v)];
    }


    std::vector< std::pair<std::string,std::string>>
    getlinkedVerticesNames();

    double edgeAttrDouble(
        vertex_t src,
        vertex_t dst,
        int attrIndex) ;
    double edgeAttrDouble(
        const std::string & src,
        const std::string & dst,
        int attrIndex) ;

    /// @brief vertices that are reachable in one hop
    /// @param v start vertex
    /// @return vector of vertices
    vVertex_t adjacentOut(vertex_t v);

    vVertex_t adjacentIn(vertex_t v);
    vVertex_t adjacentAll(vertex_t v);

    /// @brief get leaf vertices
    /// @return vertices that have exactly one edge, in or out
    vVertex_t leaves();

    /// @brief Human readable description
    /// @return string

    std::string text();

    vVertex_t::iterator begin()
    {
        return vVertex.begin();
    }
    vVertex_t::iterator end()
    {
        return vVertex.end();
    }

private:
    vVertex_t vVertex;                               // graph vertices
    std::vector<std::string> vVertexName;            // vertex user names
    std::vector<std::vector<int>> vOutEdges;         // edge indices that start at each vertex
    std::vector<int> vEdgeDst;                       // vertex indices of edge destinations
    std::vector<std::vector<std::string>> vEdgeAttr; // edge attributes


    int index(const std::string &sn) const;
    int index(vertex_t v) const;

    int edgeIndex(
    vertex_t src,
    vertex_t dst);

};
