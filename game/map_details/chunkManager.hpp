#ifndef _CHUNK_MANAGER_HPP_
#define _CHUNK_MANAGER_HPP_
#include "../../base/types.hpp"
#include "../../base/new_types/sharedString.hpp"
#include "../../base/cfgSystem.hpp"

#include <vector>
#include <SFML/System/Vector2.hpp>

class CChunk;
///////////////////// CHUNK MANAGER ///////////////////////////////
//
//
//
////////////////////////////////////////////////////////////////////
class CChunkManager {
public:
    // class
    class CU64DoubleS32 {
    public:
        S32 _x;
        S32 _y;


        CU64DoubleS32(S32 x, S32 y): _x(x), _y(y) {}
        explicit CU64DoubleS32(U64 val) {
            auto* ptr = reinterpret_cast<S32*>(&val);
            _x = *ptr;
            _y = *(ptr + 1);
        }

        U64 asU64() const {
            U64 ret = 0;
            auto* ptr = reinterpret_cast<S32*>(&ret);
            *ptr = _x;
            *(ptr + 1) = _y;
            return ret;
        }

    };


    CChunkManager(const shared_str& map);
    ~CChunkManager();
    CChunk* createChunk		(const sf::Vector2i& chunkNumber) const;
    void	destroyChunk	(CChunk* chunk) const;

protected:

    mutable std::vector<U64> _avaibleChunks;    // created chunks numbers. must be sorted by std::less
    bfs::path				 _mapPath;
    bfs::path				 _mapChunksInfoPath;

    CChunk* generateChunk(const sf::Vector2i& chunkNumber) const;

};

#endif // !_CHUNK_MANAGER_HPP_
