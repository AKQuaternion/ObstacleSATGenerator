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

    
private:
    Variable variableForTriangle(Vertex a, Vertex b, Vertex c) const;
    Vertex vertexForNonEdge(Vertex a, Vertex b);

    void addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
    void addFourPointRuleClauses();
    void addFivePointRuleClauses();
    size_t numVertices() const;
    size_t numRealVertices() const;
//    string nameFor(Vertex) const;
    
    const Graph & _g;
    SATInstance _sat;
    Vertex _nextNonEdgeVertexNumber;
    std::map<std::pair<Vertex,Vertex>,Vertex> _nonEdgeVertexNumbers;
};


#endif /* SATInstanceGenerator_hpp */
