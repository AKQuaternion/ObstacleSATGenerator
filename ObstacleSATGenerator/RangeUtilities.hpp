//
//  RangeUtilities.h
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/10/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef RangeUtilities_h
#define RangeUtilities_h

template <typename T>
class InteriorRange
{
public:
    InteriorRange(T &t):_t{t}
    {}
    auto begin() {
        auto newBegin {_t.begin()};
        --newBegin;
        return newBegin;
    }
    
    auto end() {
        auto newEnd {_t.end()};
        --newEnd;
        return newEnd;
    }
private:
    T &_t;
};

template <typename T>
auto interior(T &t) {
    return InteriorRange<T>(t);
}

#endif /* RangeUtilities_h */
