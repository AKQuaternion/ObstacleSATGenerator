//
//  SolutionAnalyzer.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/27/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "Variable.hpp"
#include "SATInstanceGenerator.hpp"
#include "SolutionAnalyzer.hpp"
#include <iostream>
#include <stdexcept>
#include <string>

using std::map;
using std::vector;
using std::cout;
using std::endl;

SolutionAnalyzer::SolutionAnalyzer(const std::map<Variable,bool> &solution, const SATInstanceGenerator *sig) :_solution(solution), _sat(sig){
//    for(const auto &var : _solution) {
//        cout << (var.second?'+':'-') << var.first.name() << endl;
//    }
}

void SolutionAnalyzer::printConvexHull() const {
    cout << "Convex hull: ";
    for(auto v:findConvexHull())
        cout << v << " ";
    cout << endl;
}

void SolutionAnalyzer::printInTriangles() const {
    for(auto x:_sat->realVertices())
        for (auto a:_sat->realVertices())
            for (auto b:_sat->realVerticesAfter(a))
                for(auto c:_sat->realVerticesAfter(b)) {
                    if(x==a || x==b || x==c)
                        continue;
                    if(inTriangle(a, b, c, x))
                        cout << x << " is in " << _sat->variableForTriangle(a, b, c).name() << endl;
                }
}

bool SolutionAnalyzer::clockWise(Vertex a, Vertex b, Vertex c) const
{
    auto v = _sat->variableForTriangle(a, b, c);
    auto i = _solution.find(v);
    if (i==_solution.end())
        throw std::runtime_error("SolutionAnalyzer::clockWise couldn't find variable");
    return i->second == v.sign();
}

bool SolutionAnalyzer::inConvexHull(Vertex aa, Vertex bb) const
{
    for(auto cc:_sat->realVertices())
    {
        if(cc==aa||cc==bb) continue;
        if(!clockWise(aa,bb,cc))
            return false;
    }
    return true;
}

bool SolutionAnalyzer::inTriangle(Vertex aa, Vertex bb, Vertex cc, Vertex xx) const
{
    if(clockWise(aa,bb,cc))
        return clockWise(aa,bb,xx)&&clockWise(bb,cc,xx)&&clockWise(cc,aa,xx);
    
    return !clockWise(aa,bb,xx)&&!clockWise(bb,cc,xx)&&!clockWise(cc,aa,xx);
}

vector<Vertex> SolutionAnalyzer::findFirstTwoInConvexHull() const
{
    for(auto aa:_sat->realVertices())
        for(auto bb:_sat->realVertices()) {
            if (aa==bb) continue;
            if(inConvexHull(aa,bb))
                return {aa,bb};
        }
    throw std::runtime_error ("findFirstTwoInConvexHull() couldn't find any!");
}

Vertex SolutionAnalyzer::nextInConvexHull(Vertex prev) const
{
    for(auto vv:_sat->realVertices()) {
        if(vv == prev)
            continue;
        if (inConvexHull(prev,vv))
            return vv;
    }
    throw std::runtime_error ("nextInConvexHull() couldn't find next vertex!");
}

vector<Vertex> SolutionAnalyzer::findConvexHull() const
{
    vector<Vertex> convHull = findFirstTwoInConvexHull();
    
    auto next = nextInConvexHull(convHull.back());
    while (next != convHull.front()) {
        convHull.push_back(next);
        next = nextInConvexHull(next);
    }
    
    return convHull;
}
