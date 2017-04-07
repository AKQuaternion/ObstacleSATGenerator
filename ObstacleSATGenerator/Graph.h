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
    PathGroup allPathsBetweenNonAdjacentVertices();
    
private:
    bool notAdjacentToAnyButLast(const Path &p, Vertex vert) const;
    PathGroup allPathsFromVOfLengthIOrLess(Vertex v, size_t i);
    void filterPathsIncreasing(PathGroup &paths);
    
    std::vector<std::vector<int> > _adjacencies;
};

std::ostream & operator<<(std::ostream &, const Graph & g);
void printPathGroup(const PathGroup &pl);

#endif /* GRAPH_H_ */
