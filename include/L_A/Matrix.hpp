#ifndef L_A_MAT
#define L_A_MAT

#include <iostream>

using namespace std;

namespace math
{
template <class T>
class Matrix
{
protected:
    Field<T> field;
    const int rows;
    const int columns;
    Matrix<T> minimize_matrix(int row, int column);
    vector<vector<T>> mat_data;
public:
    Matrix(int r, int c, Field<T> field)
        : field(field), rows(r), columns(c)
    {
        mat_data.resize(rows);
        for (int i=0;i<rows;i++)
            mat_data[i].resize(columns);
    }
    ~Matrix(){}
    vector<T> &operator[](int x) { return mat_data[x]; }
    Matrix<T> operator+(const Matrix<T> mat2);
    Matrix<T> operator+(const T scalar);
    Matrix<T> operator-(const Matrix<T> mat2);
    Matrix<T> operator-(const T scalar);
    Matrix<T> operator*(const Matrix<T> mat2);
    Matrix<T> operator*(const T scalar);
    double determinant();
    Matrix<T> transpose();
    template <class U>
    friend ostream &operator<<(std::ostream &strm, Matrix<U> m);
};
template <class T>
class Vector : public Matrix<T>
{
public:
    Vector(int r, Field<T> field) : Matrix<T>(r, 1, field) {}
    ~Vector() {};
    T dot_product(Vector<T> vec2);
    Vector<T> cross_product(const Vector<T> vec2) ;
    double cosine_similarity( Vector<double> vec2) ;
    double norm() ;
    T &operator[](int x) { return this->mat_data[x][0]; }
    bool operator<( Vector<double> vec2) { return this->norm() < vec2.norm(); }
    bool operator>( Vector<double> vec2) { return this->norm() > vec2.norm(); }
};

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> mat2)
{
    Matrix<T> sum(this->rows, this->columns, this->field);
    if (mat2.columns == this->columns && mat2.rows == this->rows)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->columns; j++)
            {
                sum.mat_data[i][j] = field.field_add(this->mat_data[i][j], mat2.mat_data[i][j]);
            }
        }
    }
    return sum;
}

template <class T>
Matrix<T> Matrix<T>::operator+(const T scalar)
{
    Matrix<T> sum(this->rows, this->columns, this->field);
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            sum.mat_data[i][j] = field.field_add(this->mat_data[i][j], scalar);
        }
    }
    return sum;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> mat2)
{
    Matrix<T> sub(this->rows, this->columns, this->field);
    if (mat2.columns == this->columns && mat2.rows == this->rows)
    {
        for (int i = 0; i < this->rows; i++)
        {
            for (int j = 0; j < this->columns; j++)
            {
                sub.mat_data[i][j] = field.field_sub(this->mat_data[i][j], mat2.mat_data[i][j]);
            }
        }
    }
    return sub;
}

template <class T>
Matrix<T> Matrix<T>::operator-(const T scalar)
{
    Matrix<T> sub(this->rows, this->columns, this->field);
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            sub.mat_data[i][j] = field.field_sub(this->mat_data[i][j], scalar);
        }
    }
    return sub;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> mat2)
{
    T sum = this->field.get_zero;
    Matrix<T> mul(this->rows, this->columns, this->field);
    if (mat2.rows == this->columns)
    {
        for (int i = 0; i < mat2.columns; i++)
        {
            for (int k = 0; k < this->rows; k++)
            {
                for (int j = 0; j < this->columns; j++)
                {
                    sum = this->field.field_add(sum, this->field.field_mul(mat2.mat_data[j][i], this->mat_data[k][j]));
                }
                mul.mat_data[k][i] = sum;
                sum = this->field.get_zero;
            }
        }
    }
    return mul;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const T scalar)
{
    Matrix<T> mul(this->rows, this->columns, this->field);
    for (int i = 0; i < this->rows; i++)
    {
        for (int j = 0; j < this->columns; j++)
        {
            mul.mat_data[i][j] = field.field_mul(this->mat_data[i][j], scalar);
        }
    }
    return mul;
}

