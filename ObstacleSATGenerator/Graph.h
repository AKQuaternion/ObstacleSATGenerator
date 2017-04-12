/*
 * Graph.h
 *
 *  Created on: Apr 23, 2012
 *      Author: hartman
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <string>
#include <iostream>
#include <vector>
#include <stdexcept>

typedef std::vector<int> Path;
typedef std::vector<std::vector<int> > Pathlist;

class Graph
{
    friend std::ostream & operator <<(std::ostream & os, const Graph & g);
    
public:
    typedef unsigned int Vertex;
    
    Graph(const std::string &fname);
    size_t numVerts() const;
    Pathlist allInducedPaths() const;
    
private:
    bool notAdjacentToAnyButLast(const Path &p, Vertex vert) const;
    Pathlist allPathsFromVOfLengthIOrLess(Vertex v, size_t i) const;
    void filterPathsIncreasing(Pathlist &paths) const;
    
    std::vector<std::vector<int> > _adj;
    
};

std::ostream & operator<<(std::ostream &, const Graph & g);
void printPathList(const Pathlist &pl);

#endif /* GRAPH_H_ */
