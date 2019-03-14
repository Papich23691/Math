#ifndef MATH_BIN_OP
#define MATH_BIN_OP

#include <initializer_list>
#include <iostream>
#include <list>
#include <utility>

#include "Set.hpp"

using namespace std;

namespace math
{
template <class T>
class BinaryOperation
{
  private:
    function<T(T, T)> op;

  public:
    BinaryOperation(function<T(T, T)> func) : op(func) {}
    ~BinaryOperation() {}
    T operation(T &x, T &y);
    bool Closure(Set<T> &set);
    bool Associative(Set<T> &set);
    bool Commutative(Set<T> &set);
    typename Set<T>::iterator Identityelement(Set<T> &set);
    typename Set<T>::iterator LeftInverseElement(Set<T> &set, T x);
    typename Set<T>::iterator RightInverseElement(Set<T> &set, T x);
    bool RightDistributive(Set<T> &set, BinaryOperation<T> &bin);
    bool LeftDistributive(Set<T> &set, BinaryOperation<T> &bin);
    Set<T> CalcSet(Set<pair<T, T>> &set);
};

template <class T>
T BinaryOperation<T>::operation(T &x, T &y)
{
    return this->op(x, y);
}

template <class T>
bool BinaryOperation<T>::Closure(Set<T> &set)
{
    for (int i = 0; i < set.Getcardinality(); i++)
    {
        for (int j = 0; j < set.Getcardinality(); j++)
        {
            if (!set.iselement(this->operation(set[i], set[j])))
                return false;
        }
    }
    return true;
}
template <class T>
bool BinaryOperation<T>::Associative(Set<T> &set)
{
    if (!this->Closure(set))
        return false;
    for (int i = 0; i < set.Getcardinality(); i++)
    {
        for (int j = 0; j < set.Getcardinality(); j++)
        {
            for (int n = 0; n < set.Getcardinality(); n++)
            {
                if (this->operation(this->operation(set[i], set[j]), set[n]) !=
                    this->operation(set[i], this->operation(set[j], set[n])))
                    return false;
            }
        }
    }
    return true;
}

template <class T>
bool BinaryOperation<T>::Commutative(Set<T> &set)
{
    for (int i = 0; i < set.Getcardinality(); i++)
    {
        for (int j = 0; j < set.Getcardinality(); j++)
        {
            if (this->operation(set[i], set[j]) != this->operation(set[j], set[i]))
                return false;
        }
    }
    return true;
}

template <class T>
typename Set<T>::iterator BinaryOperation<T>::Identityelement(Set<T> &set)
{

    for (typename Set<T>::iterator i = set.begin(); i != set.end(); i++)
    {
        bool identity = true;
        for (typename Set<T>::iterator j = set.begin(); j != set.end(); j++)
        {
            
            if ((this->operation(*i, *j) != *j) ||
                this->operation(*i, *j) != this->operation(*j, *i))
            {
                identity = false;
                break;
            }
        }
        if (identity)
            return i;
    }
    return set.end();
}

template <class T>
typename Set<T>::iterator BinaryOperation<T>::LeftInverseElement(Set<T> &set,
                                                                 T x)
{
    if (this->Identityelement(set) == set.end())
        return set.end();
    T identity=*this->Identityelement(set);
    for (typename Set<T>::iterator i = set.begin(); i != set.end(); i++)
    {
        if (this->operation(*i, x) == identity)
            return i;
    }
    return set.end();
}

template <class T>
typename Set<T>::iterator BinaryOperation<T>::RightInverseElement(Set<T> &set,
                                                                  T x)
{
    
    if (this->Identityelement(set) == set.end())
        return set.end();
    T identity=*this->Identityelement(set);
    for (typename Set<T>::iterator i = set.begin();
         i != set.end(); i++)
    {
        if (this->operation(x, *i) == identity)
            return i;
    }
    return set.end();
}

template <class T>
bool BinaryOperation<T>::RightDistributive(Set<T> &set,
                                           BinaryOperation<T> &bin)
{
    for (int i = 0; i < set.Getcardinality(); i++)
    {
        for (int j = 0; j < set.Getcardinality(); j++)
        {
            for (int n = 0; n < set.Getcardinality(); n++)
            {
                if (this->operation(set[i], bin.operation(set[j], set[n])) !=
                    bin.operation(this->operation(set[i], set[j]),
                                  this->operation(set[i], set[n])))
                    return false;
            }
        }
    }
    return true;
}

template <class T>
bool BinaryOperation<T>::LeftDistributive(Set<T> &set,
                                          BinaryOperation<T> &bin)
{
    for (int i = 0; i < set.Getcardinality(); i++)
    {
        for (int j = 0; j < set.Getcardinality(); j++)
        {
            for (int n = 0; n < set.Getcardinality(); n++)
            {
                if (this->operation(bin.operation(set[j], set[n]), set[i]) !=
                    bin.operation(this->operation(set[j], set[i]),
                                  this->operation(set[n], set[i])))
                    return false;
            }
        }
    }
    return true;
}

template <class T>
Set<T> BinaryOperation<T>::CalcSet(Set<pair<T, T>> &set)
{
    Set<T> temp;
    for (int i = 0; i < set.Getcardinality(); i++)
    {
        temp.add(this->operation(set[i].first, set[i].second));
    }
    return temp;
}
} // namespace math

#endif