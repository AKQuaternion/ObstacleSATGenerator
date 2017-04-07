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
#include <algorithm>
using std::swap;
#include <utility>
using std::make_pair;

SATInstanceGenerator::SATInstanceGenerator(const Graph &g) :_g(g),_nextNonEdgeVertexNumber(g.numVerts()) {
    addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
}

size_t SATInstanceGenerator::numRealVertices() const {
    return _g.numVerts();
}

size_t SATInstanceGenerator::numVertices() const {
    return _nextNonEdgeVertexNumber;
}

Variable SATInstanceGenerator::variableForTriangle(Vertex a, Vertex b, Vertex c) const {
    if(c<a || b<a)
        return variableForTriangle(b,c,a);
    // invariant a is smallest
    if(c<b)
        return -variableForTriangle(a,c,b);
    // invariant a<b<c
    
    if (a==b || b==c || a==c)
        throw runtime_error("variableFor() called with two equal vertices.");
    
    return {"(" + to_string(a) + "," + to_string(b) + "," + to_string(c) + ")"};
}

Vertex SATInstanceGenerator::vertexForNonEdge(Vertex a, Vertex b) {
    if(a>b)
        swap (a,b);
    auto ne = make_pair(a,b);
    auto i = _nonEdgeVertexNumbers.find(ne);
    if (i == _nonEdgeVertexNumbers.end())
        i = _nonEdgeVertexNumbers.insert(i,make_pair(ne,_nextNonEdgeVertexNumber++));
    return i->second;
}

void SATInstanceGenerator::addNonEdgeVerticesEssentiallyOnNonEdgeClauses()
{
    for(int aa=0;aa<numRealVertices();++aa)
        for(int bb=0;bb<numRealVertices();++bb) {
            if (aa==bb || _g.edge(aa,bb))
                continue;
            //aa,bb is a non-edge now
            auto zz = vertexForNonEdge(aa,bb);
            for(int cc=0;cc<numRealVertices();++cc) {
                if (aa==cc||bb==cc)
                    continue;
                // abc -> abz and bcz
                // -abc or (abz and bcz)
                // (-abc or abz) and (-abc or bcz)
                auto abc = variableForTriangle(aa, bb, cc);
                auto abz = variableForTriangle(aa, bb, zz);
                auto bcz = variableForTriangle(bb, cc, zz);
                _sat.addClause({-abc,abz});
                _sat.addClause({-abc,bcz});
            }
        }
 }

void SATInstanceGenerator::addFivePointRuleClauses() {
    for(Vertex aa=0;aa<numVertices();++aa) {
        for(Vertex bb=aa+1;bb<numVertices();++bb) {
            if(aa==bb) continue;
            for(Vertex cc=bb+1;cc<numVertices();++cc) {
                if(cc==aa || cc==bb) continue;
                for(Vertex dd=cc+1;dd<numVertices();++dd) {
                    if(dd==aa||dd==bb||dd==cc) continue;
                    for(Vertex ee=dd+1;ee<numVertices();++ee) {
                        if(ee==aa||ee==bb||ee==cc||ee==dd)
                            continue;
                        auto abc = variableForTriangle(aa,bb,cc);
                        auto abd = variableForTriangle(aa,bb,dd);
                        auto abe = variableForTriangle(aa,bb,ee);
                        auto acd = variableForTriangle(aa,cc,dd);
                        auto ace = variableForTriangle(aa,cc,ee);
                        auto ade = variableForTriangle(aa,dd,ee);

                        _sat.addClause({ abc, abd, abe, acd,-ace, ade,});
                        _sat.addClause({ abc, abd, abe,-acd, ace,-ade,});
                        _sat.addClause({ abc, abd,-abe, acd, ace,-ade,});
                        _sat.addClause({ abc, abd,-abe,-acd,-ace, ade,});
                        _sat.addClause({ abc,-abd, abe, acd, ace, ade,});
                        _sat.addClause({ abc,-abd, abe,-acd,-ace,-ade,});
                        _sat.addClause({ abc,-abd,-abe, acd,-ace,-ade,});
                        _sat.addClause({ abc,-abd,-abe,-acd, ace, ade,});
                        _sat.addClause({-abc, abd, abe, acd,-ace,-ade,});
                        _sat.addClause({-abc, abd, abe,-acd, ace, ade,});
                        _sat.addClause({-abc, abd,-abe, acd, ace, ade,});
                        _sat.addClause({-abc, abd,-abe,-acd,-ace,-ade,});
                        _sat.addClause({-abc,-abd, abe, acd, ace,-ade,});
                        _sat.addClause({-abc,-abd, abe,-acd,-ace, ade,});
                        _sat.addClause({-abc,-abd,-abe, acd,-ace, ade,});
                        _sat.addClause({-abc,-abd,-abe,-acd, ace,-ade,});
                    }
                }
            }
        }
    }
}
