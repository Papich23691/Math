#ifndef MATH_INF_SET
#define MATH_INF_SET

#include <iostream>


using namespace std;

namespace math
{
#define PRINT_SIZE 10
template <class T>
class InfiniteSet
{
private:
  /* Function used to determain the elements of the set */
  function<bool(T)> elements;
  /* How accurate function sub_of */
  int accuracy;
  /* Starting element to start iterating from */
  T start;
  /* If the set is infinite to the before the starting element {...,start} */
  bool InfiniteStart;
  /* If the set is infinite to the after the starting element {start,...} */
  bool InfiniteEnd;
  /* Set of elements added to infinite set */
  Set<T> set;
  /* Set of elements removed from infinite set */
  Set<T> rem;

public:
  InfiniteSet(function<bool(T)> func, int ac, T st, bool istart = false,
              bool iend = true)
      : elements(func), accuracy(ac), start(st), InfiniteStart(istart),
        InfiniteEnd(iend) {}

  InfiniteSet(function<bool(T)> func, int ac, T st, Set<T> s,
              bool istart = false, bool iend = true)
      : elements(func), accuracy(ac), start(st), set(s), InfiniteStart(istart),
        InfiniteEnd(iend) {}

  InfiniteSet(function<bool(T)> func, int ac, T st, Set<T> s, Set<T> r,
              bool istart = false, bool iend = true)
      : elements(func), accuracy(ac), start(st), set(s), rem(s),
        InfiniteStart(istart), InfiniteEnd(iend) {}

  ~InfiniteSet() {}

  /* Set the function of elements */
  void set_elements(function<bool(T)> e) { this->elements = e; }

  /* Remove a given element */
  void remove(T element);

  /* Add a given element */
  void add(T element);

  /* If finite set s ⊆ this */
  bool is_sub(Set<T> &s);

  /* If element ∈ this */
  bool is_element(T element);

  /* If this ⊆ s(infinite) */
  bool sub_of(InfiniteSet<T> &s);

  /* If this = other */
  bool operator==(InfiniteSet<T> &other);
  bool operator!=(InfiniteSet<T> &other) { return !(*this == other); }

  /* Compares accuracy */
  bool operator<(const InfiniteSet<T> &other) const;
  bool operator>(const InfiniteSet<T> &other) const;

  /* Returns this ⋃ s(infinte) */
  InfiniteSet<T> unite(InfiniteSet<T> &s);

  /* Returns this ⋂ s(infinte) */
  InfiniteSet<T> intersection(InfiniteSet<T> &s);

  /* Returns this ⋃ s(finite) */
  InfiniteSet<T> unite(Set<T> &s);

  /* Returns this ⋂ s(finite) */
  Set<T> intersection(Set<T> &s);

  /* Return P(this) */
  InfiniteSet<InfiniteSet<T>> power_set();

