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

void SATInstance::addClause(const Clause &c) {
    _clauses.push_back(c);
}

size_t SATInstance::numberFromVariable(const Variable &v) const {
    if(v.negated()
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
