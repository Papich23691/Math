#ifndef L_A_FLD
#define L_A_FLD

#include <iostream>

using namespace std;

namespace math
{

template <class T>
class Field
{
private:
    BinaryOperation<T> add, mul;
    T zero, one;
    bool real;

public:
    Set<T> set;
    Field(Set<T> set, BinaryOperation<T> add, BinaryOperation<T> mul, T zero, T one);
    Field() : set(Set<T>()), add(NULL), mul(NULL), zero(0), one(0), real(true)
    {
        if (!is_same<T, double>::value && !is_same<T, float>::value)
            throw invalid_argument("Real numbers must be of type float or double");
    }
    ~Field() {}
    T field_add(T element1, T element2);
    T field_sub(T element1, T element2);
    T field_mul(T element1, T element2);
    T field_div(T element1, T element2);
    T get_zero()
    {
        if (!real)
            return zero;
        return 0;
    }
    T get_one()
    {
        if (!real)
            return one;
        return 1;
    }
    bool is_real() { return real; }
};

template <class T>
Field<T>::Field(Set<T> set, BinaryOperation<T> add, BinaryOperation<T> mul, T zero, T one)
{
    this->real = false;
    // Checks if the field is actually a field
    if (add.associative(set) && mul.associative(set) && add.commutative(set) && mul.commutative(set) && *(mul.identity_element(set)) == one && *(add.identity_element(set)) == zero && mul.left_distributive(set, add) && mul.right_distributive(set, add) && !add.left_distributive(set, mul) && !add.right_distributive(set, mul))
    {
        this->set = set;
        this->add = add;
        this->mul = mul;
        this->zero = zero;
        this->one = one;
    }
}
template <class T>
T Field<T>::field_add(T element1, T element2)
{
    if (!real)
    {
        if (set.is_element(element1) && set.is_element(element2))
            return this->add(element1, element2);
        else
            throw invalid_argument("Arguments not in field");
    }
    return element1 + element2;
}

template <class T>
T Field<T>::field_sub(T element1, T element2)
{
    if (!real)
    {
        if (set.is_element(element1) && set.is_element(element2))
            return this->add(element1, *(add.left_inverse_element(set, element2)));
        else
            throw std::invalid_argument("Arguments not in field");
    }
    return element1 - element2;
}

template <class T>
T Field<T>::field_mul(T element1, T element2)
{
    if (!real)
    {
        if (set.is_element(element1) && set.is_element(element2))
            return this->mul(element1, element2);
        else
            throw std::invalid_argument("Arguments not in field");
    }
    return element1 * element2;
}

template <class T>
T Field<T>::field_div(T element1, T element2)
{
    if (!real)
    {
        if (set.is_element(element1) && set.is_element(element2))
            return this->mul(element1, *(mul.left_inverse_element(set, element2)));
        else
            throw std::invalid_argument("Arguments not in field");
    }
    return element1 / element2;
}

Field<double> real = Field<double>();
} // namespace math
#endif
