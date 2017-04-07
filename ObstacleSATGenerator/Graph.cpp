/*
 * Graph.cpp
 *
 *  Created on: Apr 23, 2012
 *      Author: hartman
 */

#include "Graph.h"

#include <fstream>
#include <vector>
using std::vector;
#include <iostream>
using std::cout;
using std::endl;

Graph::Graph(const std::string & fname)
{
    std::ifstream ifile((fname+".txt").c_str());
    if(!ifile)
        throw std::runtime_error ("readGraph() couldn't open file " + (fname+".txt"));
    
    unsigned int numVerts;
    ifile >> numVerts;
    for(size_t ii=0;ii<numVerts;++ii)
    {
        _adj.push_back(vector<int>());
        for(size_t jj=0;jj<numVerts;++jj)
        {
            int a;
            if(!(ifile >> a))
                throw std::runtime_error ("readGraph() error reading file " + (fname+".txt"));
            _adj.back().push_back(a);
        }
    }
}

bool Graph::notAdjacentToAnyButLast(const Path & p, Vertex vert) const
{
    for(size_t ii=0;ii<p.size()-1;++ii)
        if(_adj[p[ii]][vert])
            return false;
    return true;
}

size_t Graph::numVerts() const
{
    return _adj.size();
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

Pathlist Graph::allPathsBetweenNonAdjacentVertices()
{
    Pathlist allPaths;
    for(int startVert=0;startVert<numVerts();++startVert)
    {
        Pathlist paths = allPathsFromVOfLengthIOrLess(startVert,numVerts());
        filterPathsIncreasing(paths);
        //   filterPathsNonAdjacentEndpoints(paths); //no longer necessary, paths are induced
        allPaths.insert(allPaths.end(),paths.begin(),paths.end());
    }
    sort(allPaths.begin(),allPaths.end(),byFirstThenLast);
    return allPaths;
}

static bool nonIncreasing(const Path & p)
{
    return p.back()<=p.front();
}

void Graph::filterPathsIncreasing(Pathlist &paths)
{
    paths.erase(remove_if(paths.begin(),paths.end(),nonIncreasing),paths.end());
}

Pathlist Graph::allPathsFromVOfLengthIOrLess(Vertex v, size_t i)
{
    if (i==0)
    {
        return Pathlist(1,Path(1,v)); //one path with just v on it
    }
    
    else
    {
        vector<vector<int> > shorterPaths = allPathsFromVOfLengthIOrLess(v,i-1);
        vector<vector<int> > retval(shorterPaths);
        for(size_t path=0;path<shorterPaths.size();++path)
        {
            if (shorterPaths[path].size() != i-1)
                continue;
            int endVert = shorterPaths[path].back();
            for(int ii=0;ii<numVerts();++ii)
            {
                if(_adj[endVert][ii] && notAdjacentToAnyButLast(shorterPaths[path],ii))
                {
                    retval.push_back(shorterPaths[path]);
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
        os << char('a'+jj) << ' ';
    os << std::endl;
    
    for(int ii=0;ii<g.numVerts();++ii)
    {
        os << char('a'+ii) << ' ';
        for(int jj=0;jj<g.numVerts();++jj)
            os << g._adj[ii][jj] << " ";
        os << std::endl;
    }
    return os;
}

void printPathList(const Pathlist &pl)
{
    for(size_t ii=0;ii<pl.size();++ii)
    {
        for(size_t jj=0;jj<pl[ii].size();++jj)
            cout << pl[ii][jj] << " ";
        cout << endl;
    }
}



