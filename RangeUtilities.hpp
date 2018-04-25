//
//  RangeUtilities.h
//  ObstacleSATGenerator
//
//  Created by Chris Hartman on 4/10/17.
//  Copyright © 2017 Chris Hartman. All rights reserved.
//

#ifndef RangeUtilities_h
#define RangeUtilities_h

template<typename Iter>
class Range {
public:
    Range(Iter iter_begin, Iter iter_end)
    : _begin(iter_begin), _end(iter_end)
    { }
    
    Iter begin() const { return _begin; }
    Iter end()   const { return _end; }
    
private:
    Iter _begin;
    Iter _end;
};

template <class Iter>
Range<Iter> makeRange(Iter begin, Iter end) {
    return Range<Iter>(begin, end);
}

template <typename T>
auto interior(T &t) -> Range<decltype(t.begin())> {
    auto newBegin {t.begin()};
    ++newBegin;
    auto newEnd {t.end()};
    --newEnd;
    return {newBegin,newEnd};
}

template <typename T>
class LoopRangeIterator {
public:
    LoopRangeIterator(T value_)
    : value(value_){}
    
    bool operator!=(LoopRangeIterator const& other) const {
        return value != other.value;
    }
    
    T const& operator*() const {
        return value;
    }
    
    LoopRangeIterator& operator++() {
        ++value;
        return *this;
    }
    
private:
    T value;
};

template <typename T>
class LoopRange
{
public:
    LoopRange(T from, T to)
    : _from(from), _to(to){}
    
    LoopRange(T to)
    : _from(0), _to(to){}
    
    LoopRangeIterator<T> begin() const {
        return {_from};
    }
    
    LoopRangeIterator<T> end() const {
        return {_to};
    }
    
private:
    T const _from;
    T const _to;
};

template <typename T>
LoopRange<T> makeLoopRange(T beg, T end){
    return LoopRange<T>(beg,end);
}

template <typename T>
LoopRange<T> makeLoopRange(T end){
    return LoopRange<T>(0,end);
}
#endif /* RangeUtilities_h */
