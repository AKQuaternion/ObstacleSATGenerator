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
public:
//    template<typename... Args> Clause(Args&&... args): _variables(std::forward<Args>(args)...) {}
    Clause() = default;
    Clause(std::initializer_list<Variable> v) :_variables(v) {}
    Clause & add(Variable v);
    Clause reflected() const;
    auto begin() {return _variables.begin();}
    auto begin() const  {return _variables.begin();}
    auto end()  {return _variables.end();}
    auto end() const  {return _variables.end();}
private:
    std::vector<Variable> _variables;
};
Clause & operator+=(Clause &lhs, Variable v);
Clause operator+(Clause lhs, Variable v);
#endif /* Clause_hpp */
