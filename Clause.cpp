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

Clause & operator+=(Clause &lhs, const Clause &rhs){
    lhs._variables.insert(lhs.end(),rhs.begin(),rhs.end());
    return lhs;
}

Clause & operator+=(Clause &lhs, const Variable &rhs){
    return lhs += Clause{rhs};
}

Clause operator+(Clause lhs, const Clause &rhs){
    return lhs+=rhs;
}

Clause operator+(Clause lhs, const Variable &rhs){
    return lhs+=rhs;
}

Clause operator+(const Variable &lhs, const Variable &rhs){
    auto c = Clause{lhs};
    return c+=rhs;
}

Clause Clause::reflected() const {
    Clause ref(*this);
    for(auto &v:ref._variables)
        v = -v;
    return ref;
}
