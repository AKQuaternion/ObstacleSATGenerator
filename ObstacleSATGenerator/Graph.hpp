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

typedef size_t Vertex;
typedef std::vector<Vertex> Path;
typedef std::vector<Path > PathGroup;

class Graph
{
    friend std::ostream & operator <<(std::ostream & os, const Graph & g);
    
public:
    Graph(const std::string &fname);
    size_t numVerts() const;
    PathGroup allInducedPaths() const;
    bool edge(Vertex i, Vertex j) const;
    
private:
    bool notAdjacentToAnyButLast(const Path &p, Vertex vert) const;
    PathGroup allPathsFromVOfLengthIOrLess(Vertex v, size_t i) const;
    void filterPathsIncreasing(PathGroup &paths) const;
    
    std::vector<std::vector<int> > _adjacencies;
};

std::ostream & operator<<(std::ostream &, const Graph & g);
void printPathGroup(const PathGroup &pl);
std::string toString(const Path &p);
#endif /* GRAPH_H_ */
