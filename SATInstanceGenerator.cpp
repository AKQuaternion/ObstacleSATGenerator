//
//  SATInstanceGenerator.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

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
#include <cassert>

#include "SATInstanceGenerator.hpp"
#include "RangeUtilities.hpp"
#include "SolutionAnalyzer.hpp"

SATInstanceGenerator::SATInstanceGenerator(const Graph &g) :_g(g) {
    _sat.addClause({variableForTriangle(0, 1, 2)});
}

void SATInstanceGenerator::addNonEdgeVertices() {
    for(auto aa:realVertices())
        for(auto bb:realVerticesAfter(aa))
            if (!_g.edge(aa, bb)) {
                _nonEdgeVertexNumbers[make_pair(aa, bb)] = numRealVertices()+_nonEdgeVertexNames.size();
                _nonEdgeVertexNames.push_back("<" + to_string(aa) + "," + to_string(bb) +">");
            }
    addNonEdgeVerticesEssentiallyOnNonEdgeClauses();
}

size_t SATInstanceGenerator::numRealVertices() const {
    return _g.numVerts();
}

size_t SATInstanceGenerator::numVertices() const {
    return numRealVertices() + _nonEdgeVertexNumbers.size();
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
    
    auto cName = (c>=numRealVertices()?_nonEdgeVertexNames[c-numRealVertices()] : to_string(c));
    
    return {"T(" + to_string(a) + "," + to_string(b) + "," + cName + ")"};
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
    cout << "Adding clauses to enforce that the special \"non-edge\" vertices lie essentially on the segment of the non-edge." << endl;
    if (numRealVertices()==numVertices())
        std::cout << "You almost surely want to call addNonEdgeVertices() before calling addNonEdgeVerticesEssentiallyOnNonEdgeClauses()" << std::endl;
    for(auto aa:realVertices())
        for(auto bb:realVerticesAfter(aa)) {
            if (adjacent(aa,bb))
                continue;
            auto zz = vertexForNonEdge(aa,bb);
            for(auto xx : realVertices()) {
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
    cout << "Adding clauses to enforce the five point rule." << endl;
    for(auto aa : realVertices())
        for(auto bb : realVertices()) {
            if(aa==bb) continue;
            for(auto cc : realVerticesAfter(bb)) {
                if(aa==cc) continue;
                for(auto dd : realVerticesAfter(cc)) {
                    if(aa==dd) continue;
                    for(auto ee : allVerticesAfter(dd)) {
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
    cout << "Adding clauses to enforce the four point rule." << endl;
    for(auto aa : realVertices())
        for(auto bb : realVerticesAfter(aa))
            for(auto cc : realVerticesAfter(bb))
                for(auto dd : realVerticesAfter(cc)) {
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
    if(c>d)
        swap(c,d);
    return {"s{"+to_string(a)+","+to_string(b)+","+to_string(c) + "," + to_string(d) +"}"};
}

void SATInstanceGenerator::addNoInteriorObstacleClauses(){
    cout << "Adding clauses to enforce no interior obstacle." << endl;
    for (const auto &path:_g.allInducedPaths()) {
        auto aa = path.front();
        auto bb = path.back();
        auto sab = variableForsab(aa, bb);
        
        Clause c{sab};
        for(auto ss:interior(path))
            c += variableForTriangle(aa, bb, ss);
        _sat.addClause(c);              //clause 8 from the paper
        _sat.addClause(c.reflected());  //clause 9 from the paper
    }
}

void SATInstanceGenerator::addSingleObstacleClauses(){
    cout << "Adding clauses to enforce a single obstacle." << endl;
    for (const auto &path:_g.allInducedPaths()) {
        auto aa = path.front();
        auto bb = path.back();
        assert(aa<bb);
        for(auto cc : realVertices())
            for(auto dd : realVerticesAfter(cc))  {
                if(adjacent(cc, dd) || (aa==cc && bb==dd))
                    continue;
                addClauses4and5(path, cc, dd);
                addClauses6and7(path, cc, dd);
            }
    }
}

void SATInstanceGenerator::addClauses4and5(const Path &p, Vertex c, Vertex d) {
    Clause pathPart;
    for (auto v:p)
        if (v != c && v != d)
            pathPart += variableForTriangle(c, d, v);
    auto kPcd = variableForkPcd(p,c,d);
    auto c4 = pathPart + kPcd;
    auto c5 = pathPart.reflected() + kPcd;
    _sat.addClause(c4);
    _sat.addClause(c5);
}

void SATInstanceGenerator::addClauses6and7(const Path &p, Vertex c, Vertex d) {
    auto a = p.front();
    auto b = p.back();
    Clause pathPart;
    for (auto v:interior(p))
        pathPart += variableForTriangle(a, b, v);
    auto s_abcd = variableForsabcd(a,b,c,d);
    auto kPcd = variableForkPcd(p,c,d);
    auto c6 = -kPcd + -s_abcd + pathPart;
    auto c7 = -kPcd +  s_abcd + pathPart.reflected();
    _sat.addClause(c6);
    _sat.addClause(c7);
}

void SATInstanceGenerator::addNoInteriorObstacleSomeVerticesNotInTriangleClauses() {
    cout << "Adding clauses to enforce that the special \"non-edge\" vertices cannot lie inside a triangle of the graph." << endl;
    for(auto aa : realVertices())
        for(auto bb : realVerticesAfter(aa))
            for(auto cc : realVerticesAfter(bb))
                for(auto zz : nonEdgeVertices()) {
                    if (!adjacent(aa,bb) || !adjacent(aa,cc) || !adjacent(bb,cc))
                        continue;
                    //-abc V  azb V  bzc V  cza says "abc is counterclockwise, or (it's clockwise so) z is outsize of abc"
                    // abc V -azb V -bzc V -cza
                    auto abc = variableForTriangle(aa,bb,cc);
                    auto azb = variableForTriangle(aa,zz,bb);
                    auto bzc = variableForTriangle(bb,zz,cc);
                    auto cza = variableForTriangle(cc,zz,aa);
                    
                    Clause c({-abc,azb,bzc,cza});
                    _sat.addClause(c);
                    _sat.addClause(c.reflected());
                }
}

void SATInstanceGenerator::addSingleObstacleSomeVerticesNotInTriangleClauses() {
    cout << "Adding clauses to enforce no non-adjacent vertices both inside and outside a triangle." << endl;
    for (auto aa : realVertices())
        for(auto bb : realVerticesAfter(aa))
            for(auto cc : realVerticesAfter(bb)) {
                if (!adjacent(aa, bb) || !adjacent(aa, cc) || !adjacent(bb, cc))
                    continue;
                for(auto xx : allVertices()) {
                    if (xx==aa || xx==bb || xx==cc)
                        continue;
                    if (adjacent(xx, aa) && adjacent(xx, bb) && adjacent(xx, cc))
                        continue;
                    for (auto yy : allVertices()) {
                        if (yy==xx || yy==aa || yy==bb || yy==cc)
                            continue;
                        auto abc = variableForTriangle(aa,bb,cc);
                        auto abx = variableForTriangle(aa,bb,xx);
                        auto bcx = variableForTriangle(bb,cc,xx);
                        auto cax = variableForTriangle(cc,aa,xx);
                        auto aby = variableForTriangle(aa,bb,yy);
                        auto bcy = variableForTriangle(bb,cc,yy);
                        auto cay = variableForTriangle(cc,aa,yy);

                        Clause xInabc{-abc,-abx,-bcx,-cax};
                        auto abClause = xInabc + aby;
                        auto bcClause = xInabc + bcy;
                        auto caClause = xInabc + cay;
                        if (!adjacent(yy,aa)) {
                            _sat.addClause(abClause);
                            _sat.addClause(abClause.reflected());
                            _sat.addClause(caClause);
                            _sat.addClause(caClause.reflected());
                        }
                        if (!adjacent(yy,bb)) {
                            _sat.addClause(abClause);
                            _sat.addClause(abClause.reflected());
                            _sat.addClause(bcClause);
                            _sat.addClause(bcClause.reflected());
                        }
                        if (!adjacent(yy,cc)) {
                            _sat.addClause(bcClause);
                            _sat.addClause(bcClause.reflected());
                            _sat.addClause(caClause);
                            _sat.addClause(caClause.reflected());
                        } //This adds duplicate clauses, but the SAT solver doesn't care. Do we?
                    }
                }
            }
}

void SATInstanceGenerator::writeCNF(const string &filename) {
    _sat.writeCNF(filename);
}

void SATInstanceGenerator::writeCNFWithComments(const string &filename) {
    _sat.writeCNFWithComments(filename);
}

void SATInstanceGenerator::show() const {
    std::cout << _sat.numClauses() << std::endl;
}

void SATInstanceGenerator::printSolutions() const {
    if (_solutions.size()>100)
        cout << "You don't really want to print over 100 solutions..." << endl;
    else
        for(const auto &sol:_solutions) {
            for(const auto &v:sol)
                cout << (v.second?'+':'-');
            cout << endl;
        }
}

void SATInstanceGenerator::analyzeSolutions() const {
    size_t bestConvexHullSize=0;
    SolutionAnalyzer bestSolution(_solutions[0],this);
    for(const auto& sol:_solutions) {
        SolutionAnalyzer analyzer(sol,this);
//        analyzer.printConvexHull();
//        analyzer.printInTriangles();
        if (analyzer.findConvexHull().size() > bestConvexHullSize) {
            bestSolution = analyzer;
            bestConvexHullSize = analyzer.findConvexHull().size();
        }
    }
    cout << "The solution with the largest convex hull: " << endl;
    bestSolution.printConvexHull();
    bestSolution.printInTriangles();
}

bool SATInstanceGenerator::solve(size_t maxSolutions) {//default maxSolutions=100
    _solutions = _sat.getSolutions(maxSolutions);
    return !_solutions.empty();
}
