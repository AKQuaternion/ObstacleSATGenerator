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

void doFourCycle() {
    string fname{"4cycle"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addSingleObstacleClauses();
//    sat.addNoInteriorObstacleClauses();
    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.solve();
    sat.printSolutions();
    sat.analyzeSolutions();
}

void doKn(int n) {
    string fname{"K"+std::to_string(n)};
    Graph g(fname +".txt");
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.solve();
}

void doImpossible() {
    Graph g=Graph::kN(8);
    cout << "Making 8 point rule SAT instance." << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.add8PointRuleClauses();
    sat.solve();
    sat.analyzeSolutions();
}
void doIcosahedron() {
    string fname{"icosahedron"};
    Graph g(fname+".txt");
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);

    sat.addSingleObstacleClauses();
    sat.addFivePointRuleClauses();
    sat.writeCNF(fname);
    sat.solve();
}

void doTestAllClauses(const Graph &g) {//TODO: make this member of SATInstanceGenerator returning sat
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
    sat.addNoInteriorObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.addSingleObstacleSomeVerticesNotInTriangleClauses();
    sat.solve(10000);
    sat.printSolutions();
    sat.analyzeSolutions();
}

void doTriangularPrism() {
    Graph g{Graph::fromGFormat("triangularPrism.g.txt")};
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
//    sat.addNoInteriorObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
//    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.addSingleObstacleSomeVerticesNotInTriangleClauses();
//    sat.writeCNFWithComments(fname);
    sat.solve();
//    sat.printSolutions();
    sat.analyzeSolutions();
}

void doG3() {
    string fname{"G3"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addNoInteriorObstacleClauses();
    sat.addSingleObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.writeCNFWithComments(fname);
    sat.solve();
}

void doG5() {
    string fname{"G5"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
    sat.addNoInteriorObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.writeCNFWithComments(fname);
    sat.solve();
}

void fiveWheel2Hub() {
    string fname{"5wheel2hub"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
    sat.addNoInteriorObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.writeCNFWithComments(fname);
    sat.solve();
}

void doG13() {
    string fname{"G13"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addSingleObstacleSomeVerticesNotInTriangleClauses();
//    sat.writeCNFWithComments(fname);
    sat.solve();
}

void doG36() {
    string fname{"G36"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
//    sat.writeCNFWithComments(fname);
    sat.solve();
}

void doG37() {
    string fname{"G37"};
    Graph g(Graph::fromGFormat(fname +".g.txt"));
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    sat.addNonEdgeVertices();
    sat.addSingleObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
//    sat.writeCNFWithComments(fname);
    sat.solve();
}

void doDodecahedron() {
    string fname{"dodecahedron"};
    Graph g(fname+".txt");
    cout << "Making SAT instance for " << fname << endl;
    cout << g << endl;
    
    SATInstanceGenerator sat(g);
    
    sat.addNonEdgeVertices();
    sat.addNoInteriorObstacleClauses();
    sat.addSingleObstacleClauses();
    sat.addFourPointRuleClauses();
    sat.addFivePointRuleClauses();
    sat.addNoInteriorObstacleSomeVerticesNotInTriangleClauses();
    sat.addSingleObstacleSomeVerticesNotInTriangleClauses();
//    sat.writeCNF(fname);
    sat.solve(1);
//    sat.analyzeSolutions();
}

int main() {
    try {
//        doFourCycle();
//        doKn(5);
        doImpossible();
//        doKn(7); // Should be 1198560, Indeed: "1198560 solutions found."
//        doG3();
//        doG5();
//        doG13();
//        fiveWheel2Hub();
//        doG36();
//        doG37();
//        doDodecahedron();
//        doTriangularPrism();
//        auto v=Graph::graphsFromGFormatFile("bad_bad_graphs_conn_d2_order9_planar.txt");
//        for (const auto &g:v)
//            doTestAllClauses(g);
//
    
        
//        auto v=Graph::graphsFromGFormatFile("bad_bad_graphs_conn_d2_order9_planar.txt");
//        doTestAllClauses(v[8]);
        
    
    
    }
    catch (int &e) {
//        cout << "Exception: " << e.what() << endl;
    }

    return 0;
}