  /* Print */
  friend ostream &operator<<(std::ostream &strm, InfiniteSet<int> s);
};

/* Remove a given element */
template <class T>
void InfiniteSet<T>::remove(T element)
{
  if (this->elements(element))
    this->rem.add(element);
  else if (this->set.is_element(element))
    this->set.remove(element);
}

/* Add a given element */
template <class T>
void InfiniteSet<T>::add(T element)
{
  if (!this->elements(element))
    this->set.add(element);
  else if (this->rem.is_element(element))
    this->rem.remove(element);
}

/* If finite set s ⊆ this */
template <class T>
bool InfiniteSet<T>::is_sub(Set<T> &s)
{
  for (int i = 0; i < s.get_cardinality(); i++)
  {
    if (!this->is_element(s[i]))
      return false;
  }
  return true;
}

/* If element ∈ this */
template <class T>
bool InfiniteSet<T>::is_element(T element)
{
  return ((this->elements(element) && !this->rem.is_element(element)) ||
          this->set.is_element(element));
}

/* If this ⊆ s(infinite) */
template <>
bool InfiniteSet<int>::sub_of(InfiniteSet<int> &s)
{

  int num;
  /* If added numbers set is not subset then returns false */
  if (!s.is_sub(this->set))
    return false;
  /* Checks if all numbers after start are elements of s*/
  if (InfiniteEnd)
  {
    num = this->start;
    /* 
    *  i - number of elements until i = accracy
    *  j - number of iterations until j = accracy ^ 2
    *  num - Starts at start and advances until for loop is finished
    */
    for (int i = 0, j = 0; i <= this->accuracy; num++, j++)
    {
      if (this->is_element(num))
      {
        i++;
        if (!s.is_element(num))
          return false;
      }
      else if (j >= this->accuracy * this->accuracy)
        throw out_of_range("InfiniteSet<int> : index is out of range");
    }
  }
  /* Checks if all numbers before start are elements of s*/
  if (InfiniteStart)
  {
    num = this->start;
    /* 
    *  i - number of elements until i = accracy
    *  j - number of iterations until j = accracy ^ 2
    *  num - Starts at start and advances until for loop is finished
    */
    for (int i = 0, j = 0; i <= this->accuracy; num--, j++)
    {
      if (this->is_element(num))
      {
        i++;
        if (!s.is_element(num))
          return false;
      }
      else if (j >= this->accuracy * this->accuracy)
        throw out_of_range("InfiniteSet<int> : index is out of range");
    }
  }
  return true;
}

/* Returns if this = other */
template <class T>
bool InfiniteSet<T>::operator==(InfiniteSet<T> &s)
{
  return this->sub_of(s) && s.sub_of(*this);
}

/* Compares accuracy */
template <class T>
bool InfiniteSet<T>::operator<(const InfiniteSet<T> &other) const
{
    return this -> accuracy < other.accuracy;
}

template <class T>
bool InfiniteSet<T>::operator>(const InfiniteSet<T> &other) const
{
    return this -> accuracy > other.accuracy;
}


/* Returns this ⋃ s(infinte) */
template <class T>
InfiniteSet<T> InfiniteSet<T>::unite(InfiniteSet<T> &s)
{
  /* x ∈ A ⋃ B = x ∈ A ⋁ x ∈ B*/
  InfiniteSet<T> temp(
      [&s, this](T x) { return this->is_element(x) || s.is_element(x); },
      this->accuracy, this->start, (this->InfiniteStart || s.InfiniteStart),
      (this->InfiniteEnd || s.InfiniteEnd));
  return temp;
}

/* Returns this ⋂ s(infinte) */
template <class T>
InfiniteSet<T> InfiniteSet<T>::intersection(InfiniteSet<T> &s)
{
  /* x ∈ A ⋂ B = x ∈ A ⋀ x ∈ B*/
  InfiniteSet<T> temp(
      [&s, this](T x) { return this->is_element(x) && s.is_element(x); },
      this->accuracy, this->start, (this->InfiniteStart && s.InfiniteStart),
      (this->InfiniteEnd && s.InfiniteEnd));
  return temp;
}

/* Returns this ⋃ s(finite) */
template <class T>
InfiniteSet<T> InfiniteSet<T>::unite(Set<T> &s)
{
  InfiniteSet<T> temp = *this;
  for (int i = 0; i < s.get_cardinality(); i++)
  {
    temp.add(s[i]);
  }
  return temp;
}

/* Returns this ⋂ s(finite) */
template <class T>
Set<T> InfiniteSet<T>::intersection(Set<T> &s)
{
  Set<T> temp;
  for (int i = 0; i < s.get_cardinality(); i++)
  {
    if (this->is_element(s[i]))
      temp.add(s[i]);
  }
  return temp;
}

/* Return P(this) */
template <class T>
InfiniteSet<InfiniteSet<T>> InfiniteSet<T>::power_set()
{
  InfiniteSet<InfiniteSet<T>> power(
      [this](InfiniteSet<T> s) { return s.sub_of(*this); }, this->accuracy,
      *this);
  return power;
}

ostream &operator<<(std::ostream &strm, InfiniteSet<int> s)
{
  /* 
    *  i - number of elements until i = PRINT_SIZE
    *  j - number of iterations until j = PRINT_SIZE ^ 2
    *  num - Starts at start and advances until for loop is finished
    */
  int num = s.start;
  strm << "{ ";
  if (s.InfiniteStart)
  {
    strm << "... ";
    for (int i = 0, j = 0; i < PRINT_SIZE; num--, j++)
    {
      if (s.is_element(num))
        i++;
      else if (j >= PRINT_SIZE * PRINT_SIZE)
        throw out_of_range("InfiniteSet<int> : index is out of range");
    }
    /* set = {...,start,...} */
    if (s.InfiniteEnd)
    {
      for (int i = 0, j = 0; i <= PRINT_SIZE * 2; num++, j++)
      {
        if (s.is_element(num))
        {
          strm << num << " , ";
          i++;
        }
        else if (j >= PRINT_SIZE * PRINT_SIZE * 2)
          throw out_of_range("InfiniteSet<int> : index is out of range");
      }
      strm << "...";
    }
    /* set = {...,start} */
    else
    {
      for (int i = 0, j = 0; i <= PRINT_SIZE; num++, j++)
      {
        if (s.is_element(num))
        {
          strm << num << " , ";
          i++;
        }
        else if (j >= PRINT_SIZE * PRINT_SIZE)
          throw out_of_range("InfiniteSet<int> : index is out of range");
      }
    }
  }
  /* set = {start,...} */
  else if (s.InfiniteEnd)
  {
    for (int i = 0, j = 0; i <= PRINT_SIZE; num++, j++)
    {
      if (s.is_element(num))
      {
        strm << num << " , ";
        i++;
      }
      else if (j >= PRINT_SIZE * PRINT_SIZE)
        throw out_of_range("InfiniteSet<int> : index is out of range");
    }
    strm << "...";
  }
  return strm << "}";
}
} // namespace math

#endif