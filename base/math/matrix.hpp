#ifndef _MATRIX_HPP_
#define _MATRIX_HPP_

#include <vector>
#include <iostream>
#include <bitset>
#include "../types.hpp"
#include "../engineDefines.hpp"
#include "../Asserts.hpp"



template<U32 S, typename T>
class MathVector {
protected:
    T	_vector[S];

public:
    MathVector				() = default;
    MathVector				(std::initializer_list<T> list) { U32 j = 0; for(auto& i : list) _vector[j++] = i; }

    virtual void rotate_r	(U32 steps) {
        steps = steps % S;
        if (steps == 0U) return;

        T temp[S];
        for (U32 i = 0U; i < S; ++i)
            temp[(i + steps) % S] = _vector[i];
        for (U32 i = 0U; i < S; ++i)
            _vector[i] = temp[i];
    }
    virtual void rotate_l	(U32 steps) {
        steps = steps % S;
        if (steps == 0U) return;

        T temp[S];
        for (U32 i = 0U; i < S; ++i)
            temp[i] = _vector[(i + steps) % S];
        for (U32 i = 0U; i < S; ++i)
            _vector[i] = temp[i];
    }

    T&	operator[]			(U32 pos)					{ return _vector[pos]; }
    MathVector&	operator=	(const MathVector& right)	{ memcpy_s(_vector, S * sizeof(T), right._vector, S * sizeof(T)); return *this; }
    MathVector	operator^	(const MathVector& right)	{ MathVector ret; for (U32 i = 0; i < S; ++i) ret[i] = this->_vector[i] ^ right._vector[i]; return ret; }


public:
    virtual void _print() {
        for (U32 i = 0; i < S; ++i)
            printf_s("0x%02x  ", _vector[i]);
        printf("\n");
    }
};


template<U32 S, typename T>
class SquareMatrix {
protected:
    T	_matrix[S][S];


public:
    SquareMatrix					()
    {
        for (U32 i = 0; i < S; ++i)
            for (U32 j = 0; j < S; ++j)
                _matrix[i][j] = (T)0;
    }
    SquareMatrix					(T(&matrix)[S][S], bool fill_per_columns = false) {
        if (fill_per_columns)
            for (U32 i = 0; i < S; ++i)
                for (U32 j = 0; j < S; ++j)
                    _matrix[j][i] = matrix[i][j];
        else
        memcpy_s(_matrix, (S*S) * sizeof(T), matrix, (S*S) * sizeof(T));
    }
    void		shift_row_r			(U32 row, U32 steps)
    {
        ASSERT(row < S, "matrix out of border");
        steps = steps % S;
        if (steps == 0U) return;

        T temp[S];
        for (U32 i = 0U; i < S; ++i)
            temp[(i + steps) % S] = _matrix[row][i];
        for (U32 i = 0U; i < S; ++i)
            _matrix[row][i] = temp[i];
    }
    void		shift_row_l			(U32 row, U32 steps)
    {
        ASSERT(row < S, "matrix out of border");
        steps = steps % S;
        if (steps == 0U) return;

        T temp[S];
        for (U32 i = 0U; i < S; ++i)
            temp[i] = _matrix[row][(i + steps) % S];
        for (U32 i = 0U; i < S; ++i)
            _matrix[row][i] = temp[i];
    }
    void		shift_column_u		(U32 column, U32 steps)
    {
        ASSERT(column < S, "matrix out of border");
        steps = steps % S;
        if (steps == 0U) return;

        T temp[S];
        for (U32 i = 0U; i < S; ++i)
            temp[i] = _matrix[(i + steps) % S][column];
        for (U32 i = 0U; i < S; ++i)
            _matrix[i][column] = temp[i];
    }
    void		shift_column_d		(U32 column, U32 steps)
    {
        ASSERT(column < S, "matrix out of border");
        steps = steps % S;
        if (steps == 0U) return;

        T temp[S];
        for (U32 i = 0U; i < S; ++i)
            temp[(i + steps) % S] = _matrix[i][column];
        for (U32 i = 0U; i < S; ++i)
            _matrix[i][column] = temp[i];
    }
    void		transpose			()
    {
        if (S == 1) return;
        for (U32 j = 1; j < S; ++j) {
            for (U32 i = 0; i < j; ++i) {
                T temp = _matrix[j][i];
                _matrix[j][i] = _matrix[i][j];
                _matrix[i][j] = temp;
            }
        }
    }
    void		xor_row_vector		(MathVector<S, T>& vec, U32 row) {
        ASSERT(row < S, "matrix out of border");
        for (U32 i = 0; i < S; ++i)
            _matrix[row][i] ^= vec[i];
    }
    void		xor_column_vector	(MathVector<S, T>& vec, U32 column) {
        ASSERT(column < S, "matrix out of border");
        for (U32 i = 0; i < S; ++i)
            _matrix[i][column] ^= vec[i];
    }
    void		array_set			(T* arr, U32 last = S * S)
    {
        U32 counter = 0U;
        for (U32 i = 0U; i < S; ++i)
            for (U32 j = 0U; j < S; ++j) {
                if (counter == last) return;
                _matrix[i][j] = *arr;
                ++arr;
                ++counter;
            }
    }
    void		bytes_set			(bytes_t& bytes, U32 _first, U32 last)
    {
        ASSERT(last > _first, "SquareMatrix: bytes_set(): last <= first");
        ASSERT(last <= bytes.size(), "SquareMatrix: bytes_set(): last > bytes.size()");

        auto* ptr = (U8*)&(_matrix[0][0]);

        for (U32 i = _first; i < last; ++i) {
            *ptr = bytes[i];
            ++ptr;
        }
    }
    bytes_t		to_bytes			()
    {
        auto size = S * S * sizeof(T);
        bytes_t bytes;
        bytes.reserve(size);
        auto* ptr = (U8*)&(_matrix[0][0]);

        for (U32 i = 0U; i < size; ++i) {
            bytes.push_back(*ptr);
            ++ptr;
        }
        return bytes;
    }

    T*			operator[]			(U32 pos) { return _matrix[pos]; }
    bool		operator==			(const SquareMatrix& right) const
    {
        for (U32 i = 0; i < S; ++i)
            for (U32 j = 0; j < S; ++j)
                if (_matrix[i][j] != right._matrix[i][j])
                    return false;
        return true;
    }

public:
    void _print() {
        for (U32 i = 0; i < S; ++i) {
            for (U32 j = 0; j < S; ++j)
                printf_s("0x%02x  ", _matrix[i][j]);
            printf("\n");
        }
        printf("\n");
    }

    void _print_linear(bool transf = true) {
        for (U32 i = 0; i < S; ++i) {
            for (U32 j = 0; j < S; ++j)
                printf_s("%02x", _matrix[i][j]);
        }
        if (transf)	printf("\n");
    }
    void _print_char() {
        for (U32 i = 0; i < S; ++i) {
            for (U32 j = 0; j < S; ++j)
                printf_s("%c", _matrix[i][j]);
        }
        printf("\n");
    }

};

#endif // !_MATRIX_HPP_