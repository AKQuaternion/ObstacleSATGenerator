//
//  main.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include <iostream>
using std::cout;
using std::endl;
#include <string>
using std::string;

#include "SATInstanceGenerator.hpp"

int main() {
    string fname{"icosahedron"};
    try {
        Graph g(fname+".txt");
        cout << g;
        cout << endl;

        for (auto p:g.allInducedPaths())
            cout << toString(p) << endl;
        SATInstanceGenerator sat(g);
//        sat.addNonEdgeVertices();
//        sat.show();
        sat.addNoSingleObstacleNonEdgeClauses();
        sat.show();
        sat.addFivePointRuleClauses();
        sat.show();
//        sat.addNonEdgeVerticesNotInTriangleClauses();
//        sat.show();
        sat.writeCNF(fname);
    }
    catch (std::exception &e)
    {
        cout << "Exception: " << e.what() << endl;
    }
    
    return 0;
}
