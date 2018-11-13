#ifndef __ARRAY_POOL__
#define __ARRAY_POOL__

#include "../Asserts.hpp"
#include "../types.hpp"
#include <cstdio>
#include <cstddef>

#define VAL_START	(128U - sizeof(Node))
#define VAL_MAGN	(128U)
#define NOD_MAGN	(16384U)

class ArrayPool {

private:
    struct			Node {
        Node*		    _next;
        Node*		    _prev;
        size_t		    _bytes;
        bool		    _isOccupied;
        //bool		    _dummy0;
        //std::int16_t    _dummy1;
    };

    size_t			_val_start;
    size_t			_val_magn;
    size_t			_nod_magn;

    Node*			_first;
    Node*			_last;
    Node*			_current;
    size_t			_count;


private:
    static Node*	    _get_node		(void* block)	{ return reinterpret_cast<Node*>(reinterpret_cast<std::byte*>(block) - sizeof(Node)); }
    static std::byte*	_get_block		(Node* node)	{ return reinterpret_cast<std::byte*>(node) + sizeof(Node); }
    static Node*	    _node_malloc	(size_t N)		{ return reinterpret_cast<Node*>(::malloc(sizeof(Node) + N)); }
    void			    _node_init		(Node* node)	{ node->_bytes = _val_start; node->_isOccupied = false; }


    Node*			realoc_block		(Node* ptr, size_t size, bool free_ptr = true);
    void			extend				();



public:
    ArrayPool							(size_t block_size, size_t block_magn, size_t node_magn);
    ArrayPool							(): ArrayPool(VAL_START, VAL_MAGN, NOD_MAGN){}
    ~ArrayPool							();


    void*			malloc				(size_t size);
    void*			calloc				(size_t count, size_t size)	{ return malloc(count * size); }
    void*			realloc				(void* ptr, size_t size);
    void			free				(void* ptr);



public:
    template<size_t T>
    void			_toMaxPrefix		(char(&S)[T], size_t _Bytes) {
        size_t counter = 0U;
        double bytes = _Bytes;
        while (bytes > 1024.0) {
            bytes /= 1024.0;
            ++counter;
        }

        switch (counter) {
        case 0: sprintf(S, "%.2lf  bytes", bytes); break;
        case 1: sprintf(S, "%.2lf Kbytes", bytes); break;
        case 2: sprintf(S, "%.2lf Mbytes", bytes); break;
        case 3: sprintf(S, "%.2lf Gbytes", bytes); break;
        case 4: sprintf(S, "OOM is a social construct");
        }
    }
    /*get info about alloceted block*/
    void			_infoMemoryBlocks	();
    /*check size of Node struct*/
    void			_sizeofNode			();

};

#undef VAL_START
#undef VAL_MAGN
#undef NOD_MAGN


#endif // !__ARRAY_POOL__