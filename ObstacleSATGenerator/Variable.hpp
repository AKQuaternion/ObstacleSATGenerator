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
    friend Variable operator-(const Variable rhs);
public:
    Variable(std::string name);
private:
    bool _sign = true;
    std::string _name;
};



#endif /* Variable_hpp */
