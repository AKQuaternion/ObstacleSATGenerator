//
//  Variable.cpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#include "Variable.hpp"

Variable::Variable(std::string name) : _name(name)
{}

bool Variable::negated() const {
    return !_sign;
}

const std::string & Variable::name() const {
    return _name;
}

Variable operator-(Variable rhs) {
    rhs._sign = !rhs._sign;
    return rhs;
}
