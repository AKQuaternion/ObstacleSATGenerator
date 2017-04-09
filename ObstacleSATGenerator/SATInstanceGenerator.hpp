//
//  SATInstanceGenerator.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef SATInstanceGenerator_hpp
#define SATInstanceGenerator_hpp
#include "Graph.hpp"
#include "SATInstance.hpp"
#include <string>

class SATInstanceGenerator {
public:
    SATInstanceGenerator(const Graph &g);
    void addNonEdgeVertices();
    void addFourPointRuleClauses();
    void addFivePointRuleClauses();
    void addNoInteriorObstacleNonEdgeClauses();
    void addNoSingleObstacleNonEdgeClauses();
    void addNonEdgeVerticesNotInTriangleClauses();
    void writeCNF(const std::string &filename);
    void show() const;
    
private:
    Variable variableForTriangle(Vertex a, Vertex b, Vertex c) const;
    Variable variableForsab(Vertex a, Vertex b) const;
    Variable variableForkPcd(const Path &path, Vertex cc,Vertex dd) const;
    Variable variableForsabcd(Vertex a, Vertex b, Vertex c, Vertex d) const;
    Vertex vertexForNonEdge(Vertex a, Vertex b);

    bool adjacent(Vertex a, Vertex b) const;
    void addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
    size_t numVertices() const;
    size_t numRealVertices() const;
//    string nameFor(Vertex) const;
    
    const Graph & _g;
    SATInstance _sat;
    Vertex _nextNonEdgeVertexNumber;
    std::map<std::pair<Vertex,Vertex>,Vertex> _nonEdgeVertexNumbers;
};

#endif /* SATInstanceGenerator_hpp */
