#include <vector>
#include <string>
#include <memory>
#include <iostream>

/// @brief A vertex
class cVertex
{
    int myID;            // node ID
    static int myLastID; // ID assigned to most recently constructed node
    std::string myUserName;
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
    vVertex_t vVertex;

    void setEdges(const std::string &sEdges);

    vertex_t findorAdd(const std::string &sn);
    int findIndex(const std::string &sn);
    int findIndex(vertex_t v);

    void bfs(vertex_t start);

    /// @brief Depth first search, detecting cycles
    /// @param start 
    void dfs_cycle_detector(vertex_t start);

    /// @brief Dijsktra path finder
    /// @param[in] start node
    /// @param[out] pred node vector of nodes that precedes indexed node
    void dijsktra(
        vertex_t start,
        std::vector<int> &pred);

    vVertex_t adjacentOut( vertex_t v );
    vVertex_t adjacentIn( vertex_t v );
    vVertex_t adjacentAll( vertex_t v );

    int ID(const std::string &name);

    /// @brief Human readable description
    /// @return
    std::string text();
};
