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
    void addSingleObstacleNonEdgeClauses();
    void addNonEdgeVerticesNotInTriangleClauses();
    void writeCNF(const std::string &filename);
    void writeCNFWithComments(const std::string &filename);
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
    auto realVertices() const {return makeLoopRange<Vertex>(0,numRealVertices());}
    auto nonEdgeVertices() const {return makeLoopRange<Vertex>(numRealVertices(),numVertices());}
    auto allVertices() const {return makeLoopRange<Vertex>(0,numVertices());}
    auto realVerticesAfter(Vertex s) const {return makeLoopRange<Vertex>(s+1,numRealVertices());}
//    auto nonEdgeVerticesAfter() const {return makeLoopRange<Vertex>(numRealVertices(),numVertices());}
    auto allVerticesAfter(Vertex s) const {return makeLoopRange<Vertex>(s+1,numVertices());}
    
    const Graph & _g;
    SATInstance _sat;
    Vertex _nextNonEdgeVertexNumber;
    std::map<std::pair<Vertex,Vertex>,Vertex> _nonEdgeVertexNumbers;
};

#endif /* SATInstanceGenerator_hpp */
