#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include <functional>

/// @brief A vertex is an element of a graph with edges connected to other vertices

class cVertex
{
    int myID;                   // node ID
    static int myLastID;        // ID assigned to most recently constructed node
    std::string myUserName;     // name assigned to vertex by user ( input specification )
    std::vector<std::shared_ptr<cVertex>> vLinks; // vertices that this vertex links to

public:
    cVertex() {} // default constructor
    cVertex(const std::string &name);

    /// @brief Add directed edge from this node to node in next layer
    /// @param dst pointer to node in next layer

    void addEdge(std::shared_ptr<cVertex> dst);

    /// @brief get adjacent nodes
    /// @return
    std::vector<std::shared_ptr<cVertex>> &
    adjacent()
    {
        return vLinks;
    }

    int ID() const
    {
        return myID;
    }

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
    /** @brief populate the graph with edges
       @param sEdges string specifying the edges

        e.g. "a b\nb c" specifies a graph with two edges a -> b -> c
    */

    void setEdges(const std::string &sEdges);

    /// @brief breadth first search
    /// @param start 
    /// @param visitor function called when a new node is reached

    void bfs(
        const std::string & start,
        std::function<void(vertex_t)> visitor );

    /// @brief depth first search
    /// @param start 
    /// @param visitor function called when a new node is reached

    void dfs(
        const std::string & start,
        std::function<void(vertex_t)> visitor );

    /// @brief Shortest path between vertices using dijsktra algorithm
    /// @param start name of start vertex
    /// @param finish name of finish vertex
    /// @return vector of vertices on path

    vVertex_t path(
        const std::string & start,
        const std::string & finish );

    /// @brief Dijsktra path from start to every node
    /// @param[in] start node
    /// @return vertex index vector of vertice that precedes each vertex

    std::vector<int> dijsktra(
        vertex_t start );

    /// @brief Depth first search, detecting cycles
    /// @param start name of start vertex
    void dfs_cycle_detector(const std::string &start);

    int ID(const std::string &name);

    /// @brief Human readable description
    /// @return

    std::string text();

private:
    vVertex_t vVertex;

    vertex_t findorAdd(const std::string &sn);
    int index(const std::string &sn);
    int index(vertex_t v);

    vVertex_t adjacentOut(vertex_t v);
    vVertex_t adjacentIn(vertex_t v);
    vVertex_t adjacentAll(vertex_t v);
};
