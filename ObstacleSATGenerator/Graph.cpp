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
#include <sstream>
using std::ostringstream;
#include <exception>

Graph Graph::fromAdjacencyMatrixFile(const std::string &graphName){
    auto fileName = graphName+".txt";
    std::ifstream ifile(fileName);
    if(!ifile)
        throw std::runtime_error ("readGraph() couldn't open file " + fileName);
    
    size_t numVerts;
    if(!(ifile >> numVerts))
        throw std::runtime_error ("readGraph() error reading file " + fileName);
    Graph g;
    for(size_t ii=0;ii<numVerts;++ii) {
        g._adjacencies.push_back(vector<char>());
        for(size_t jj=0;jj<numVerts;++jj) {
            int a;
            if(!(ifile >> a))
                throw std::runtime_error ("readGraph() error reading file " + fileName);
            g._adjacencies.back().push_back(a);
        }
    }
    return g;
}

Graph Graph::kN(size_t n){
    Graph g;
    g._adjacencies.resize(n);
    for(size_t ii=0;ii<n;++ii)
        for(int jj=0;jj<n;++jj)
            g._adjacencies[ii].push_back(ii==jj?0:1);
    cout << g;
    return g;
}

Graph Graph::fromGFormat(const std::string &graphName) {
    auto fileName = graphName+".g.txt";
    std::ifstream fin(fileName);//TODO: fromGformat should assume .g.txt (and other shouldn't be constructor?)
    if(!fin)
        throw std::runtime_error ("fromGFormat() couldn't open file " + fileName);
    return fromGFormat(fin);
}

class out_of_graphs : public std::exception {};

Graph Graph::fromGFormat(std::ifstream &fin){
    Graph g;
    int numVertices;
    fin>>numVertices;
//    if(!fin && fin.eof())
//        throw out_of_graphs();
    g._adjacencies = vector<vector<char>> (numVertices,vector<char>(numVertices));
    for(int thisVertex=0;thisVertex<numVertices;++thisVertex) {
        int neighbor;
        fin >> neighbor;
        while (neighbor != -1) {
            g._adjacencies[thisVertex][neighbor]=true;
            fin >> neighbor;
        }
    }
    return g;
}

vector<Graph> Graph::graphsFromGFormatFile(const std::string &graphName) {
    auto fileName = graphName+".g.txt";
    std::ifstream fin(fileName);//TODO: fromGformat should assume .g.txt (and other shouldn't be constructor?)
    if(!fin)
        throw std::runtime_error ("fromGFormat() couldn't open file " + fileName);
    fin.exceptions(std::istream::eofbit | std::istream::badbit);
    vector<Graph> v;
    while(1)
        try {
            v.push_back(fromGFormat(fin));
        }
        catch (std::ios_base::failure &e) {//fin eof
            break;
        }
    return v;
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

PathGroup Graph::allInducedPaths() const
{
    PathGroup allPaths;
    for(int startVert=0;startVert<numVerts();++startVert)
    {
        PathGroup paths = allPathsFromVOfLengthIOrLess(startVert,numVerts());
        filterPathsIncreasingAndMoreThanTwoVertices(paths);
        allPaths.insert(allPaths.end(),paths.begin(),paths.end());
    }
    sort(allPaths.begin(),allPaths.end(),byFirstThenLast);
    return allPaths;
}

static bool nonIncreasingOrLessThan3Vertices(const Path & p)
{
    return p.back()<=p.front() || p.size() < 3;
}

void Graph::filterPathsIncreasingAndMoreThanTwoVertices(PathGroup &paths) const
{
    paths.erase(remove_if(paths.begin(),paths.end(),nonIncreasingOrLessThan3Vertices),paths.end());
}

PathGroup Graph::allPathsFromVOfLengthIOrLess(Vertex v, size_t i) const {
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
        os << std::setw(3) << ii << " ";
        for(int jj=0;jj<g.numVerts();++jj)
            os << (g._adjacencies[ii][jj]?"*":".") << "  ";
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


std::string toString(const Path &path)
{
    ostringstream os;
    os << "[";
    for (auto v : path)
        os << v << ",";
    os.seekp(-1, std::ios_base::cur);
    os << "]";
    return os.str();
}
