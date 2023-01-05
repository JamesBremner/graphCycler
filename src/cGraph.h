/// @brief A vertex
class cVertex
{
    int myID;            // node ID
    static int myLastID; // ID assigned to most recently constructed node
    std::string myUserName;
    std::vector<std::reference_wrapper<cVertex>> vLinks; // vertices that this vertex links to

public:
    cVertex() {} // default constructor
    cVertex(const std::string &name);

    /// @brief Random network generator
    /// @param layerCount
    /// @param minNodePerLayer
    /// @param maxNodePerLayer
    /// @param minEdge
    /// @param maxEdge

    void RandomGenerator(
        int layerCount,
        int minNodePerLayer,
        int maxNodePerLayer,
        int minEdge,
        int maxEdge);

    /// @brief Add edge from this node to node in next layer
    /// @param dst pointer to node in next layer

    void addEdge(cVertex &dst);

    /// @brief get adjacent nodes
    /// @return
    std::vector<std::reference_wrapper<cVertex>> &
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

class cGraph
{

public:
    std::vector<std::reference_wrapper<cVertex>> vVertex;

    void setEdges(const std::string &sEdges);

    cVertex &findorAdd(const std::string &sn);

    void bfs(cVertex &start);

    int ID(const std::string &name);

    /// @brief Human readable description
    /// @return
    std::string text();
};
