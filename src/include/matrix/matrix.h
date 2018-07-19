#pragma once

#include <algorithm>
#include <iostream>
#include <iomanip>

#include "../common/generator.h"

namespace math{

    class size_missmatch : public std::exception{
    public:
        virtual const char* what(){
            return "size mismatch matrices";
        }
    };
    class not_square : public std::exception{
    public:
        virtual const char* what(){
            return "matrix is not square";
        }
    };
    class singular_matrix: public std::exception{
    public:
        virtual const char* what(){
            return "matrix is singular";
        }
    };

    struct row_placeholder{}_1;
    struct col_placeholder{}_2;

    template<class T>
    class matrix{

        T *data;
        int row,col;

    public:
        matrix(int size1, int size2, T value = 0) 
			: row(size1), col(size2)
		{
            data = new T[size1 * size2];
            std::fill(data, data + size1 * size2, value);
        }
        matrix(const matrix &other) 
			: row(other.row), col(other.col)
		{
            data = new T[other.size1() * other.size2()];
            std::copy(other.data, other.data + row * col, data);
        }
		matrix(matrix && other) 
			: row(other.row), col(other.col), data(other.data)
		{
			other.data = nullptr;
			other.row = 0;
			other.col = 0;
		}
        matrix& operator=(const matrix &other) {
               free();
               row = other.row;
               col = other.col;
               data = new T[other.size1() * other.size2()];
               std::copy(other.data, other.data + row * col, data);
               return *this;
        }

		matrix& operator=(matrix&& other) {
			free();
			row = other.row;
			col = other.col;
			data = other.data;

			other.data = nullptr;
			other.row = 0;
			other.col = 0;

			return *this;
		}
        bool size_equal(const matrix &other) const {
            return size1() == other.size1() && size2() == other.size2();
        }
        bool is_square() const {
            return size1() == size2();
        }
        bool operator==(const matrix &other) {
             if(!size_equal(other))return false;
             for(int i = 0; i < row; i++)
                 for(int j = 0; j < col; j++)
                     if(this->operator ()(i, j) != other(i, j))
                         return false;
             return true;
        }
        T& operator()(int i, int j) {
            if(i * col + j > row * col - 1)
                throw std::out_of_range("out of matrix range");
            return data[i * col + j];
        }
        T operator()(int i, int j) const {
            if(i * col + j > row * col - 1)
                throw std::out_of_range("out of matrix range");
            return data[i * col + j];
        }
        matrix operator+(const matrix &other) {
            if(!size_equal(other))
                throw size_missmatch();
            matrix result(row, col);
            for(int i = 0; i < row; i++)
                for(int j = 0; j < col; j++)
                    result(i, j) = this->operator ()(i, j) + other(i, j);
            return result;

        }
        matrix operator-(const matrix &other) {
            if(!size_equal(other))
                throw size_missmatch();
            matrix result(row, col);
            for(int i = 0; i < row; i++)
                for(int j = 0; j < col; j++)
                    result(i, j) = this->operator ()(i, j) - other(i, j);
            return result;

        }
        matrix operator*(const matrix &other) {
            if(size2() != other.size1())
                throw size_missmatch();
            matrix result(size1(), other.size2());
            for(int i = 0; i < result.size1(); i++)
                for(int j = 0; j < result.size2(); j++)
                {
                    for(int k = 0; k < size2(); k++)
                        result(i, j) += this->operator ()(i, k)*other(k, j);
                }
            return result;
        }
        matrix operator^(int degree) {
            if(!is_square())
                throw not_square();
            if(degree == 0)
                return eye(size1());
            if(degree < 0)
                return inverse(*this) ^ degree;
            matrix<T>result(*this);
            for(int i = 1; i < degree; i++)
                result *= result;
            return result;
        }
        matrix& operator+=(const matrix &other ){
            if(!size_equal(other))
                throw size_missmatch();
            for(int i = 0; i < row; i++)
                for(int j = 0; j < col; j++)
                    this->operator ()(i, j) += other(i, j);
            return *this;
        }
        matrix& operator-=(const matrix &other) {
            if(!size_equal(other))
                throw size_missmatch();
            for(int i = 0; i < row; i++)
                for(int j = 0; j < col; j++)
                    this->operator ()(i, j) -= other(i, j);
            return *this;
        }
        matrix& operator*=(const matrix &other) {
            if(size2() != other.size1())
                throw size_missmatch();
            matrix result(size1(), other.size2());
            for(int i = 0; i < result.size1(); i++)
                for(int j = 0; j < result.size2(); j++)
                {
                    for(int k = 0; k < size2(); k++)
                        result(i, j) += this->operator ()(i, k)*other(k, j);
                }
            return (*this = result);
        }
        matrix operator*(double factor) {
            matrix result(*this);
            std::for_each(result.data, result.data + row*col, [=](T &elem){
                  elem *= factor;
            });
            return result;
        }
        friend matrix operator*(double factor, const matrix &matr) {
             matrix result(matr);
             std::for_each(result.data, result.data + matr.row * matr.col, [=](T &elem){
                   elem *= factor;
             });
             return result;
        }
        friend std::ostream& operator<<(std::ostream &out, const matrix &m) {
            for(int i = 0; i < m.size1(); i++)
            {
                for(int j = 0; j < m.size2(); j++)
                    out << std::setw(10) << m(i, j);
                out << std::endl;
            }
            return out;
        }

