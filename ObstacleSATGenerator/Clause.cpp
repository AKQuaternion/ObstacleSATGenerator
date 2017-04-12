//
//  Clause.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/10/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "Clause.hpp"
#include <utility>
using std::move;

Clause & Clause::add(Variable v) {
    _variables.emplace_back(std::move(v));
    return *this;
}

Clause & operator+=(Clause &lhs, const Clause &rhs){
    lhs._variables.insert(lhs.end(),rhs.begin(),rhs.end());
    return lhs;
}

Clause operator+(Clause lhs, const Clause &rhs){
    return lhs+=rhs;
}

Clause Clause::reflected() const {
    Clause ref(*this);
    for(auto &v:ref._variables)
        v = -v;
    return ref;
}
