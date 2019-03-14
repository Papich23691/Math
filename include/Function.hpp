#ifndef MATH_FUNCT
#define MATH_FUNCT

#include <initializer_list>
#include <iostream>
#include <list>
#include <utility>

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
  bool Injective();
  bool Surjective();
  typename Set<X>::iterator FindArgument(Y y);
  Y operator()(X domain);
  Set<Y> getCodomain();
  Function<Y, X> Inverse();
  template <class Z> Function<Z, Y> operator*(Function<Z, X> &func2);
  template <class A, class B>
  friend ostream &operator<<(std::ostream &strm, Function<A, B> &fun);
};

template <class X, class Y> bool Function<X, Y>::Injective() {
  for (int i = 0; i < this->domain.Getcardinality(); i++) {
    for (int j = 0; j < this->domain.Getcardinality(); j++) {
      if (this->domain[i] == this->domain[j] && j != i)
        return false;
    }
  }
  return true;
}

template <class X, class Y> bool Function<X, Y>::Surjective() {
  for (int i = 0; i < this->domain.Getcardinality(); i++) {
    codomain.remove((*this)(this->domain[i]));
  }
  return !codomain.Getcardinality();
}

template <class X, class Y> typename Set<X>::iterator Function<X, Y>::FindArgument(Y y) {
  for (int i = 0; i < this->domain.Getcardinality(); i++)
    if ((*this)(this->domain[i]) == y)
      return &this->domain[i];
  return this->domain.end();
}

template <class X, class Y> Y Function<X, Y>::operator()(X domain) {
  return this->func(domain);
}

template <class X, class Y> Function<Y, X> Function<X, Y>::Inverse() {
  function<X(Y)> f;
  if (!this->Injective() || !this->Surjective())
    f = [](Y i) { return NULL; };
  else
    f = [this](Y i) { return *(this->FindArgument(i)); };
  return Function<Y, X>(f, this->codomain, this->domain);
}

template <class X, class Y> Set<Y> Function<X, Y>::getCodomain() {
  Set<Y> temp;
  for (int i = 0; i < this->domain.Getcardinality(); i++) {
    temp.add((*this)(this->domain[i]));
  }
  return temp;
}

template <class X, class Y>
template <class Z>
Function<Z, Y> Function<X, Y>::operator*(Function<Z, X> &func2) {
  Function<Z, Y> f([this, &func2](Z z) { return (*this)(func2(z)); },
                    func2.domain, this->getCodomain());
  return f;
}

template <class X, class Y>
ostream &operator<<(std::ostream &strm, Function<X, Y> &fun) {
  for (int i = 0; i < fun.domain.Getcardinality(); i++) {
    if (fun.codomain.iselement(fun(fun.domain[i])))
      strm << "( " << fun.domain[i] << " , " << fun(fun.domain[i]) << " )  ";
  }
  return strm;
}
} // namespace math

#endif