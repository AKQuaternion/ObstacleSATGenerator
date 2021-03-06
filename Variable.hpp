//
//  Variable.hpp
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/7/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef Variable_hpp
#define Variable_hpp
#include <string>

class Variable {
    friend Variable operator-(Variable rhs);
public:
    explicit Variable(std::string name);
    bool negated() const;
    const std::string & name() const;
    bool sign() const;
    std::string toString() const;
private:
    bool _sign = true;
    std::string _name;
};

bool operator<(const Variable &lhs, const Variable &rhs);
#endif /* Variable_hpp */
