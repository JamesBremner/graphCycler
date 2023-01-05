#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <memory>

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
    dst->vLinks.push_back(vertex_t(this));
}

vertex_t cGraph::findorAdd(const std::string &sn)
{
    for (auto &n : vVertex)
    {
        if (sn == n.get()->userName())
            return n;
    }
    auto v = new cVertex(sn);
    vVertex.push_back(vertex_t(v));
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

                for( int k = 0; k < vVertex.size(); k++ )
                {
                    if( pred[k] == -1 )
                        continue;
                    std::cout << vVertex[k]->userName()
                        <<" "<< vVertex[pred[k]]->userName() << "\n";
                }

                std::vector<int> vCycle;
                vCycle.push_back(iw);
                vCycle.push_back(iv);
                int pre = pred[iv];
                while( pre != -1 ) {
                    vCycle.push_back( pre );
                    pre = pred[pre];
                }
                std::reverse(vCycle.begin(),vCycle.end());
                std::cout << "\ncycle ";
                for( int ic : vCycle )
                    std::cout << vVertex[ic]->userName() << " ";

                continue;
            }
            std::cout << w->userName() << " ";
            Q.push(iw);
            visited[iw] = true;
            pred[iw] = iv;
        }
    }
}

main()
{
    cGraph graph;
    graph.setEdges("a b\nb c\na d\nc e\nd e");
    // graph.setEdges("a b\nb c\na d");

    std::cout << graph.text();

    graph.bfs(graph.vVertex[0]);

    return 0;
}
