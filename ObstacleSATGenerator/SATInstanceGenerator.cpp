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
    addNonEdgeVertices(); //Always do this
    addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
}

void SATInstanceGenerator::addNonEdgeVertices() {
    for(int aa=0;aa<numRealVertices();++aa)
        for(int bb=aa+1;bb<numRealVertices();++bb)
            if (!_g.edge(aa, bb))
                _nonEdgeVertexNumbers[make_pair(aa, bb)] =_nextNonEdgeVertexNumber++;
}

size_t SATInstanceGenerator::numRealVertices() const {
    return _g.numVerts();
}

size_t SATInstanceGenerator::numVertices() const {
    return _nextNonEdgeVertexNumber;
}

bool SATInstanceGenerator::adjacent(Vertex a, Vertex b) const {
    if (a >= numRealVertices() || b >= numRealVertices())
        return false;
    else
        return _g.edge(a,b);
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
        throw runtime_error("vvertexForNonEdge() called with unknown non-edge.");
    return i->second;
}

void SATInstanceGenerator::addNonEdgeVerticesEssentiallyOnNonEdgeClauses() {
    for(int aa=0;aa<numRealVertices();++aa)
        for(int bb=0;bb<numRealVertices();++bb) {
            if (aa==bb || adjacent(aa,bb))
                continue;
            //aa,bb is a non-edge now
            auto zz = vertexForNonEdge(aa,bb);
            for(int xx=0;xx<numVertices();++xx) {
                if (aa==xx||bb==xx)
                    continue;
                // axb -> axz and xbz
                // -axb or (axz and xbz)
                // (-axb or axz) and (-axb or xbz)
                auto axb = variableForTriangle(aa, xx, bb);
                auto axz = variableForTriangle(aa, xx, zz);
                auto xbz = variableForTriangle(xx, bb, zz);
                _sat.addClause({-axb,axz});
                _sat.addClause({-axb,xbz});
            }
        }
 }

void SATInstanceGenerator::addFivePointRuleClauses() {
    for(Vertex aa=0;aa<numVertices();++aa)
        for(Vertex bb=0;bb<numVertices();++bb) {
            if(aa==bb) continue;
            for(Vertex cc=bb+1;cc<numVertices();++cc)
                for(Vertex dd=cc+1;dd<numVertices();++dd)
                    for(Vertex ee=dd+1;ee<numVertices();++ee) {
                        auto abc = variableForTriangle(aa,bb,cc);
                        auto abd = variableForTriangle(aa,bb,dd);
                        auto abe = variableForTriangle(aa,bb,ee);
                        auto acd = variableForTriangle(aa,cc,dd);
                        auto ace = variableForTriangle(aa,cc,ee);
                        auto ade = variableForTriangle(aa,dd,ee);

                        _sat.addClause({ abc, abd, abe, acd,-ace, ade});
                        _sat.addClause({ abc, abd, abe,-acd, ace,-ade});
                        _sat.addClause({ abc, abd,-abe, acd, ace,-ade});
                        _sat.addClause({ abc, abd,-abe,-acd,-ace, ade});
                        _sat.addClause({ abc,-abd, abe, acd, ace, ade});
                        _sat.addClause({ abc,-abd, abe,-acd,-ace,-ade});
                        _sat.addClause({ abc,-abd,-abe, acd,-ace,-ade});
                        _sat.addClause({ abc,-abd,-abe,-acd, ace, ade});
                        _sat.addClause({-abc, abd, abe, acd,-ace,-ade});
                        _sat.addClause({-abc, abd, abe,-acd, ace, ade});
                        _sat.addClause({-abc, abd,-abe, acd, ace, ade});
                        _sat.addClause({-abc, abd,-abe,-acd,-ace,-ade});
                        _sat.addClause({-abc,-abd, abe, acd, ace,-ade});
                        _sat.addClause({-abc,-abd, abe,-acd,-ace, ade});
                        _sat.addClause({-abc,-abd,-abe, acd,-ace, ade});
                        _sat.addClause({-abc,-abd,-abe,-acd, ace,-ade});
                    }
        }
}

void SATInstanceGenerator::addFourPointRuleClauses() {
    for(Vertex aa=0;aa<numVertices();++aa)
        for(Vertex bb=aa+1;bb<numVertices();++bb)
            for(Vertex cc=bb+1;cc<numVertices();++cc)
                for(Vertex dd=cc+1;dd<numVertices();++dd) {
                    
                    auto abc = variableForTriangle(aa,bb,cc);
                    auto abd = variableForTriangle(aa,bb,dd);
                    auto acd = variableForTriangle(aa,cc,dd);
                    auto bcd = variableForTriangle(bb,cc,dd);
                    
                    _sat.addClause({ abc,-abd, acd,-bcd,});
                    _sat.addClause({-abc, abd,-acd, bcd,});
                }
}

static Clause reflected(Clause c) {
    for(auto &v:c)
        v = -v;
    return c;
}

Variable SATInstanceGenerator::variableSForNonEdge_ab(Vertex a, Vertex b) const {
    if(b>a)
        return -variableSForNonEdge_ab(b, a);
    return {"s{"+to_string(a)+","+to_string(b)+"}"};
}

void SATInstanceGenerator::addNoInteriorObstacleNonEdgeClauses(){
    {
        for (const auto &path:_g.allPathsBetweenNonAdjacentVertices())
        {
            if (path.size() <= 2)
                throw std::runtime_error("Oops, path too short in addNoInteriorObstacleNonEdgeClauses()");
            
            auto aa = path.front();
            auto bb = path.back();
            auto sab = variableSForNonEdge_ab(aa, bb);

            Clause c{sab};
            for(size_t ss=1;ss<path.size()-1;++ss)
                c.push_back(variableForTriangle(aa, bb, path[ss]));
            _sat.addClause(c);
            _sat.addClause(reflected(c));
        }
    }
}

