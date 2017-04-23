//
//  SATInstance.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "SATInstance.hpp"
using std::string;
using std::vector;
#include <fstream>
#include <iostream>
using std::cout;
using std::endl;
#include <stdexcept>
using std::runtime_error;
#include <cassert>
#include <cryptominisat5/cryptominisat.h>
using namespace CMSat;

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

bool SATInstance::satisfiable() const {
    SATSolver solver;
    
    solver.new_vars(numVariables());
    
    for (const auto & c : _clauses) {
        vector<Lit> clause;
        for (const auto & v : c) {
            auto n = numberFromVariable(v);
            if (n>0)
                clause.push_back(Lit( uint32_t(n)-1, false));
            else
                clause.push_back(Lit(-uint32_t(n)-1, true));
        }
        solver.add_clause(clause);
    }
    
    if(solver.solve() == l_True)
        cout << "Satisfiable." << endl;
    else
        cout << "UNSatisfiable." << endl;
    
    int count = 0;
    while(true) {
        lbool ret = solver.solve();
        if (ret != l_True) {
            assert(ret == l_False);
            cout << count << " solutions found." << endl;
            return false;
        }
        ++count;
        //Use solution here. print it, for example.
        
        vector<Lit> ban_solution;
        for (uint32_t var = 0; var < solver.nVars(); var++) {
            if (solver.get_model()[var] != l_Undef  && important(var)) {
                cout << ((solver.get_model()[var] == l_True)? "+" : "-") /*<< _variableNames[var+1] << " "*/;
                ban_solution.push_back(Lit(var, (solver.get_model()[var] == l_True)? true : false));
            }
        }
        cout << endl;
        solver.add_clause(ban_solution);
    }
    return (solver.okay());
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

long SATInstance::numberFromVariable(const Variable &v) const {
    auto i = _variableNumbers.find(v.name());
    if (i == _variableNumbers.end())
        throw runtime_error("SATInstance::numberFromVariable called with variable who's name isn't in _variableNumbers");
    if(v.negated())
        return -i->second;
    else
        return i->second;
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
