#ifndef _VECTORMATRIX_HPP_
#define _VECTORMATRIX_HPP_

#include <vector>

// Todo: ассерты блеадь

namespace zog {

    template <typename T>
    class matrix {

    protected:
        std::vector<T> _vector;

    public:
        matrix() = default;
        void reserve(size_t newCapacity) { _vector.reserve(newCapacity * newCapacity); _vector[0][0] = 0; }
    };

}



#endif //_VECTORMATRIX_HPP_