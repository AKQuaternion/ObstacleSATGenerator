//
//  Clause.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/10/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef Clause_hpp
#define Clause_hpp
#include "Variable.hpp"
#include <vector>

class Clause {
    friend Clause & operator+=(Clause &lhs, const Clause &rhs);
public:
//    template<typename... Args> Clause(Args&&... args): _variables(std::forward<Args>(args)...) {}
    Clause() = default;
    Clause(std::initializer_list<Variable> v) :_variables(v) {}
    Clause(Variable v):_variables{std::move(v)} {}
    Clause reflected() const;
    std::vector<Variable>::iterator begin() {return _variables.begin();}
    std::vector<Variable>::const_iterator begin() const  {return _variables.begin();}
    std::vector<Variable>::iterator end()  {return _variables.end();}
    std::vector<Variable>::const_iterator end() const  {return _variables.end();}
private:
    std::vector<Variable> _variables;
public:
};

Clause operator+(Clause lhs, const Clause &rhs);

#endif /* Clause_hpp */
