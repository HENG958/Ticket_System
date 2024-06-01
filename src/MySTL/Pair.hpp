#ifndef PAIR_HPP
#define PAIR_HPP

namespace trainsys {

template <class FirstType, class SecondType>
struct Pair 
{
    FirstType first;
    SecondType second;

    Pair() {}
    Pair(const FirstType &first, const SecondType &second): first(first), second(second) {}
};

template <class FirstType, class SecondType>
bool Check1(const Pair<FirstType, SecondType> &lhs, const Pair<FirstType, SecondType> &rhs) 
{
    if (lhs.first != rhs.first) {
        return lhs.first < rhs.first;
    } else {
        return lhs.second < rhs.second;
    }
}

template <class FirstType, class SecondType>
bool Check2(const Pair<FirstType, SecondType> &lhs, const Pair<FirstType, SecondType> &rhs) 
{
    return lhs.first == rhs.first && lhs.second == rhs.second;
}

}

#endif
