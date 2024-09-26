#include <vector>

#ifndef _MATRIX_H_
#define _MATRIX_H_

template <class T>
class matrix {
    private:
        int rows;
        int cols;
        std::vector<T> array;

        int index(int i, int j) {
            return i * cols + j;
        }
    public:
        using reference = typename std::vector<T>::reference;

        matrix<T> (int rows, int cols, T def);

        matrix<T> (const matrix<T> &that);

        reference operator()(int i, int j);

        reference operator()(int i) {
            return this->array[i];
        }
        
        int get_rows() {
            return this->rows;
        }

        int get_cols() {
            return this->cols;
        }

        // Matrix addition
        matrix<T> operator+(matrix<T> &that);

        // Matrix subtraction
        matrix<T> operator-(matrix<T> &that);

        // elementwise greater
        bool operator>=(matrix<T> &that);

        bool operator<=(matrix<T> &that);

        void operator=(matrix<T> that);
};

#endif