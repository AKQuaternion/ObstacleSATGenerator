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
#include "RangeUtilities.hpp"

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
    void addClauses6and7(const Path &p, Vertex c, Vertex d);
    void addClauses8and9(const Path &p, Vertex c, Vertex d);

    bool adjacent(Vertex a, Vertex b) const;
    void addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
    size_t numVertices() const;
    size_t numRealVertices() const;
    LoopRange<Vertex> realVertices() const {return {0,numRealVertices()};}
    LoopRange<Vertex> nonEdgeVertices() const {return {numRealVertices(),numVertices()};}
    LoopRange<Vertex> allVertices() const {return {0,numVertices()};}
    LoopRange<Vertex> realVerticesAfter(Vertex s) const {return {s+1,numRealVertices()};}
    LoopRange<Vertex> allVerticesAfter(Vertex s) const {return {s+1,numVertices()};}
    
    const Graph & _g;
    SATInstance _sat;
    Vertex _nextNonEdgeVertexNumber;
    std::map<std::pair<Vertex,Vertex>,Vertex> _nonEdgeVertexNumbers;
};

#endif /* SATInstanceGenerator_hpp */
