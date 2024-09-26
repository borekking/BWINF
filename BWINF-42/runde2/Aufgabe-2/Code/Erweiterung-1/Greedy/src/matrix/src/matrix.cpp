#include <vector>
#include "matrix.h"

template <class T>
matrix<T>::matrix(int rows, int cols, T def) {
    this->rows = rows;
    this->cols = cols;
    this->array.resize(rows * cols);

    for (int i = 0; i < (int) this->array.size(); i++) {
        this->array[i] = def;
    }
}

template <class T>
matrix<T>::matrix(const matrix<T> &that) {
    this->rows = that.rows;
    this->cols = that.cols;
    this->array = that.array;
}

template <class T>
typename std::vector<T>::reference matrix<T>::operator()(int i, int j) {
    return (*this)(index(i, j));
}

// Matrix addition
template <class T>
matrix<T> matrix<T>::operator+(matrix<T> &that) {
    matrix<T> sum((*this));

    if (this->cols * this->rows != that.cols * that.rows) return sum;
            
    for (int i = 0; i < this->rows * this->cols; i++) {
        sum(i) = sum(i) + that(i);
    }

    return sum;
}

// Matrix subtraction
template <class T>
matrix<T> matrix<T>::operator-(matrix<T> &that) {
    matrix<T> sum((*this));

    if (this->cols * this->rows != that.cols * that.rows) return sum;
            
    for (int i = 0; i < this->rows * this->cols; i++) {
        sum(i) = sum(i) - that(i);
    }

    return sum;
}

// elementwise greater
template <class T>
bool matrix<T>::operator>=(matrix<T> &that) {
    if (this->cols * this->rows != that.cols * that.rows) return false;
        
    for (int i = 0; i < this->rows * this->cols; i++) {
        if ((*this)(i) < that(i)) return false;
    }

    return true;
}

template <class T>
bool matrix<T>::operator<=(matrix<T> &that) {
    if (this->cols * this->rows != that.cols * that.rows) return false;
            
    for (int i = 0; i < this->rows * this->cols; i++) {
        if ((*this)(i) > that(i)) return false;
    }
    return true;
}

template <class T>
void matrix<T>::operator=(matrix<T> that) {
    this->rows = that.rows;
    this->cols = that.cols;
    this->array = that.array;
}