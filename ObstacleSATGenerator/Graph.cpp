/*
 * Graph.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: hartman
 */

#include "Graph.hpp"

#include <fstream>
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;
#include <iomanip>

Graph::Graph(const std::string & fname)
{
    std::ifstream ifile(fname);
    if(!ifile)
        throw std::runtime_error ("readGraph() couldn't open file " + fname);
    
    size_t numVerts;
    ifile >> numVerts;
    for(size_t ii=0;ii<numVerts;++ii)
    {
        _adjacencies.push_back(vector<int>());
        for(size_t jj=0;jj<numVerts;++jj)
        {
            int a;
            if(!(ifile >> a))
                throw std::runtime_error ("readGraph() error reading file " + (fname+".txt"));
            _adjacencies.back().push_back(a);
        }
    }
}

bool Graph::edge(Vertex i, Vertex j) const {
    return _adjacencies[i][j];
}

bool Graph::notAdjacentToAnyButLast(const Path & p, Vertex vert) const
{
    for(size_t ii=0;ii<p.size()-1;++ii)
        if(_adjacencies[p[ii]][vert])
            return false;
    return true;
}

size_t Graph::numVerts() const
{
    return _adjacencies.size();
}

static bool byFirstThenLast(const Path &l,const Path &r)
{
    if(l.front()==r.front())
    {
        if (l.back()==r.back())
            return l.size()<r.size();
        else
            return l.back()<r.back();
    }
    else return l.front()<r.front();
}

PathGroup Graph::allPathsBetweenNonAdjacentVertices() const
{
    PathGroup allPaths;
    for(int startVert=0;startVert<numVerts();++startVert)
    {
        PathGroup paths = allPathsFromVOfLengthIOrLess(startVert,numVerts());
        filterPathsIncreasing(paths);
        allPaths.insert(allPaths.end(),paths.begin(),paths.end());
    }
    sort(allPaths.begin(),allPaths.end(),byFirstThenLast);
    return allPaths;
}

static bool nonIncreasing(const Path & p)
{
    return p.back()<=p.front();
}

void Graph::filterPathsIncreasing(PathGroup &paths)
{
    paths.erase(remove_if(paths.begin(),paths.end(),nonIncreasing),paths.end());
}

PathGroup Graph::allPathsFromVOfLengthIOrLess(Vertex v, size_t i) {
    if (i==0)
        return PathGroup(1,Path(1,v)); //one path with just v on it
    
    else
    {
        PathGroup shorterPaths = allPathsFromVOfLengthIOrLess(v,i-1);
        PathGroup retval(shorterPaths);
        for(const auto &path : shorterPaths)
        {
            if (path.size() != i-1)
                continue;
            Vertex endVert = path.back();
            for(int ii=0;ii<numVerts();++ii)
            {
                if(_adjacencies[endVert][ii] && notAdjacentToAnyButLast(path,ii))
                {
                    retval.push_back(path);
                    retval.back().push_back(ii);
                }
            }
        }
        return retval;
    }
}

std::ostream & operator <<(std::ostream & os, const Graph & g)
{
    os << "  ";
    for(int jj=0;jj<g.numVerts();++jj)
        os << std::setw(3) << jj;
    os << std::endl;
    
    for(int ii=0;ii<g.numVerts();++ii)
    {
        os << std::setw(3) << ii;
        for(int jj=0;jj<g.numVerts();++jj)
            os << g._adjacencies[ii][jj] << "  ";
        os << std::endl;
    }
    return os;
}

void printPathGroup(const PathGroup &pg)
{
    for(const auto &path : pg)
    {
        for(auto v : path)
            cout << v << " ";
        cout << endl;
    }
}



