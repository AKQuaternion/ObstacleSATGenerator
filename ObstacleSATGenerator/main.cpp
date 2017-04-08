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
    try {
        Graph g("G13.txt");
        cout << g;
        cout << endl;
        
        SATInstanceGenerator sat(g);
        sat.addNonEdgeVertices();
        sat.addNoInteriorObstacleNonEdgeClauses();
        sat.addFivePointRuleClauses();
        sat.writeCNF("G13");
    }
    catch (std::exception &e)
    {
        cout << "Exception: " << e.what() << endl;
    }
    
    return 0;
}
