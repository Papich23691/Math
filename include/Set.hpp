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
  /* Set iterator */
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

  /* Begin and end of set */
  iterator begin() { return iterator(this->elements.begin()); }
  iterator end() { return iterator(this->elements.end()); }
  
  void remove(T element);
  void add(T element);
  
  /* If this ⊆ s*/
  bool sub_set_of(Set<T> &s);

  /* If element ∈ this */
  bool is_element(T element);

  /* If this = other */
  bool operator==(Set<T> &other);
  bool operator!=(Set<T> &other) { return !(*this == other); }

  /* Returns this[x] */
  T &operator[](int x)
  {
    typename list<T>::iterator point = this->elements.begin();
    advance(point,x);
    return *point;
  }

  /* Compares cardinality */
  bool operator<(const Set<T> &other) const;
  bool operator>(const Set<T> &other) const;

  /* Returns P(this) */
  Set<Set<T>> power_set();

  /* Returns |this| */
  int get_cardinality() const { return this->elements.size(); };

  /* Returns this ⋃ s */
  Set<T> unite(Set &s);

  /* Returns this ⋂ s */
  Set<T> intersection(Set<T> &s);

  /* Returns this/s */
  Set<T> operator-(Set<T> &s);

  /*Returns this * s */
  Set<pair<T, T>> operator*(Set<T> &s);

  /* Printing */
  template <class U>
  friend ostream &operator<<(std::ostream &strm, Set<U> s);
  template <class U>
  friend ostream &operator<<(std::ostream &strm, Set<pair<U, U>> s);

private:
  list<T> elements;
};


/**
 * @brief Removes a given element from set
 * 
 * @param given element 
 */
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

/**
 * @brief Adds a given element to a set
 * 
 * @param element The element being added
 */
template <class T>
void Set<T>::add(T element)
{
  this->elements.push_back(element);
  this->elements.sort();
}

/**
 * @brief Returns if the set is a subset of s
 * 
 * @return true this ⊆ s
 * @return false this ⊈ s
 */
template <class T>
bool Set<T>::sub_set_of(Set<T> &s)
{
  if (!this->get_cardinality())
    return true;
  if (!s.get_cardinality() && this->get_cardinality())
    return false;
  for (typename list<T>::iterator point = this->elements.begin();
       point != this->elements.end(); point++)
  {
    if (!s.is_element(*point))
      return false;
  }
  return true;
}

/**
 * @brief Returns if a given elements is an element of the set
 * 
 * @return true element ∈ this
 * @return false element ∉ this
 */
template <class T>
bool Set<T>::is_element(T element)
{
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    if (*i == element)
      return true;
  }
  return false;
}

/* If this = other */
template <class T>
bool Set<T>::operator==(Set<T> &s)
{
  /* If both has cardinality of 0 */
  if (!s.get_cardinality())
    return !this->get_cardinality();
  else if (!this->get_cardinality())
    return false;
  /* this ⊆ s ⋀ s ⊆ this */
  return this->sub_set_of(s) && s.sub_set_of(*this);
}

/* Compares cardinality */
template <class T>
bool Set<T>::operator<(const Set<T> &other) const
{
  return this->get_cardinality() < other.get_cardinality();
}

template <class T>
bool Set<T>::operator>(const Set<T> &other) const
{
  return this->get_cardinality() > other.get_cardinality();
}

/* Returns P(this) */
template <class T>
Set<Set<T>> Set<T>::power_set()
{
  Set<Set<T>> ret;
  /* Adds ∅ */
  ret.add(Set<T>());
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    /* Each iteration adds the current element to all previous sets in ret and adds the new sets to ret */
    Set<Set<T>> rs;
    for (int j = 0; j < ret.get_cardinality(); j++)
    {
      Set<T> x = ret[j];
      x.add(*i);
      rs.add(x);
    }
    ret = ret.unite(rs);
  }
  return ret;
}

  /* Returns this ⋃ s */
template <class T>
Set<T> Set<T>::unite(Set<T> &s)
{
  Set<T> temp = *this;
  for (int i = 0; i < s.get_cardinality(); i++)
  {
    if (!temp.is_element(s[i]))
      temp.add(s[i]);
  }
  return temp;
}

/* Returns this ⋂ s */
template <class T>
Set<T> Set<T>::intersection(Set<T> &s)
{
  list<T> inter;
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    if (s.is_element(*i))
      inter.push_back(*i);
  }
  inter.sort();
  Set<T> temp(inter);
  return temp;
}

/* Returns this/s */
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

/*Returns this * s */
template <class T>
Set<pair<T, T>> Set<T>::operator*(Set<T> &s)
{
  list<pair<T, T>> el;
  for (typename list<T>::iterator i = this->elements.begin();
       i != this->elements.end(); i++)
  {
    for (int j = 0; j < s.get_cardinality(); j++)
    {
      el.push_back(pair<T, T>(*i, s[j]));
    }
  }
  Set<pair<T, T>> temp(el);
  return temp;
}

/* Printing */
template <class T>
ostream &operator<<(std::ostream &strm, Set<T> s)
{
  s.elements.sort();
  strm << "{";
  for (int i = 0; i < s.get_cardinality(); i++)
  {
    strm << " " << s[i] << " ";
    if (i != s.get_cardinality() - 1)
      strm << ",";
  }
  return strm << "}";
}

template <class T>
ostream &operator<<(std::ostream &strm, Set<pair<T, T>> s)
{
  s.elements.sort();
  strm << "{";
  for (int i = 0; i < s.get_cardinality(); i++)
  {
    strm << " "
         << "( " << s[i].first << " , " << s[i].second << " )"
         << " ";
    if (i != s.get_cardinality() - 1)
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