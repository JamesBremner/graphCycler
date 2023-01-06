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
    while (iss.good())
    {
        iss >> n1 >> n2;
        findorAdd(n1)->addEdge(findorAdd(n2));
    }
}
void cVertex::addEdge(vertex_t dst)
{
    vLinks.push_back(dst);
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
        for (auto &dst : n->adjacent())
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

void cGraph::bfs(vertex_t start)
{
    // queue of vertices with adjacencies to be explored
    std::queue<int> Q;

    // true for vertex that has been visited
    std::vector<bool> visited(vVertex.size(), false);

    // vertex that visited immedatly before
    std::vector<int> pred(vVertex.size(), -1);

    std::cout << "bfs " << start->userName() << " ";
    Q.push(ID(start->userName()));
    while (Q.size())
    {
        int iv = Q.front();
        Q.pop();
        for (auto &w : vVertex[iv]->adjacent())
        {
            int iw = ID(w->userName());
            if (visited[iw])
            {
                // visited this node before
                // we have found a cycle

                std::vector<int> vCycle;
                for (auto a : vVertex[iv]->adjacent())
                {
                    if (a == w)
                        continue;
                    std::vector<int> pred;
                    dijsktra( a, pred);

                    int ai = findIndex( a->userName() );
                    int pi = pred[ ai ];
                    while( pi != ai ) {
                        vCycle.push_back(pi);
                        pi = pred[pi];
                    }
                }
                std::cout << "\ncycle ";
                for( int pi : vCycle )
                    std::cout << vVertex[pi]->userName() << " ";

                // for( int k = 0; k < vVertex.size(); k++ )
                // {
                //     if( pred[k] == -1 )
                //         continue;
                //     std::cout << vVertex[k]->userName()
                //         <<" "<< vVertex[pred[k]]->userName() << "\n";
                // }

                // std::vector<int> vCycle;
                // vCycle.push_back(iw);
                // vCycle.push_back(iv);
                // int pre = pred[iv];
                // while( pre != -1 ) {
                //     vCycle.push_back( pre );
                //     pre = pred[pre];
                // }
                // std::reverse(vCycle.begin(),vCycle.end());
                // std::cout << "\ncycle ";
                // for( int ic : vCycle )
                //     std::cout << vVertex[ic]->userName() << " ";

                continue;
            }
            std::cout << w->userName() << " ";
            Q.push(iw);
            visited[iw] = true;
            pred[iw] = iv;
        }
    }
}

void cGraph::dijsktra(
    vertex_t start,
    std::vector<int> &pred)
{
    // shortest distance from start to each node
    std::vector<double> dist(vVertex.size(), INT_MAX);

    // previous node on shortest path to each node
    pred.clear();
    pred.resize(vVertex.size(), -1);

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
        for (auto vp : vVertex[uidx]->adjacent())
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
}


vVertex_t cGraph::adjacentOut(vertex_t v)
{
    return v->adjacent();
}
vVertex_t cGraph::adjacentIn(vertex_t v)
{
    vVertex_t ret;
    for (auto t : vVertex)
    {
        for (auto a : t->adjacent())
        {
            if (a == v)
                ret.push_back(a);
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

int cGraph::findIndex(const std::string &sn)
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
int cGraph::findIndex(vertex_t v)
{
    return findIndex(v->userName());
}

void cGraph::dfs_cycle_detector(vertex_t start)
{
    std::vector<bool> visited(vVertex.size(), false);
    vVertex_t pred(vVertex.size(), 0);

    std::stack<vertex_t> wait;
    wait.push(start);

    while (!wait.empty())
    {
        vertex_t v = wait.top();
        wait.pop();
        int vi = findIndex(v);
        if (!visited[vi])
        {
            visited[vi] = true;

            for (vertex_t w : adjacentOut(v))
            {
                if (!visited[findIndex(w)])
                {
                    wait.push(w);
                    pred[findIndex(w)] = v;
                }
                else
                {
                    // previously visited node - a cycle
                    vVertex_t cycle;

                    cycle.push_back(w);
                    cycle.push_back(v);

                    vertex_t pv = pred[findIndex(v)];
                    cycle.push_back(pv);
                    while (pv != w)
                    {
                        // back track one hop
                        pv = pred[findIndex(pv)];
                        cycle.push_back(pv);
                    }

                    std::reverse(cycle.begin(), cycle.end());

                    // display cycle
                    std::cout << "cycle: ";
                    for (vertex_t vc : cycle)
                        std::cout << vc->userName() << " ";
                    std::cout << "\n";
                }
            }
        }
    }
}
