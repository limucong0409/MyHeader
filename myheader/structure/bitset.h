#include <cstddef>
#include <algorithm>
#include <cstring>
#include <stdexcept>
namespace MySTL
{
    template<size_t _size> class bitset
    {
    private:
        typedef unsigned long long _Chunk;
        static constexpr size_t _chunk_count=(_size+63)/64;
        _Chunk _data[_chunk_count];
        static constexpr size_t word_index(size_t pos) noexcept{return pos/64;}
        static constexpr size_t bit_offset(size_t pos) noexcept{return pos%64;}
    public:
        bitset(){for(size_t i=0;i<_chunk_count;++i)_data[i]=0;}

        class Reference
        {
        private:
            friend class bitset;
            _Chunk* _ptr;
            size_t _mask;
            Reference(_Chunk* ptr, size_t offset) noexcept:_ptr(ptr),_mask(1ULL<<offset){}
        public:
            Reference& operator=(bool x) noexcept
            {
                *_ptr=(*_ptr&~_mask)|(static_cast<_Chunk>(x)*_mask);
                return *this;
            }
            Reference& operator=(const Reference& other) noexcept
            {
                other?*_ptr|=_mask:*_ptr&=~_mask;
                return *this;
            }
            operator bool() const noexcept{return(*_ptr&_mask)!=0;}
            Reference& flip() noexcept
            {
                *_ptr^=_mask;
                return *this;
            }
            bool operator~()const noexcept{return !(*this);}
        };

        Reference operator[](size_t pos) noexcept{return Reference(&_data[word_index(pos)],bit_offset(pos));}
        bool operator[](size_t pos) const noexcept{return (_data[word_index(pos)]&(1ULL<<bit_offset(pos)))!=0;}
        bitset& set() noexcept
        {
            for (size_t i=0;i<_chunk_count;++i)_data[i]=~0ULL;
            return *this;
        }
        bitset& reset() noexcept
        {
            std::memset(_data,0,sizeof(_data));
            return *this;
        }
        bitset& flip() noexcept
        {
            for(size_t i=0;i<_chunk_count;++i)
                _data[i]=~_data[i];
            return *this;
        }
        bitset& operator=(const bitset& _other) noexcept
        {
            if(this!=&_other)
                std::memcpy(_data,_other._data,sizeof(_data));
            return *this;
        }
        bool operator==(const bitset& _other) const noexcept
        {
            for(size_t i=0;i<_chunk_count;++i)
                if(_data[i]!=_other._data[i])return false;
            return true;
        }
        bool operator!=(const bitset& _other) const noexcept
        {
            return !(*this==_other);
        }
        size_t count() const noexcept
        {
            size_t cnt=0;
            for (size_t i=0;i<_chunk_count;++i)
                cnt+=__builtin_popcountll(_data[i]);
            return cnt;
        }
        bool any() const noexcept
        {
            for(size_t i=0;i<_chunk_count;++i)
                if(_data[i]!=0)return true;
            return false;
        }
        bool none() const noexcept{return !any();}
        bool all() const noexcept
        {
            constexpr size_t full_chunks=_size / 64;
            constexpr size_t remaining=_size % 64;
            constexpr _Chunk last_mask=remaining?((1ULL<<remaining)-1):~0ULL;
            for(size_t i=0;i<full_chunks;++i)
                if (_data[i]!=~0ULL)return false;
            return remaining==0||(_data[_chunk_count-1]&last_mask)==last_mask;
        }
        bitset& operator&=(const bitset& rhs) noexcept
        {
            for (size_t i=0;i<_chunk_count;++i)
                _data[i]&=rhs._data[i];
            return *this;
        }
        bitset& operator|=(const bitset& rhs) noexcept
        {
            for(size_t i=0;i<_chunk_count;++i)
                _data[i]|=rhs._data[i];
            return *this;
        }
        bitset& operator^=(const bitset& rhs) noexcept
        {
            for(size_t i=0;i<_chunk_count;++i)
                _data[i]^=rhs._data[i];
            return *this;
        }
        bitset operator&(const bitset& rhs) const noexcept
        {
            bitset temp=*this;
            temp&=rhs;
            return temp;
        }
        bitset operator|(const bitset& rhs) const noexcept
        {
            bitset temp=*this;
            temp|=rhs;
            return temp;
        }
        bitset operator^(const bitset& rhs) const noexcept
        {
            bitset temp=*this;
            temp^=rhs;
            return temp;
        }
        bitset operator~() const noexcept
        {
            bitset temp=*this;
            temp.flip();
            return temp;
        }
        bitset& operator<<=(size_t step) noexcept
        {
            if(step>=_size){reset();return *this;}
            if(step==0)return *this;
            size_t chunk_shift=step/64;
            size_t bit_shift=step%64;
            if (bit_shift==0)
                for(size_t i=_chunk_count-1;i>=chunk_shift;--i)
                    _data[i]=_data[i - chunk_shift];
            else
            {
                for(size_t i=_chunk_count-1;i>chunk_shift;--i)
                    _data[i]=(_data[i-chunk_shift]<<bit_shift)
                            |(_data[i-chunk_shift-1]>>(64-bit_shift));
                _data[chunk_shift] = _data[0] << bit_shift;
            }
            for (size_t i=0;i<chunk_shift;++i)
                _data[i]=0;
            return *this;
        }
        bitset& operator>>=(size_t step) noexcept
        {
            if(step>=_size){reset();return *this;}
            if(step==0)return *this;
            size_t chunk_shift=step/64;
            size_t bit_shift=step%64;
            if(bit_shift==0)
                for(size_t i=0;i<_chunk_count-chunk_shift;++i)
                    _data[i]=_data[i+chunk_shift];
            else
            {
                for(size_t i=0;i<_chunk_count-chunk_shift-1;++i)
                    _data[i]=(_data[i+chunk_shift]>>bit_shift)
                            |(_data[i+chunk_shift+1]<<(64-bit_shift));
                _data[_chunk_count-chunk_shift-1]=_data[_chunk_count-1]>>bit_shift;
            }
            for(size_t i=_chunk_count-chunk_shift;i<_chunk_count;++i)
                _data[i] = 0;
            return *this;
        }
        bitset operator<<(size_t step) const noexcept
        {
            bitset temp=*this;
            temp<<=step;
            return temp;
        }bitset operator>>(size_t step) const noexcept
        {
            bitset temp=*this;
            temp>>=step;
            return temp;
        }
        void _ior_shifted(size_t step) noexcept
        {
            if (step>=_size) return;
            if (step==0) return;
            size_t chunk_shift=step/64;
            size_t bit_shift=step%64;
            if(bit_shift==0)
                for (size_t i=_chunk_count-1;i>=chunk_shift;--i)
                    _data[i]|=_data[i-chunk_shift];
            else
            {
                // 从后往前计算，避免覆盖未读取的旧数据
                for (size_t i=_chunk_count-1;i>chunk_shift;--i)
                {
                    _Chunk shifted=(_data[i-chunk_shift]<<bit_shift) 
                                   |(_data[i-chunk_shift-1]>>(64-bit_shift));
                    _data[i]|=shifted;
                }
                _data[chunk_shift]|=_data[0]<<bit_shift;
            }
        }
    };
}