//
//  SATInstance.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "SATInstance.hpp"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wall"
#pragma clang diagnostic ignored "-Weverything"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#include <cryptominisat5/cryptominisat.h>
#pragma clang diagnostic pop
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using std::string;
using std::map;
using std::cout;
using std::endl;
using std::runtime_error;
using std::vector;
using CMSat::SATSolver;
using CMSat::Lit;
using CMSat::lbool;

static bool isTriangleVariable(const string &name) {
    return name[0]=='T';
}

static bool doesntContainNonEdgeVertex(const string &name) {
    return name.find('<')==string::npos;
}

bool SATInstance::important(uint32_t n) const {
    auto name = _variableNames[n+1];
    return isTriangleVariable(name) && doesntContainNonEdgeVertex(name);
}

vector<map<string,bool>> SATInstance::getSolutions(size_t maxSolutions) const {
    SATSolver solver;
    
    solver.new_vars(numVariables());
    
    for (const auto & c : _clauses) {
        vector<Lit> clause;
        for (const auto & v : c) {
            auto n = numberFromVariable(v);
            if (n>0)
                clause.emplace_back( n-1, false);
            else
                clause.emplace_back(-n-1, true);
        }
        solver.add_clause(clause);
    }
    
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
    if(solver.solve() == l_True)
        cout << "Satisfiable." << endl;
    else
        cout << "UNSatisfiable." << endl;
    
    vector<map<string,bool>> solutions;
    while(true) {
        lbool ret = solver.solve();
        if (ret != l_True) {
            assert(ret == l_False); //NOLINT
            cout << solutions.size() << " solutions found." << endl;
            return solutions;
        }
        map<string,bool> solution;
        vector<Lit> ban_solution;
        for (uint32_t var = 0; var < solver.nVars(); var++) {
            if (solver.get_model()[var] != l_Undef  && important(var)) {
                solution[_variableNames[var+1]] = (solver.get_model()[var] == l_True);
                ban_solution.emplace_back(var, (solver.get_model()[var] == l_True));
#pragma clang diagnostic pop
            }
        }
        solutions.push_back(solution);
        solver.add_clause(ban_solution);
        if (solutions.size()==maxSolutions){
            cout << "More than " << maxSolutions << " solutions, moving on..." << endl;
            return solutions;
        }
    }
}

void SATInstance::addClause(const Clause &c) {
    for(const auto &v:c) {
        auto i = _variableNumbers.find(v.name());
        if (i == _variableNumbers.end()){
            _variableNumbers.insert(i,std::make_pair(v.name(),_variableNames.size()));
            _variableNames.push_back(v.name());
        }
    }
    _clauses.push_back(c);
}

int SATInstance::numberFromVariable(const Variable &v) const {
    auto i = _variableNumbers.find(v.name());
    if (i == _variableNumbers.end())
        throw runtime_error("SATInstance::numberFromVariable called with variable who's name isn't in _variableNumbers");
    if(v.negated())
        return -int(i->second);
    
    return int(i->second);
}

size_t SATInstance::numVariables() const{
    return _variableNumbers.size();
}

size_t SATInstance::numClauses() const {
    return _clauses.size();
}

void SATInstance::writeCNF(const std::string &s) const {
    writeCNFHelper(s, false);
}

void SATInstance::writeCNFWithComments(const std::string &s) const {
    writeCNFHelper(s, true);
}

void SATInstance::writeCNFHelper(const string &filename, bool writeComments) const{
    std::ofstream of(filename+".cnf");
    
    of << "p cnf " << _variableNumbers.size() <<" "<< _clauses.size() << endl;
    for (const auto &c : _clauses) {
        for (const auto &v : c)
            of << numberFromVariable(v) << " ";
        of << "0" << endl;
        if (writeComments) {
            of << "c ";
            for (const auto &v : c)
                of << v.toString() << " ";
            of << endl;
        }
    }
    std::cout << "Wrote " << filename << ".cnf with " << _variableNumbers.size() <<" variables and "<< _clauses.size() << " clauses\n";
}
