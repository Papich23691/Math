#ifndef L_A_MAT
#define L_A_MAT

#include <iostream>


using namespace std;

namespace math
{
    template <class T>
    class Matrix{
        private:
            Field<T> field;
            const int rows;
            const int columns;
            T ** mat_data;
        public:
            Matrix(int r,int c,Set<T> field)
            : field(field), rows(r), columns(c) {mat_data = (T **)malloc(sizeof(T) * rows * columns);}
            ~Matrix(){free(this->mat_data);};
            void set_value_at(int row,int column,T value){mat_data[row][column] = value;}
            T *operator[](int x){return mat_data[x];}
            Matrix<T> operator+(const Matrix<T> mat2);
            Matrix<T> operator+(const T scalar);
            Matrix<T> operator-(const Matrix<T> mat2);
            Matrix<T> operator-(const T scalar);
            Matrix<T> operator*(const Matrix<T> mat2);
            Matrix<T> operator*(const T scalar);
            double determinant();
            Matrix<T> tranpose();
            
    };
    template <class T>
    class Vector : public Matrix<T>{
        public:
            Vector(int r,Set<T> field) : Matrix<T>(r,1,field){}
            ~Vector(){free(this->mat_data);};
            T dor_product(const Vector<T> vec2);
            Vector<T> cross_product(const Vector<T> vec2);
            double cosine_similarity(const Vector<double> vec2);
            double norm() const;
            bool operator<(const Vector<double> vec2){return this->norm() < vec2.norm();}
            bool operator>(const Vector<double> vec2){return this->norm() > vec2.norm();}

    }; 
}

#endif