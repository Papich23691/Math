#ifndef MATH_BIN_OP
#define MATH_BIN_OP


#include <iostream>


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
    /* Returns the result of the operation */
    T operator()(T &x, T &y);
    /* If the operation is closed on set */
    bool closure(Set<T> &set);

    /* If the operation is associative on set */
    bool associative(Set<T> &set);

    /* If the operation is commtative on set */
    bool commutative(Set<T> &set);
    /* Returns identity element of the operation in set */
    typename Set<T>::iterator identity_element(Set<T> &set);

    /* Returns right or left inverse element of the operation in set for a given element */
    typename Set<T>::iterator left_inverse_element(Set<T> &set, T x);
    typename Set<T>::iterator right_inverse_element(Set<T> &set, T x);

    /* If the operation is right or left distributive on set */
    bool right_distributive(Set<T> &set, BinaryOperation<T> &bin);
    bool left_distributive(Set<T> &set, BinaryOperation<T> &bin);

    /* Calculactes a set of ordered pairs */
    Set<T> calc_set(Set<pair<T, T>> &set);
};

/* Returns the result of the operation */
template <class T>
T BinaryOperation<T>::operator()(T &x, T &y)
{
    return this->op(x, y);
}

/* 
*  If the operation is closed on set 
*  ∀x,y ∈ set ((x * y) ∈ set ) 
*/
template <class T>
bool BinaryOperation<T>::closure(Set<T> &set)
{
    for (int i = 0; i < set.get_cardinality(); i++)
    {
        for (int j = 0; j < set.get_cardinality(); j++)
        {
            if (!set.is_element((*this)(set[i], set[j])))
                return false;
        }
    }
    return true;
}

/*
*  If the operation is associative on set 
*  operation is closed ⋀ ∀x,y,z ∈ set ( (x * (y * z)) = ((x * y) * z) )
*/
template <class T>
bool BinaryOperation<T>::associative(Set<T> &set)
{
    if (!this->closure(set))
        return false;
    for (int i = 0; i < set.get_cardinality(); i++)
    {
        for (int j = 0; j < set.get_cardinality(); j++)
        {
            for (int n = 0; n < set.get_cardinality(); n++)
            {
                if ((*this)((*this)(set[i], set[j]), set[n]) !=
                    (*this)(set[i], (*this)(set[j], set[n])))
                    return false;
            }
        }
    }
    return true;
}

/*
*  If the operation is commtative on set 
*  ∀x,y ∈ set ((x * y) = (y * x))
*/
template <class T>
bool BinaryOperation<T>::commutative(Set<T> &set)
{
    for (int i = 0; i < set.get_cardinality(); i++)
    {
        for (int j = 0; j < set.get_cardinality(); j++)
        {
            if ((*this)(set[i], set[j]) != (*this)(set[j], set[i]))
                return false;
        }
    }
    return true;
}

/* 
*  Returns identity element of the operation in set (element e)
*  ∃e ∈ set (∀x ∈ set  (e * x) = x ⋀ (x * e) = x)
*/
template <class T>
typename Set<T>::iterator BinaryOperation<T>::identity_element(Set<T> &set)
{

    for (typename Set<T>::iterator i = set.begin(); i != set.end(); i++)
    {
        bool identity = true;
        for (typename Set<T>::iterator j = set.begin(); j != set.end(); j++)
        {
            
            if (((*this)(*i, *j) != *j) ||
                (*this)(*i, *j) != (*this)(*j, *i))
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

/* 
*  Returns right or left inverse element of the operation in set for a given element (element i)
*  Right : ∃i ∈ set ((i * x) = e)
*  Left : ∃i ∈ set ((x * i) = e)
*/
template <class T>
typename Set<T>::iterator BinaryOperation<T>::left_inverse_element(Set<T> &set,
                                                                 T x)
{
    if (this->identity_element(set) == set.end())
        return set.end();
    T identity=*this->identity_element(set);
    for (typename Set<T>::iterator i = set.begin(); i != set.end(); i++)
    {
        if ((*this)(*i, x) == identity)
            return i;
    }
    return set.end();
}

template <class T>
typename Set<T>::iterator BinaryOperation<T>::right_inverse_element(Set<T> &set,
                                                                  T x)
{
    
    if (this->identity_element(set) == set.end())
        return set.end();
    T identity=*this->identity_element(set);
    for (typename Set<T>::iterator i = set.begin();
         i != set.end(); i++)
    {
        if ((*this)(x, *i) == identity)
            return i;
    }
    return set.end();
}

/* 
*  If the operation is right or left distributive on set with given operation(∆)
*  Left : ∀x,y,z ∈ set (x * (y ∆ z) = (x * y) ∆ (x * z))
*  Right : ∀x,y,z ∈ set ((y ∆ z) * x = (y * x) ∆ (z * x))
*/
template <class T>
bool BinaryOperation<T>::left_distributive(Set<T> &set,
                                           BinaryOperation<T> &bin)
{
    for (int i = 0; i < set.get_cardinality(); i++)
    {
        for (int j = 0; j < set.get_cardinality(); j++)
        {
            for (int n = 0; n < set.get_cardinality(); n++)
            {
                if ((*this)(set[i], bin.operation(set[j], set[n])) !=
                    bin.operation((*this)(set[i], set[j]),
                                  (*this)(set[i], set[n])))
                    return false;
            }
        }
    }
    return true;
}

template <class T>
bool BinaryOperation<T>::right_distributive(Set<T> &set,
                                          BinaryOperation<T> &bin)
{
    for (int i = 0; i < set.get_cardinality(); i++)
    {
        for (int j = 0; j < set.get_cardinality(); j++)
        {
            for (int n = 0; n < set.get_cardinality(); n++)
            {
                if ((*this)(bin.operation(set[j], set[n]), set[i]) !=
                    bin.operation((*this)(set[j], set[i]),
                                  (*this)(set[n], set[i])))
                    return false;
            }
        }
    }
    return true;
}

/* Returns a set with calculated values of each ordered pairs in set */
template <class T>
Set<T> BinaryOperation<T>::calc_set(Set<pair<T, T>> &set)
{
    Set<T> temp;
    for (int i = 0; i < set.get_cardinality(); i++)
    {
        temp.add((*this)(set[i].first, set[i].second));
    }
    return temp;
}
} // namespace math

#endif