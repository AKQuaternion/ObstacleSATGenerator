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
using std::endl;
#include <stdexcept>
using std::runtime_error;

void SATInstance::addClause(const Clause &c) {
    for(const auto &v:c) {
        auto i = _variableNumbers.find(v.name());
        if (i == _variableNumbers.end())
            _variableNumbers.insert(i,std::make_pair(v.name(),_nextVariableNumber++));
    }
    _clauses.push_back(c);
}

size_t SATInstance::numberFromVariable(const Variable &v) const {
    auto i = _variableNumbers.find(v.name());
    if (i == _variableNumbers.end())
        throw runtime_error("SATInstance::numberFromVariable called with variable who's name isn't in _variableNumbers");
    if(v.negated())
        return -i->second;
    else
        return i->second;
}

void SATInstance::writeCNF(const string &filename) const{
    std::ofstream of(filename+".cnf");
    
    of << "p cnf " << _variableNumbers.size() <<" "<< _clauses.size() << endl;
    for (const auto &c : _clauses) {
        for (const auto &v : c)
            of << numberFromVariable(v) << " ";
        of << "0" << endl;
        }
    std::cout << "Wrote " << filename << ".cnf with " << _variableNumbers.size() <<" variables and "<< _clauses.size() << " clauses\n";
}