template <class T>
Matrix<T> Matrix<T>::minimize_matrix(int row, int column)
{
    Matrix<T> minimize(this->rows - 1, this->columns - 1, this->field);
    if (this->rows != this->columns)
        return *this;
    for (int i = 0; i < this->rows ; i++)
    {
        if (i != row)
        {
            for (int j = 0; j < this->rows ; j++)
            {
                if (j != column)
                    minimize.mat_data[i < row ? i : i - 1][j < column ? j : j - 1] = this->mat_data[i][j];
            }
        }
    }
    return minimize;
}

template <class T>
double Matrix<T>::determinant()
{
    double det = 0;
    if (!is_same<T, double>::value && !is_same<T, float>::value)
        throw invalid_argument("Must be of type float or double");
    if (this->rows != this->columns)
        throw invalid_argument("Not a square matrix");
    if (this->rows == 2)
    {
        return (this->mat_data[1][1] * this->mat_data[0][0]) - (this->mat_data[0][1] * this->mat_data[1][0]);
    }
    for (int i = 0; i < this->rows; i++)
    {
      det += pow(-1, i) * this->mat_data[0][i] * this->minimize_matrix(0, i).determinant();
    }
    return det;
}

template <class T>
Matrix<T> Matrix<T>::transpose()
{
    Matrix<T> transposed(this->columns,this->rows,this->field);
    for (int i=0;i<this->rows;i++)
    {
        for (int j=0;j<this->columns;j++)
            transposed.mat_data[j][i] = this->mat_data[i][j];
    }
    return transposed;
}


template <class T>
ostream &operator<<(std::ostream &strm, Matrix<T> m)
{
    for (int i=0;i<m.rows;i++){
        strm << "{";
        for (int j=0;j<m.columns;j++){
            cout << m.mat_data[i][j] ;
            if ((j+1) != m.columns)
                strm << "   " ; 
        }
        strm << "}\n";
    }
    return strm;
}
template <class T>
T Vector<T>::dot_product( Vector<T> vec2) 
{
    double d_p=0;
    for (int i=0;i < (this->rows < vec2.rows ? this->rows:vec2.rows) ; i++)
    {
        d_p += this->field.field_mul((*this)[i],vec2[i]);
    }
    return d_p;
}

template <class T>
Vector<T> Vector<T>::cross_product( Vector<T> vec2) 
{
    Vector<T> cross(3,this->field);
    if (this->rows < 3 || vec2.rows < 3)
        throw invalid_argument("Must be at least a 3D vector");
    cross[0] = this->field.field_sub(this->field.field_mul(this->mat_data[1][0], vec2.mat_data[2][0]) ,  this->field.field_mul(this->mat_data[2][0], vec2.mat_data[1][0]));
    cross[1] = this->field.field_sub(this->field.field_mul(this->mat_data[2][0], vec2.mat_data[0][0]) ,  this->field.field_mul(this->mat_data[0][0], vec2.mat_data[2][0]));
    cross[2] = this->field.field_sub(this->field.field_mul(this->mat_data[0][0], vec2.mat_data[1][0]) ,  this->field.field_mul(this->mat_data[1][0], vec2.mat_data[0][0]));
    return cross;

}

template <class T>
double Vector<T>::norm() 
{
    if (!is_same<T, double>::value && !is_same<T, float>::value)
        throw invalid_argument("Must be of type float or double");
    return sqrt(this->dot_product((*this)));
}

template <class T>
double Vector<T>::cosine_similarity( Vector<double> vec2) 
{
    if (!is_same<T, double>::value && !is_same<T, float>::value)
        throw invalid_argument("Must be of type float or double");
    return (this->dot_product(vec2)) / (vec2.norm() * this->norm());
}


} // namespace math

#endif