#include <iostream>
#include <string>
#include "math_s.h"

using namespace std;
using namespace math;

/* Examples */
int main()
{
    cout << "\nSets\n"
         << endl;
    Set<int> X({1, 2, 3, 4, 5, 6});
    Set<int> Y({25, 36, 49, 64, 81});
    Set<int> Z({5, 6, 7, 8, 9});
    Set<pair<int, int>> M = Y * Z;
    cout << X << " intersection " << Z << ":" << endl;
    cout << X.intersection(Z) << "\n"
         << endl;
    cout << Z << " powerset "
         << ":" << endl;
    cout << Z.power_set() << "\n"
         << endl;
    cout << Y << " * " << Z << ":" << endl;
    cout << M << endl;

    cout << "\nBinary\n"
         << endl;
    BinaryOperation<int> op([](int x, int y) {
        if (x == 2)
            return y;
        else if (y == 2)
            return x;
        else
            return x + y;
    });
    cout << "Binary operation (x,y)\nif (x==2)\t (x,y)=y\nif (y==2)\t "
            "(x,y)=x\n(x,y)=x+y\n\nIdentity element of set "
         << X
         << endl;
    cout << *op.identity_element(X) << endl;
    cout << "Left Inverse in set" << X << " element 1 " << endl;
    cout << *op.left_inverse_element(X, 1) << endl;
    cout << "Calculation of all elemeents of set " << M << endl;
    cout << op.calc_set(M) << endl;

    cout << "\nFunctions\n"
         << endl;
    Function<int, int> f1([](int x) { return x + 5; }, X, Z);
    Function<int, int> f2([](int x) { return x * x; }, Z, Y);
    Function<int, int> f3 = f2 * f1;
    cout << "Function f1(x)=x+5\t" << X << " -> " << Z << endl;
    cout << f1 << endl;
    cout << "Function f2(x)=x*x\t" << Z << " -> " << Y << endl;
    cout << f2 << endl;
    cout << "Function f3(x)=f2(f1(x))\t" << X << " -> " << Y << endl;
    cout << f3 << endl;

    cout << "\nInfinite Sets\n"
         << endl;
    InfiniteSet<int> N([](int x) { return !(x % 1) && x > -1; }, 100, 0);
    InfiniteSet<int> N_zero([](int x) { return !(x % 1) && x > 0; }, 67, 0);
    InfiniteSet<int> EN([](int x) { return x > 89; }, 21, 0, false, true);
    InfiniteSet<InfiniteSet<int>> PN = N.power_set();
    PN.remove(N_zero);
    cout << "Is infinite group of numbers greater than 89 an element of P(N)-(N-{0}):" << endl;
    cout << PN.is_element(EN) << endl;
     cout << "Is N-{0} an element of P(N)-(N-{0}):" << endl;
    cout << PN.is_element(N_zero) << endl;
    EN.add(9);
    cout << "N:" << endl;
    cout << N << endl;
    cout << "infinite group of numbers greater than 89 :" << endl;
    cout << EN << endl;
    cout << "Intersection of N and previous set:" << endl;
    cout << N.intersection(EN) << endl;
    InfiniteSet<int> NUM([](int x) { return true; }, 100, 0, true);
    cout << "Z:" << endl;
    cout << NUM << endl;
    return 0;
}