        int size1() const {
            return row;
        }
		int size2() const {
			return col;
		}

        void clear() {
            std::fill(data, data+row * col, 0);
        }
        ~matrix() {
            free();
        }

        matrix operator()(row_placeholder, int j) {
            matrix result(size1(), 1);
            for(int i = 0; i < size1(); i++)
                result(i, 0) = this->operator ()(i, j);
            return result;
        }
        matrix operator ()(int j, col_placeholder) {
            matrix result(1, size2());
            for(int i = 0; i < size2(); i++)
                result(0, i) = this->operator ()(j, i);
            return result;
        }
        static matrix generate(int size1, int size2) {
            typedef generator<T> generator;
            matrix result(size1, size2);
            std::generate(result.data, result.data + size1 * size2, &generator::generate);
            return result;
        }
        static matrix eye(int size) {
            matrix result(size, size);
            for(int i = 0; i < size; i++)
                result(i, i) = 1;
            return result;
        }
	private:
		void free() {
			row = 0;
			col = 0;
			if (data) {
				delete[]data;
				data = nullptr;
			}
		}
    };

    template<class T>
    matrix<T> transpose(const matrix<T> &matr) {
        matrix<T> result(matr.size2(), matr.size1());
        for(int i = 0; i < matr.size1(); i++)
            for(int j = 0; j < matr.size2(); j++)
                result(j, i) = matr(i, j);
        return result;
    }
    template<class T>
    T determinant(const matrix<T> &matr) {
        if(!matr.is_square())
            throw not_square();
        if(matr.size1() == 1)
            return matr(0,0);
        if(matr.size1() == 2)
            return matr(0,0) * matr(1,1) - matr(0,1) * matr(1,0);
        T sum(0);
        for(int i = 0; i < matr.size2(); i++)
        {

            matrix<T>_minor(matr.size1() - 1, matr.size2() - 1);
            for(int j = 1; j < matr.size1(); j++)
            {
                int col_index(0);
                for(int k = 0; k < matr.size2(); k++)
                {
                    if(k != i)
                    {
                        _minor(j - 1, col_index) = matr(j, k);
                        col_index++;
                    }
                }
            }
            sum += (i % 2 == 0 ? matr(0, i) : -matr(0, i)) * determinant(_minor);
        }
        return sum;
    }
    template<class T>
    matrix<T> inverse(const matrix<T> &matr) {
        matrix<T>result(matr.size1(), matr.size2());
        auto det = determinant(matr);
        if(det == 0)
            throw singular_matrix();
        for(int i = 0; i < result.size1(); i++)
            for(int j = 0; j < result.size2(); j++)
            {
                matrix<T>_minor(result.size1() - 1, result.size2() - 1);
                int row_index(0);
                for(int k = 0; k < matr.size1(); k++)
                {
                    int col_index(0);
                    if(k != i)
                    {
                        for(int l = 0; l < matr.size2(); l++)
                            if(l != j)
                            {
                                _minor(row_index, col_index) = matr(k, l);
                                col_index++;
                            }
                        row_index++;
                    }
                }
                result(i, j) = (i + j) % 2 == 0 ? determinant(_minor) : -determinant(_minor);
            }
        return transpose(result) * (1.0 / det);
    }
}