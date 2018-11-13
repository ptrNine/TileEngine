#include <cstring>
#include "array_allocator.hpp"

ArrayPool sharedAlloc = ArrayPool(48U, 32U, 16384U);



ArrayPool::ArrayPool (size_t block_size, size_t block_magn, size_t node_magn) : _val_start(block_size), _val_magn(block_magn), _nod_magn(node_magn)
{
    _count				= _nod_magn;
    Node* cur			= _node_malloc(_val_start);

    ASSERT		(cur != nullptr, "out of memory");
    _node_init	(cur);

    cur->_prev			= nullptr;
    _first				= _current = cur;

    for (size_t i = 0; i < _nod_magn - 1; ++i)
    {
        cur->_next			= _node_malloc(_val_start);

        ASSERT		(cur->_next	!= nullptr, "out of memory");
        _node_init	(cur->_next);

        cur->_next->_prev	= cur;
        cur					= cur->_next;
    }

    _last				= cur;
    cur->_next			= nullptr;
}



ArrayPool::~ArrayPool ()
{
    Node* cur	= _last;

    while (cur)
    {
        Node* prev	= cur->_prev;
        ::free(cur);
        cur			= prev;
    }

    _first		= nullptr;
    _current	= nullptr;
    _last		= nullptr;
}



void* ArrayPool::malloc (size_t size)
{
    if (!_current)
        extend();

    if (size > _current->_bytes) {
        while (_current->_bytes < size) {
            _current->_bytes += _val_magn;
        }
        _current			= realoc_block(_current, _current->_bytes);
    }

    void* ret				= _get_block(_current);
    _current->_isOccupied	= true;
    _current				= _current->_next;

    return ret;
}



void* ArrayPool::realloc (void* ptr, size_t size)
{
    if (!ptr)
        return malloc(size);

    Node* cur			= _get_node(ptr);

    if (!cur->_isOccupied)
        return malloc(size);

    if (cur->_bytes < size)
    {
        size_t new_size = cur->_bytes;
        while (new_size < size)
            new_size   += _val_magn;

        Node* temp		= cur;
        cur				= realoc_block(cur, new_size, false);
        cur->_isOccupied = true;

        memcpy	(_get_block(cur), _get_block(temp), temp->_bytes);
        ::free		(temp);

        return	_get_block(cur);
    }

    return ptr;
}



void ArrayPool::free (void* ptr)
{
    if (!ptr)
        return;

    Node* cur			= _get_node(ptr);
    cur->_isOccupied	= false;

    Node* prev			= cur->_prev;
    Node* next			= cur->_next;

    if (prev)
        prev->_next		= next;
    if (next)
        next->_prev		= prev;

    if (_first == cur)
        _first			= next;

    if (_last != cur) {
        _last->_next	= cur;
        cur->_prev		= _last;
        cur->_next		= nullptr;
        _last			= cur;
    }
}



ArrayPool::Node* ArrayPool::realoc_block(Node* ptr, size_t size, bool free_ptr)
{
    Node*	prev		= ptr->_prev;
    Node*	next		= ptr->_next;
    bool	needFirst	= false;
    bool	needLast	= false;

    if (ptr == _first)
        needFirst		= true;
    if (ptr == _last)
        needLast		= true;

    if (free_ptr)
        ::free(ptr);

    ptr					= _node_malloc(size);

    ASSERT(ptr != nullptr, "out of memory");

    ptr->_bytes			= size;
    ptr->_isOccupied	= false;

    if (prev) {
        prev->_next		= ptr;
        ptr->_prev		= prev;
    }
    else {
        ptr->_prev		= nullptr;
    }

    if (next) {
        next->_prev		= ptr;
        ptr->_next		= next;
    }
    else {
        ptr->_next		= nullptr;
    }

    if (needFirst)
        _first			= ptr;
    if (needLast)
        _last			= ptr;

    return ptr;
}



void ArrayPool::extend()
{
    _last->_next		= _node_malloc(_val_start);
    Node* cur			= _current = _last->_next;

    _node_init(cur);

    cur->_prev			= _last;

    size_t end				= (_count * 2U) - _count - 1U;
    for (size_t i = 0; i < end; ++i)
    {
        cur->_next		= _node_malloc(_val_start);

        ASSERT		(cur->_next != nullptr, "out of memory");
        _node_init	(cur->_next);

        cur->_next->_prev = cur;
        cur				= cur->_next;
    }

    _last				= cur;
    cur->_next			= nullptr;
    _count			   *= 2U;
}




void ArrayPool::_infoMemoryBlocks()
{
    Node* cur			= _first;
    size_t count		= 0;
    size_t bytes		= 0;
    size_t occupied_c	= 0;
    size_t occupied_b	= 0;

    while (cur) {
        ++count;
        bytes		   += cur->_bytes;

        char	msg[30];

        if (cur->_isOccupied)
        {
            sprintf	(&msg[0], "\t    OCCUPIED");
            ++occupied_c;
            occupied_b += cur->_bytes;
        }
        else {
            sprintf	(&msg[0], "\tnot occupied");
        }

        std::string msginfo;


        if (_first == cur)
            msginfo		= "FIRST  ";
        else if (_last == cur)
            msginfo		= "LAST  ";

        if (_current == cur)
            msginfo	   += "CURRENT";


        Log("memory block [%06i][%p]: %6ibytes, %s	prev[%p], next[%p]  %s", count - 1, _get_block(cur), cur->_bytes, msg, _get_block(cur->_prev), _get_block(cur->_next), msginfo.c_str());

        cur				= cur->_next;
    }


    char totalBytes		[40U];
    char totalBytesOccup[40U];
    _toMaxPrefix	(totalBytesOccup, occupied_b);
    _toMaxPrefix	(totalBytes, bytes);

    Log("TOTAL BLOCKS:\noccupied: %i/%s   all: %i/%s\n", occupied_c, totalBytesOccup, count, totalBytes);
}

void ArrayPool::_sizeofNode() {
    Log("sizeof Node = %i\n Node._prev = %i\n Node._next = %i\n Node._bytes = %i\n Node._isOccupied = %i\n Node._dummy0 = %i\n Node._dummy1 = %i",
        sizeof(Node), sizeof(Node*), sizeof(Node*), sizeof(size_t), sizeof(bool), sizeof(bool), sizeof(U16));
}