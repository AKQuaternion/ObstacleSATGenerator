//
//  SolutionAnalyzer.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/27/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef SolutionAnalyzer_hpp
#define SolutionAnalyzer_hpp
#include <map>
#include "Variable.hpp"
#include <vector>

using Vertex=size_t; //TODO: duplicated from Graph.h
class SATInstanceGenerator;

class SolutionAnalyzer {
public:
    SolutionAnalyzer(const std::map<Variable,bool> &solutions, const SATInstanceGenerator &sig);
    bool clockWise(Vertex a, Vertex b, Vertex c) const;
    bool inConvexHull(Vertex aa, Vertex bb) const;
    bool inTriangle(Vertex aa, Vertex bb, Vertex cc, Vertex xx);
    std::vector<Vertex> findFirstTwoInConvexHull();
    std::vector<Vertex> findConvexHull(); //TODO: What should be private?

private:
    size_t _numVertices;
    std::map<Variable,bool> _solution;
    const SATInstanceGenerator &_sat;
};
#endif /* SolutionAnalyzer_hpp */
