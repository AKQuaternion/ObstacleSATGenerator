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
#include <vector>

class SATInstanceGenerator {
    friend class SolutionAnalyzer; //TODO: Untangle this. Really friend?
    // maybe a subclass that both can use.
public:
    SATInstanceGenerator(const Graph &g);
    void addNonEdgeVertices();
    void addFourPointRuleClauses();
    void addFivePointRuleClauses();
    void addNoInteriorObstacleClauses();
    void addSingleObstacleClauses();
    void addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    void addSingleObstacleSomeVerticesNotInTriangleClauses();
    void writeCNF(const std::string &filename);
    void writeCNFWithComments(const std::string &filename);
    void show() const;
    void printSolutions() const;
    void analyzeSolutions() const;
    bool solve(size_t maxSolutions=1000);
    void add8PointRuleClauses();
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
    auto realVerticesAfter(Vertex s) const {return makeLoopRange<Vertex>(s+1,numRealVertices());}
    auto allVertices() const {return makeLoopRange<Vertex>(0,numVertices());}
    auto allVerticesAfter(Vertex s) const {return makeLoopRange<Vertex>(s+1,numVertices());}
    auto nonEdgeVertices() const {return makeLoopRange<Vertex>(numRealVertices(),numVertices());}
//    auto nonEdgeVerticesAfter() const {return makeLoopRange<Vertex>(numRealVertices(),numVertices());}
    
    const Graph & _g;
    SATInstance _sat;
    std::map<std::pair<Vertex,Vertex>,Vertex> _nonEdgeVertexNumbers;
    std::vector<std::string> _nonEdgeVertexNames;
    std::vector<std::map<Variable,bool>> _solutions;
};

#endif /* SATInstanceGenerator_hpp */
