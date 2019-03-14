#ifndef MATH_SET
#define MATH_SET

#include <initializer_list>
#include <iostream>
#include <list>
#include <utility>
#include <iterator>

using namespace std;

namespace math
{
template <typename T>
class Set
{
public:
  Set() {}
  Set(list<T> list) : elements(list) { elements.sort(); }
  ~Set() {}
  class iterator
  {
  public:
    typedef iterator self_type;
    typedef T value_type;
    typedef T &reference;
    typedef typename list<T>::iterator pointer;
    typedef std::forward_iterator_tag iterator_category;
    typedef int difference_type;
    iterator(pointer ptr) : ptr_(ptr) {}
    self_type operator++()
    {
      self_type i = *this;
      ptr_++;
      return i;
    }
    self_type operator++(int junk)
    {
      ptr_++;
      return *this;
    }
    reference operator*() { return *ptr_; }
    pointer operator->() { return ptr_; }
    bool operator==(const self_type &rhs) { return ptr_ == rhs.ptr_; }
    bool operator!=(const self_type &rhs) { return ptr_ != rhs.ptr_; }

  private:
    typename list<T>::iterator ptr_;
  };

  iterator begin() { return iterator(this->elements.begin()); }
  iterator end() { return iterator(this->elements.end()); }
  void remove(T element);
  void add(T element);
  bool subsetOf(Set<T> &s);
  bool iselement(T element);
  bool operator==(Set<T> &other);
  bool operator!=(Set<T> &other) { return !(*this == other); }
  T &operator[](int x)
  {
    typename list<T>::iterator point = this->elements.begin();
    advance(point, x);
    return *point;
  }
  bool operator<(const Set<T> &other) const;
  bool operator>(const Set<T> &other) const;
  Set<Set<T>> powerSet();
  int Getcardinality() const { return this->elements.size(); };
  Set<T> unite(Set &s);
  Set<T> intersection(Set<T> &s);
  Set<T> operator-(Set<T> &s);
  Set<pair<T, T>> operator*(Set<T> &s);
  template <class U>
  friend ostream &operator<<(std::ostream &strm, Set<U> s);
  template <class U>
  friend ostream &operator<<(std::ostream &strm, Set<pair<U, U>> s);

private:
  list<T> elements;
};

template <class T>
void Set<T>::remove(T element)
{
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    if (*i == element)
      this->elements.erase(i);
  }
}

template <class T>
void Set<T>::add(T element)
{
  this->elements.push_back(element);
  this->elements.sort();
}

template <class T>
bool Set<T>::subsetOf(Set<T> &s)
{
  if (!this->Getcardinality())
    return true;
  if (!s.Getcardinality() && this->Getcardinality())
    return false;
  for (typename list<T>::iterator point = this->elements.begin();
       point != this->elements.end(); point++)
  {
    if (!s.iselement(*point))
      return false;
  }
  return true;
}

template <class T>
bool Set<T>::iselement(T element)
{
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    if (*i == element)
      return true;
  }
  return false;
}

template <class T>
bool Set<T>::operator==(Set<T> &s)
{
  if (!s.Getcardinality())
    return !this->Getcardinality();
  else if (!this->Getcardinality())
    return !s.Getcardinality();
  return this->subsetOf(s) && s.subsetOf(*this);
}

template <class T>
bool Set<T>::operator<(const Set<T> &other) const
{
  return this->Getcardinality() < other.Getcardinality();
}

template <class T>
bool Set<T>::operator>(const Set<T> &other) const
{
  return this->Getcardinality() > other.Getcardinality();
}

template <class T>
Set<Set<T>> Set<T>::powerSet()
{
  Set<Set<T>> ret;
  ret.add(Set<T>());
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    Set<Set<T>> rs;
    for (int j = 0; j < ret.Getcardinality(); j++)
    {
      Set<T> x = ret[j];
      x.add(*i);
      rs.add(x);
    }
    ret = ret.unite(rs);
  }
  return ret;
}

template <class T>
Set<T> Set<T>::unite(Set<T> &s)
{
  Set<T> temp = *this;
  for (int i = 0; i < s.Getcardinality(); i++)
  {
    if (!temp.iselement(s[i]))
      temp.add(s[i]);
  }
  return temp;
}

template <class T>
Set<T> Set<T>::intersection(Set<T> &s)
{
  list<T> inter;
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    if (s.iselement(*i))
      inter.push_back(*i);
  }
  inter.sort();
  Set<T> temp(inter);
  return temp;
}

template <class T>
Set<T> Set<T>::operator-(Set<T> &s)
{
  Set<T> temp = *this;
  for (typename list<T>::iterator point = s.elements.begin();
       point != s.elements.end(); point++)
  {
    temp.remove(*point);
  }
  return temp;
}

template <class T>
Set<pair<T, T>> Set<T>::operator*(Set<T> &s)
{
  list<pair<T, T>> el;
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    for (int j = 0; j < s.Getcardinality(); j++)
    {
      el.push_back(pair<T, T>(*i, s[j]));
    }
  }
  Set<pair<T, T>> temp(el);
  return temp;
}

template <class T>
ostream &operator<<(std::ostream &strm, Set<T> s)
{
  s.elements.sort();
  strm << "{";
  for (int i = 0; i < s.Getcardinality(); i++)
  {
    strm << " " << s[i] << " ";
    if (i != s.Getcardinality() - 1)
      strm << ",";
  }
  return strm << "}";
}

template <class T>
ostream &operator<<(std::ostream &strm, Set<pair<T, T>> s)
{
  s.elements.sort();
  strm << "{";
  for (int i = 0; i < s.Getcardinality(); i++)
  {
    strm << " "
         << "( " << s[i].first << " , " << s[i].second << " )"
         << " ";
    if (i != s.Getcardinality() - 1)
      strm << ",";
  }
  return strm << "}";
}

template <class T>
typename Set<T>::iterator begin(Set<T> &s)
{
  return s.begin();
}

template <class T>
typename Set<T>::iterator end(Set<T> &s)
{
  return s.end();
}
} // namespace math

#endif