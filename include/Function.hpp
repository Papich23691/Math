#ifndef MATH_FUNCT
#define MATH_FUNCT

#include <iostream>

using namespace std;
namespace math {
template <class X, class Y> class Function {
private:
  Set<X> domain;
  Set<Y> codomain;
  function<Y(X)> func;

public:
  Function(function<Y(X)> fun, Set<X> t, Set<Y> z)
      : func(fun), domain(t), codomain(z) {}
  ~Function() {}
  
  /* If function is injective */
  bool injective();
  
  /* If function is surjective */
  bool surjective();

  /* Returns f^-1 (y) */
  typename Set<X>::iterator find_argument(Y y);

  /* Returns f(x) */
  Y operator()(X domain);

  /* Get the codomain of f(X) (Domain set X) */
  Set<Y> get_codomain();

  /* Returns f^-1 */
  Function<Y, X> inverse();

  /* Returns f ∘ f2 */
  template <class Z> Function<Z, Y> operator*(Function<Z, X> &func2);

  /* Print */
  template <class A, class B>
  friend ostream &operator<<(std::ostream &strm, Function<A, B> &fun);
};

/*
*  If function is injective 
*  ∀x,y ∈ X (f(x) = f(y) -> y = x)
*/
template <class X, class Y> bool Function<X, Y>::injective() {
  for (int i = 0; i < this->domain.get_cardinality(); i++) {
    for (int j = 0; j < this->domain.get_cardinality(); j++) {
      if (this->domain[i] == this->domain[j] && j != i)
        return false;
    }
  }
  return true;
}

/*
*  If function is surjective 
*  ∀y ∈ Y ∃x ∈ X (f(x) = y)
*/
template <class X, class Y> bool Function<X, Y>::surjective() {
  for (int i = 0; i < this->codomain.get_cardinality(); i++) {
    if (find_argument(codomain[i]) == codomain.end())
      return false;
  }
  return true;
}

/* Returns f^-1(y) */
template <class X, class Y> typename Set<X>::iterator Function<X, Y>::find_argument(Y y) {
  for (int i = 0; i < this->domain.get_cardinality(); i++)
    if ((*this)(this->domain[i]) == y)
      return &this->domain[i];
  return this->domain.end();
}

/* Returns f(x) */
template <class X, class Y> Y Function<X, Y>::operator()(X domain) {
  return this->func(domain);
}

/* Returns f^-1 */
template <class X, class Y> Function<Y, X> Function<X, Y>::inverse() {
  function<X(Y)> f;
  if (!this->injective() || !this->surjective())
    f = [](Y i) { return NULL; };
  else
    f = [this](Y i) { return *(this->find_argument(i)); };
  return Function<Y, X>(f, this->codomain, this->domain);
}

/* Returns f(X) of domain X */
template <class X, class Y> Set<Y> Function<X, Y>::get_codomain() {
  Set<Y> temp;
  for (int i = 0; i < this->domain.get_cardinality(); i++) {
    temp.add((*this)(this->domain[i]));
  }
  return temp;
}

/* Returns f ∘ f2 */
template <class X, class Y>
template <class Z>
Function<Z, Y> Function<X, Y>::operator*(Function<Z, X> &func2) {
  Function<Z, Y> f([this, &func2](Z z) { return (*this)(func2(z)); },
                    func2.domain, this->get_codomain());
  return f;
}

/* Print */
template <class X, class Y>
ostream &operator<<(std::ostream &strm, Function<X, Y> &fun) {
  for (int i = 0; i < fun.domain.get_cardinality(); i++) {
    if (fun.codomain.is_element(fun(fun.domain[i])))
      strm << "( " << fun.domain[i] << " , " << fun(fun.domain[i]) << " )  ";
  }
  return strm;
}
} // namespace math

#endif