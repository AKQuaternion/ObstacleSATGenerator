//
//  SATInstance.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef SATInstance_hpp
#define SATInstance_hpp

#include "Clause.hpp"
#include "Variable.hpp"
#include <cstddef>
#include <map>
#include <string>
#include <vector>

class SATInstance {
public:
    void addClause(const Clause &c);
    void writeCNF(const std::string & /*s*/) const;
    void writeCNFWithComments(const std::string & /*s*/) const;
    size_t numVariables() const;
    size_t numClauses() const;
    std::vector<std::map<Variable,bool>> getSolutions(size_t maxSolutions) const;
    bool important(uint32_t n) const;
private:
    void writeCNFHelper(const std::string & /*filename*/, bool /*writeComments*/) const;
    int numberFromVariable(const Variable &v) const;
    std::vector<Clause> _clauses;
    std::map<std::string,size_t> _variableNumbers;
    std::vector<std::string> _variableNames = {"NoVariableZero"};
};
#endif /* SATInstance_hpp */
