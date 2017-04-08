//
//  SATInstance.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef SATInstance_hpp
#define SATInstance_hpp

#include <string>
#include <vector>
#include <map>
#include <stddef.h>

#include "Variable.hpp"

using Clause = std::vector<Variable>;

class SATInstance {
public:
    void addClause(const Clause &c);
    void writeCNF(const std::string &) const;
    size_t numVariables() const;
    size_t numClauses() const;
private:
    size_t numberFromVariable(const Variable &v) const;
    std::vector<Clause> _clauses;
    std::map<std::string,size_t> _variableNumbers;
    size_t _nextVariableNumber=1;
};
#endif /* SATInstance_hpp */
