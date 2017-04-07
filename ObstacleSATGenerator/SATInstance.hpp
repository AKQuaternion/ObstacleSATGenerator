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

class SATInstance_ {
public:
    void addClause();
    void writeCNF(const std::string) const;
private:
    std::vector<Clause> _clauses;
    std::map<std::string,size_t> _variableNumbers;
};
#endif /* SATInstance_hpp */
