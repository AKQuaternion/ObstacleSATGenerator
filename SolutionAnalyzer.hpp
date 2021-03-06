//
//  SolutionAnalyzer.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/27/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef SolutionAnalyzer_hpp
#define SolutionAnalyzer_hpp
#include "Variable.hpp"
#include <map>
#include <vector>

using Vertex=size_t; // TODO(hartman): duplicated from Graph.h
class SATInstanceGenerator;

class SolutionAnalyzer {
public:
    SolutionAnalyzer(std::map<std::string,bool> solution, const SATInstanceGenerator *sig);
    bool inTriangle(Vertex aa, Vertex bb, Vertex cc, Vertex xx) const;
    std::vector<Vertex> findFirstTwoInConvexHull() const;
    std::vector<Vertex> findConvexHull() const; // TODO(hartman): What should be private?
    void printConvexHull() const;
    void printInTriangles() const;

private:
    bool clockWise(Vertex a, Vertex b, Vertex c) const;
    bool inConvexHull(Vertex aa, Vertex bb) const;
    Vertex nextInConvexHull(Vertex prev) const;
    std::map<std::string,bool> _solution;
    const SATInstanceGenerator *_sat;
};
#endif /* SolutionAnalyzer_hpp */
