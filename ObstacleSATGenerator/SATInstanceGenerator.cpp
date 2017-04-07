//
//  SATInstanceGenerator.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "SATInstanceGenerator.hpp"
#include <stdexcept>
using std::runtime_error;
#include <string>
using std::string;
using std::to_string;

SATInstanceGenerator::SATInstanceGenerator(const Graph &g) :_g(g) {
}

Variable SATInstanceGenerator::variableFor(Vertex a, Vertex b, Vertex c) {
    if(c<a || b<a)
        return variableFor(b,c,a);
    // invariant a is smallest
    if(c<b)
        return -variableFor(a,c,b);
    // invariant a<b<c
    
    if (a==b || b==c || a==c)
        throw runtime_error("variableFor() called with two equal vertices.");
    
    return {"(" + to_string(a) + "," + to_string(b) + "," + to_string(c) + ")"};
}
