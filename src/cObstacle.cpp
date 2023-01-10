#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include "cRunWatch.h"
#include "cxy.h"
#include "cObstacle.h"

void read(
    cObstacle &obs,
    const std::string &fname)
{
    obs.clear();
    std::ifstream ifs(fname);
    if (!ifs.is_open())
        throw std::runtime_error(
            "Cannot open input file");
    std::string form, calc;
    ifs >> form >> calc;
    if (form != "format")
        throw std::runtime_error(
            "Bad input format");

    int nx, ny, view;
    if (calc.find("obs") != -1)
    {
        ifs >> nx >> ny >> view;
        obs.grid(nx, ny);
        obs.view(view);
        while (ifs.good())
        {
            // insert obstacle
            int ox, oy;
            ifs >> ox >> oy;
            obs.obstacle(ox, oy);
        }
    }

    else if (calc.find("farm") != -1)
    {
        ifs >> view;
        obs.view(view);
        std::string sid;
        ifs >> sid;
        if (sid[0] == 'p')
            obs.poly();
        else
        {
            ifs >> nx >> ny;
            obs.grid(nx, ny);
            return;
        }
        ifs >> nx >> ny;
        obs.polyAdd(cxy(nx, ny));
        while (ifs.good())
        {
            ifs >> sid >> nx >> ny;
            if (sid[0] == 'p')
                obs.polyAdd(cxy(nx, ny));
        }
    }
    else
        throw std::runtime_error(
            "bad calculation type ");
}

void cObstacle::obstacle(int x, int y)
{
    if (0 > x || x > nx - 1 || 0 > y || y >> ny - 1)
        throw std::runtime_error("Bad location");
    A->cell(x, y)->myType = 1;
}

void cObstacle::clear()
{
    myView = -999;
    myfrect = true;
    vN.clear(); ///< nodes to be included in path
    vL.clear(); ///< links between nodes
    vPath.clear();
    myPolygon.clear();
    mySpanningTree.clear();
}

void cObstacle::grid(int x, int y)
{
    nx = x;
    ny = y;
    A = new cell::cAutomaton<cOCell>(nx, ny);
    A->wrap(false);
    A->ortho(false);
}

void cObstacle::unobstructedPoints()
{
    raven::set::cRunWatch aWatcher("unobstructedPoints");
    int W, H;

    if (myfrect)
        A->size(W, H);
    else
    {
        cxy wh = cxy::enclosingWidthHeight(myPolygon);
        W = wh.x;
        H = wh.y;
        grid(W, H);
    }

    /** The distance covered by the robot
     * The minimum is 2
     * If the robot that can cover 2 is located at 10
     * it can cover all points from 8 to to 12
     * so there is no need for the robot to revisit points
     * closer than 5 units apart
     *
     * If there is an obstacle at 10
     * the robot must visit points at 7 and 13
     *
     * 7->8->9->10<-11<-12<-13
     */
    int V;
    if (myView > 0)
        V = myView;
    else
        V = 2;
    int space = 2 * V + 1;

    for (int h = V; h < H - V + 1; h += space)
        for (int w = V; w <= W - V + 1; w += space)
        {
            if (!myfrect)
            {
                cxy p(w, h);
                if (!p.isInside(myPolygon))
                    continue;
            }
            cOCell *c = A->cell(w, h);
            c->myType = 2;
            vN.push_back(c);
        }
    if (!vN.size())
        throw std::runtime_error(
            "No unobstructed points");
    std::cout << "Node count " << vN.size() << "\n";
    // for( auto p : vN )
    // {
    //     int w,h;
    //     A->coords(w,h,p);
    //     std::cout << p->ID() <<" "<< w <<" "<< h << "\n";
    // }
}

std::string cObstacle::draw(int w, int h) const
{
    switch (A->cell(w, h)->myType)
    {
    case 0:
    default:
        return ".";
    case 1:
        return "X";
    case 2:
         return std::to_string(A->cell(w, h)->ID());
    }
}

void cObstacle::connect()
{}
void cObstacle::tourSpanningTree()
{}