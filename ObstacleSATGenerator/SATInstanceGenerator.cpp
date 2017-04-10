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
#include <iostream>
using std::cout;
using std::endl;

SATInstanceGenerator::SATInstanceGenerator(const Graph &g) :_g(g),_nextNonEdgeVertexNumber(g.numVerts()) {
}

void SATInstanceGenerator::addNonEdgeVertices() {
    for(int aa=0;aa<numRealVertices();++aa)
        for(int bb=aa+1;bb<numRealVertices();++bb)
            if (!_g.edge(aa, bb))
                _nonEdgeVertexNumbers[make_pair(aa, bb)] =_nextNonEdgeVertexNumber++;
    addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
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
    if (numRealVertices()==numVertices())
        std::cout << "You almost surely want to call addNonEdgeVertices() before calling addNonEdgeVerticesEssentiallyOnNonEdgeClauses()" << std::endl;
    for(int aa=0;aa<numRealVertices();++aa)
        for(int bb=aa+1;bb<numRealVertices();++bb) {
            if (adjacent(aa,bb))
                continue;
            auto zz = vertexForNonEdge(aa,bb);
            for(int xx=0;xx<numRealVertices();++xx) {
                if (xx==aa||xx==bb||xx==zz)
                    continue;
                // axb -> axz and xbz
                // -axb or (axz and xbz)
                // (-axb or axz) and (-axb or xbz)
                auto axb = variableForTriangle(aa, xx, bb);
                auto axz = variableForTriangle(aa, xx, zz);
                auto xbz = variableForTriangle(xx, bb, zz);
                _sat.addClause({-axb,axz});
                _sat.addClause({-axb,xbz});
                //then add the reflections
                _sat.addClause({axb,-axz});
                _sat.addClause({axb,-xbz});
            }
        }
 }

void SATInstanceGenerator::addFivePointRuleClauses() {
    for(Vertex aa=0;aa<numRealVertices();++aa)
        for(Vertex bb=0;bb<numRealVertices();++bb) {
            if(aa==bb) continue;
            for(Vertex cc=bb+1;cc<numRealVertices();++cc) {
                if(aa==cc) continue;
                for(Vertex dd=cc+1;dd<numRealVertices();++dd) {
                    if(aa==dd) continue;
                    for(Vertex ee=dd+1;ee<numVertices();++ee) {
                        if(aa==ee) continue;
                        
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
        }
}

void SATInstanceGenerator::addFourPointRuleClauses() {
    for(Vertex aa=0;aa<numRealVertices();++aa)
        for(Vertex bb=aa+1;bb<numRealVertices();++bb)
            for(Vertex cc=bb+1;cc<numRealVertices();++cc)
                for(Vertex dd=cc+1;dd<numVertices();++dd) {
                    
                    auto abc = variableForTriangle(aa,bb,cc);
                    auto abd = variableForTriangle(aa,bb,dd);
                    auto acd = variableForTriangle(aa,cc,dd);
                    auto bcd = variableForTriangle(bb,cc,dd);
                    
                    _sat.addClause({ abc,-abd, acd,-bcd,});
                    _sat.addClause({-abc, abd,-acd, bcd,});
                }
}

Variable SATInstanceGenerator::variableForsab(Vertex a, Vertex b) const {
    if(b>a)
        return -variableForsab(b, a);
    return {"s{"+to_string(a)+","+to_string(b)+"}"};
}

Variable SATInstanceGenerator::variableForkPcd(const Path &path, Vertex cc,Vertex dd) const{
    return {"k_"+toString(path)+",{" + to_string(cc) + "," + to_string(dd) +"}"};
}

Variable SATInstanceGenerator::variableForsabcd(Vertex a, Vertex b, Vertex c, Vertex d) const {
    if(b>a)
        return -variableForsabcd(b, a, c, d);
    return {"s{"+to_string(a)+","+to_string(b)+","+to_string(c) + "," + to_string(d) +"}"};
    //TODO:: discuss with Glenn, what is canonical here?
    //(Should be negated if a,b flipped, no change for c,d, yes?
}

void SATInstanceGenerator::addNoInteriorObstacleNonEdgeClauses(){
        for (const auto &path:_g.allInducedPaths()) {// paths are lexicographically sorted by first then last vertex
            auto aa = path.front();
            auto bb = path.back();
            auto sab = variableForsab(aa, bb);

            Clause c{sab};
            for(size_t ss=1;ss<path.size()-1;++ss)
                c.add(variableForTriangle(aa, bb, path[ss]));
            _sat.addClause(c);
            _sat.addClause(c.reflected());
        }
}

void SATInstanceGenerator::addNoSingleObstacleNonEdgeClauses(){
    for (const auto &path:_g.allInducedPaths()) {// paths are lexicographically sorted by first then last vertex with first<last.
        auto aa = path.front();
        auto bb = path.back();
        for(Vertex cc=0;cc<numRealVertices();++cc)
            for(Vertex dd=cc+1;dd<numRealVertices();++dd)  {//TODO: Consider using numVertices() here? Ask Glenn.
                if(adjacent(cc, dd) || (aa==cc && bb==dd))
                    continue;
                addClauses6and7(path, cc, dd);
                addClauses8and9(path, cc, dd);
            }
    }
}

void SATInstanceGenerator::addClauses6and7(const Path &p, Vertex c, Vertex d) {
    Clause pathPart;
    for (auto v:p)
        pathPart += variableForTriangle(c, d, v);
    auto kPcd = variableForkPcd(p,c,d);
    auto c6 = pathPart + kPcd;
    auto c7 = pathPart.reflected() + kPcd;
    _sat.addClause(c6);
    _sat.addClause(c7);
}

void SATInstanceGenerator::addClauses8and9(const Path &p, Vertex c, Vertex d) {
    auto s_abcd = variableForsabcd(p.front(),p.back(),c,d); //TODO:: discuss with Glenn, what is canonical here?
    //(Should be negated if a,b flipped, no change for c,d, yes?
    
    //TODO: build and add clauses (8) and (9)

}

void SATInstanceGenerator::addNonEdgeVerticesNotInTriangleClauses() {
    //-abc V  azb V  bzc V  cza says "abc is counterclockwise, or (it's clockwise so) z is outsize of abc"
    // abc V -azb V -bzc V -cza
    for(int aa=0;aa<numRealVertices();++aa)
        for(int bb=aa+1;bb<numRealVertices();++bb)
            for(int cc=bb+1;cc<numRealVertices();++cc)
                for(auto zz=numRealVertices(); zz<numVertices(); ++zz)
                {
                    if (!adjacent(aa,bb) || !adjacent(aa,cc) || !adjacent(bb,cc))
                        continue;
                    auto abc = variableForTriangle(aa,bb,cc);
                    auto azb = variableForTriangle(aa,zz,bb);
                    auto bzc = variableForTriangle(bb,zz,cc);
                    auto cza = variableForTriangle(cc,zz,aa);

                    Clause c({-abc,azb,bzc,cza});
                    _sat.addClause(c);
                    _sat.addClause(c.reflected());
                }
}

void SATInstanceGenerator::writeCNF(const string &filename) {
    _sat.writeCNF(filename);
}

void SATInstanceGenerator::show() const {
    std::cout << _sat.numClauses() << std::endl;
}
