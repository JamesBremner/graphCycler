#include <stack>
#include <algorithm>
#include <sstream>
#include <queue>

#include "cGraph.h"

int cVertex::myLastID = -1;

cVertex::cVertex(const std::string &name)
    : myUserName(name)
{
    myID = ++myLastID;
}

void cGraph::setEdges(const std::string &sEdges)
{
    std::istringstream iss(sEdges);
    std::string n1, n2;
    vVertex.clear();
    while (iss.good())
    {
        iss >> n1 >> n2;
        addEdge(findorAdd(n1), findorAdd(n2));
    }
}
void cGraph::setEdges(
    const std::string &sEdges,
    int countAttributes)
{
    std::istringstream iss(sEdges);
    std::string n1, n2, sattr;

    vVertex.clear();
    vEdgeAttr.clear();
    vEdgeDst.clear();

    while (iss.good())
    {
        iss >> n1 >> n2;
        sattr = "";
        std::vector<std::string> vattr;
        std::string sattr_one;
        for( int k = 0; k < countAttributes; k++ )
        {
            iss >> sattr_one;
            vattr.push_back(sattr_one);
        }
        vEdgeAttr.push_back( vattr );
        addEdge(findorAdd(n1), findorAdd(n2));
    }
}
void cGraph::addEdge(vertex_t src, vertex_t dst)
{
    vEdgeDst.push_back(index(dst));
    src->addEdge(vEdgeDst.size() - 1);
}

vertex_t cGraph::findorAdd(const std::string &sn)
{
    for (auto &n : vVertex)
    {
        if (sn == n.get()->userName())
            return n;
    }
    // auto v = new cVertex(sn);
    vVertex.push_back(vertex_t(new cVertex(sn)));
    return vVertex.back();
}

std::string cGraph::text()
{
    std::stringstream ss;

    for (auto &n : vVertex)
    {
        ss << "node " << n->userName() << " linked to ";
        for (auto &dst : adjacentOut(n))
        {
            ss << dst->userName() << " ";
        }
        ss << "\n";
    }
    return ss.str();
}

int cGraph::ID(const std::string &name)
{
    for (auto &v : vVertex)
        if (v->userName() == name)
            return v->ID();
    return -1;
}

void cGraph::bfs(
    const std::string &start,
    std::function<void(vertex_t)> visitor)
{
    // queue of vertices with adjacencies to be explored
    std::queue<int> Q;

    // true for vertex that has been visited
    std::vector<bool> visited(vVertex.size(), false);

    visitor(findorAdd(start));
    int si = index(start);
    visited[si] = true;
    Q.push(si);

    while (Q.size())
    {
        int iv = Q.front();
        Q.pop();

        for (auto &w : adjacentOut(vVertex[iv]))
        {
            int iw = index(w->userName());
            if (!visited[iw])
            {
                visitor(w);
                visited[iw] = true;
                Q.push(iw);
            }
        }
    }
}

void cGraph::dfs(
    const std::string &start,
    std::function<void(vertex_t)> visitor)
{
    // track visited vertices
    std::vector<bool> visited(vVertex.size(), false);

    // vertices waiting to be visited
    std::stack<vertex_t> wait;

    /*  1 Start by putting one of the graph's vertices on top of a stack.
        2 Take the top vertex of the stack and add it to the visited list.
        3 Add sdjscent vertices which aren't in the visited list to the top of the stack.
        4 Keep repeating steps 2 and 3 until the stack is empty.
    */

    wait.push(vVertex[index(start)]);
    while (!wait.empty())
    {
        vertex_t v = wait.top();
        wait.pop();
        int vi = index(v);
        visitor(v);
        visited[vi] = true;

        for (vertex_t w : adjacentOut(v))
            if (!visited[index(w)])
                wait.push(w);
    }
}

vVertex_t cGraph::path(
    const std::string &start,
    const std::string &finish)
{
    return path(
        vVertex[index(start)],
        vVertex[index(finish)]);
}
vVertex_t cGraph::path(
    vertex_t start,
    vertex_t finish)
{
    vVertex_t ret;

    int si = index(start->userName());

    auto pred = dijsktra(
        vVertex[si]);

    int vi = index(finish->userName());
    while (vi != si)
    {
        if (vi == -1)
        {
            ret.clear();
            return ret;
        }
        ret.push_back(vVertex[vi]);
        vi = pred[vi];
    }
    ret.push_back(vVertex[si]);

    std::reverse(ret.begin(), ret.end());

    return ret;
}

