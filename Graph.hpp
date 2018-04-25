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

using Vertex = size_t;
using Path = std::vector<Vertex>;
using PathGroup = std::vector<Path>;

class Graph
{
    friend std::ostream & operator <<(std::ostream & os, const Graph & g);
    
public:
    Graph() = default;
    size_t numVerts() const;
    PathGroup allInducedPaths() const;
    bool edge(Vertex i, Vertex j) const;
    static Graph fromAdjacencyMatrixFile(const std::string &graphName);
    static Graph fromGFormat(const std::string &graphName);
    static Graph kN(size_t n);
    static std::vector<Graph> graphsFromGFormatFile(const std::string& graphName);
private:
    static Graph fromGFormat(std::ifstream &fin);
    bool notAdjacentToAnyButLast(const Path &p, Vertex vert) const;
    PathGroup allPathsFromVOfLengthIOrLess(Vertex v, size_t i) const;
    void filterPathsIncreasingAndMoreThanTwoVertices(PathGroup &paths) const;
    
    std::vector<std::vector<char> > _adjacencies;
};

std::ostream & operator<<(std::ostream &, const Graph & g);
void printPathGroup(const PathGroup &pl);
std::string toString(const Path &p);
#endif /* GRAPH_H_ */
