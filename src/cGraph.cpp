#include <stack>
#include <algorithm>
#include "cGraph.h"

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