std::vector<int> cGraph::dijsktra(
    vertex_t start)
{
    // shortest distance from start to each node
    std::vector<double> dist(vVertex.size(), INT_MAX);

    // previous node on shortest path to each node
    std::vector<int> pred(vVertex.size(), -1);

    std::vector<bool> sptSet(vVertex.size(), false); // sptSet[i] will be true if vertex i is included in shortest
                                                     // path tree or shortest distance from src to i is finalized

    int startIdx = std::distance(
        vVertex.begin(),
        std::find(vVertex.begin(), vVertex.end(),
                  start));

    // Distance of source vertex from itself is always 0
    dist[startIdx] = 0;
    pred[startIdx] = 0;

    // Find shortest path for all vertices
    for (int count = 0; count < vVertex.size() - 1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not
        // yet processed. u is always equal to src in the first iteration.
        int min = INT_MAX, uidx;
        for (int vidx = 0; vidx < vVertex.size(); vidx++)
            if (sptSet[vidx] == false && dist[vidx] <= min)
            {
                min = dist[vidx];
                uidx = vidx;
            }
        if (min == INT_MAX)
        {
            // no more nodes connected to start
            break;
        }

        // Mark the picked vertex as processed
        sptSet[uidx] = true;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (auto vp : adjacentOut(vVertex[uidx]))
        {
            int vidx = std::distance(
                vVertex.begin(),
                std::find(vVertex.begin(), vVertex.end(),
                          vp));
            if (sptSet[vidx])
                continue; // already processed

            // Update dist[v] only if total weight of path from src to  v through u is
            // smaller than current value of dist[v]
            double cost = 1;
            if (dist[uidx] + cost < dist[vidx])
            {
                dist[vidx] = dist[uidx] + cost;
                pred[vidx] = uidx;
            }
        }
    }
    return pred;
}

vVertex_t cGraph::adjacentOut(vertex_t v)
{
    vVertex_t ret;
    for (int i : v->outEdges())
    {
        ret.push_back(vVertex[vEdgeDst[i]]);
    }
    return ret;
}
vVertex_t cGraph::adjacentIn(vertex_t v)
{
    vVertex_t ret;
    int vi = index(v);
    for (auto t : vVertex)
    {
        for (auto ei : t->outEdges())
        {
            if (vEdgeDst[ei] == vi)
                ret.push_back(t);
        }
    }
    return ret;
}
vVertex_t cGraph::adjacentAll(vertex_t v)
{
    vVertex_t ret(adjacentOut(v));
    vVertex_t vin = adjacentIn(v);
    ret.insert(ret.end(), vin.begin(), vin.end());
    return ret;
}

int cGraph::index(const std::string &sn)
{
    int ret = 0;
    for (auto &n : vVertex)
    {
        if (sn == n.get()->userName())
            return ret;
        ret++;
    }
    return -1;
}
int cGraph::index(vertex_t v)
{
    return index(v->userName());
}

std::vector<std::vector<vertex_t>>
cGraph::dfs_cycle_finder(const std::string &start)
{
    std::vector<std::vector<vertex_t>> ret;

    // track visited vertices
    std::vector<bool> visited(vVertex.size(), false);

    // vertices waiting to be processed
    std::stack<vertex_t> wait;

    // start at the beginning
    wait.push(vVertex[index(start)]);

    // continue until no more vertices need processing
    while (!wait.empty())
    {
        vertex_t v = wait.top();
        wait.pop();
        int vi = index(v);
        if (!visited[vi])
        {
            visited[vi] = true;

            for (vertex_t w : adjacentOut(v))
            {
                if (!visited[index(w)])
                {
                    wait.push(w);
                }
                else
                {
                    // previously visited node, check for ancestor
                    auto cycle = path(w, v);
                    if (cycle.size() > 0)
                    {
                        // found a cycle
                        cycle.push_back(w);
                        ret.push_back(cycle);
                    }
                }
            }
        }
    }
    return ret;
}
