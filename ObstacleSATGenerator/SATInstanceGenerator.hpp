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

class SATInstanceGenerator {
public:
    SATInstanceGenerator(const Graph &g);
private:
    const Graph & _g;
};
#endif /* SATInstanceGenerator_hpp */
