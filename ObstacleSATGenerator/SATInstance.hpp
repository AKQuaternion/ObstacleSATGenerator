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
#include <cstddef>

#include "Variable.hpp"
#include "Clause.hpp"

class SATInstance {
public:
    void addClause(const Clause &c);
    void writeCNF(const std::string &) const;
    void writeCNFWithComments(const std::string &) const;
    size_t numVariables() const;
    size_t numClauses() const;
    std::vector<std::map<Variable,bool>> satisfiable() const;
    bool important(uint32_t n) const;
private:
    void writeCNFHelper(const std::string &, bool) const;
    long numberFromVariable(const Variable &v) const;
    std::vector<Clause> _clauses;
    std::map<std::string,size_t> _variableNumbers;
    std::vector<std::string> _variableNames = {"NoVariableZero"};
};
#endif /* SATInstance_hpp */
