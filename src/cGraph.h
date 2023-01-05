


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


    /// @brief Add edge from this node to node in next layer
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

class cGraph
{

public:
    std::vector<vertex_t> vVertex;

    void setEdges(const std::string &sEdges);

    vertex_t findorAdd(const std::string &sn);

    void bfs(vertex_t start);

    int ID(const std::string &name);

    /// @brief Human readable description
    /// @return
    std::string text();
};
