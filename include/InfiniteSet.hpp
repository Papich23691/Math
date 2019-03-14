#ifndef MATH_INF_SET
#define MATH_INF_SET

#include <initializer_list>
#include <iostream>
#include <list>
#include <utility>

using namespace std;

namespace math
{
#define PRINT_SIZE 10
template <class T>
class InfiniteSet
{
private:
  function<bool(T)> elements;
  int accuracy;
  T start;
  bool InfiniteStart;
  bool InfiniteEnd;
  Set<T> set;
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
  void SetElements(function<bool(T)> e) { this->elements = e; }
  void remove(T element);
  void add(T element);
  bool issub(Set<T> &s);
  bool iselement(T element);
  bool subof(InfiniteSet<T> &s);
  bool operator==(InfiniteSet<T> &other);
  bool operator!=(InfiniteSet<T> &other) { return !(*this == other); }
  bool operator<(const InfiniteSet<T> &other) const;
  bool operator>(const InfiniteSet<T> &other) const;
  InfiniteSet<T> unite(InfiniteSet<T> &s);
  InfiniteSet<T> intersection(InfiniteSet<T> &s);
  InfiniteSet<T> unite(Set<T> &s);
  Set<T> intersection(Set<T> &s);
  InfiniteSet<InfiniteSet<T>> powerSet();
  friend ostream &operator<<(std::ostream &strm, InfiniteSet<int> s);
};
template <class T>
void InfiniteSet<T>::remove(T element)
{
  if (this->elements(element))
    this->rem.add(element);
  else if (this->set.iselement(element))
    this->set.remove(element);
}

template <class T>
void InfiniteSet<T>::add(T element)
{
  if (!this->elements(element))
    this->set.add(element);
  else if (this->rem.iselement(element))
    this->rem.remove(element);
}

template <class T>
bool InfiniteSet<T>::issub(Set<T> &s)
{
  for (int i = 0; i < s.Getcardinality(); i++)
  {
    if (!this->iselement(s[i]))
      return false;
  }
  return true;
}

template <class T>
bool InfiniteSet<T>::iselement(T element)
{
  return ((this->elements(element) && !this->rem.iselement(element)) ||
          this->set.iselement(element));
}

template <>
bool InfiniteSet<int>::subof(InfiniteSet<int> &s)
{
  int num;
  if (!s.issub(this->set))
    return false;
  if (InfiniteEnd)
  {
    num = this->start;
    for (int i = 0, j = 0; i <= this->accuracy; num++, j++)
    {
      if (this->iselement(num))
      {
        i++;
        if (!s.iselement(num))
          return false;
      }
        else if (j >= this->accuracy * this->accuracy)
          throw out_of_range("InfiniteSet<int> : index is out of range");
    }
  }
  if (InfiniteStart)
  {
    num = this->start;
    for (int i = 0, j = 0; i <= this->accuracy; num--, j++)
    {
      if (this->iselement(num))
      {
        i++;
        if (!s.iselement(num))
          return false;
      }
      else if (j >= this->accuracy * this->accuracy)
          throw out_of_range("InfiniteSet<int> : index is out of range");
    }
  }
  return true;
}

template <class T>
bool InfiniteSet<T>::operator==(InfiniteSet<T> &s)
{
  return this->subof(s) && s.subof(*this);
}

template <class T>
bool InfiniteSet<T>::operator<(const InfiniteSet<T> &other) const
{
  return this->accuracy < other.accuracy ;
}

template <class T>
bool InfiniteSet<T>::operator>(const InfiniteSet<T> &other) const
{
  return this->accuracy > other.accuracy ;
}

template <class T>
InfiniteSet<T> InfiniteSet<T>::unite(InfiniteSet<T> &s)
{
  InfiniteSet<T> temp(
      [&s, this](T x) { return this->iselement(x) || s.iselement(x); },
      this->accuracy, this->start, (this->InfiniteStart || s.InfiniteStart),
      (this->InfiniteEnd || s.InfiniteEnd));
  return temp;
}

template <class T>
InfiniteSet<T> InfiniteSet<T>::intersection(InfiniteSet<T> &s)
{
  InfiniteSet<T> temp(
      [&s, this](T x) { return this->iselement(x) && s.iselement(x); },
      this->accuracy, this->start, (this->InfiniteStart && s.InfiniteStart),
      (this->InfiniteEnd && s.InfiniteEnd));
  return temp;
}

template <class T>
InfiniteSet<T> InfiniteSet<T>::unite(Set<T> &s)
{
  InfiniteSet<T> temp = *this;
  for (int i = 0; i < s.Getcardinality(); i++)
  {
    temp.add(s[i]);
  }
  return temp;
}

template <class T>
Set<T> InfiniteSet<T>::intersection(Set<T> &s)
{
  Set<T> temp;
  for (int i = 0; i < s.Getcardinality(); i++)
  {
    if (this->iselement(s[i]))
      temp.add(s[i]);
  }
  return temp;
}

template <class T>
InfiniteSet<InfiniteSet<T>> InfiniteSet<T>::powerSet()
{
  InfiniteSet<InfiniteSet<T>> power(
      [this](InfiniteSet<T> s) { return s.subof(*this); }, this->accuracy,
      *this);
  return power;
}

ostream &operator<<(std::ostream &strm, InfiniteSet<int> s)
{
  int num = s.start;
  strm << "{ ";
  if (s.InfiniteStart)
  {
    strm << "... ";
    for (int i = 0, j = 0; i < PRINT_SIZE; num--, j++)
    {
      if (s.iselement(num))
        i++;
      else if (j >= PRINT_SIZE * PRINT_SIZE)
        throw out_of_range("InfiniteSet<int> : index is out of range");
    }
    if (s.InfiniteEnd)
    {
      for (int i = 0, j = 0; i <= PRINT_SIZE * 2; num++, j++)
      {
        if (s.iselement(num))
        {
          strm << num << " , ";
          i++;
        }
        else if (j >= PRINT_SIZE * PRINT_SIZE * 2)
          throw out_of_range("InfiniteSet<int> : index is out of range");
      }
      strm << "...";
    }
    else
    {
      for (int i = 0, j = 0; i <= PRINT_SIZE; num++, j++)
      {
        if (s.iselement(num))
        {
          strm << num << " , ";
          i++;
        }
        else if (j >= PRINT_SIZE * PRINT_SIZE)
          throw out_of_range("InfiniteSet<int> : index is out of range");
      }
    }
  }
  else if (s.InfiniteEnd)
  {
    for (int i = 0, j = 0; i <= PRINT_SIZE; num++, j++)
    {
      if (s.iselement(num))
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