#pragma once 

#include <cstddef>
#include <utility>
#include <iostream>
#include <new>
#include <initializer_list>

#include "deque_iterator.hpp"

namespace ds
{
    template<typename T>
    class deque
    {
    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;
        using size_type = std::size_t;

        using iterator = DequeIterator<value_type, pointer, reference>;
        using const_iterator = DequeIterator<value_type, const_pointer, const_reference>;

        // constructors
        deque();
        deque(size_type count);
        deque(size_type count, const T& value);
        deque(const deque& other);
        deque(deque&& other) noexcept;
        deque(std::initializer_list<T> li);



        // destructors
        ~deque() noexcept;



        // assignment
        deque& operator=(const deque& other);
        deque& operator=(deque&& other) noexcept;



        // element access
        reference at(size_type index);
        const_reference at(size_type index) const;

        reference operator[](size_type index);
        const_reference operator[](size_type index) const;

        reference front();
        const_reference front() const;

        reference back();
        const_reference back() const;


        // iterators
        iterator begin() { return start_;}
        const_iterator begin() const { return const_iterator(start_);}
        iterator end() { return end_;}
        const_iterator end() const { return const_iterator(end_);};



        // capacity
        bool empty() const { return start_ == end_;}
        const size_type size() const { return end_ - start_;}




    private:
        T** map;
        iterator start_;
        iterator end_;
        size_t mapSize;


        constexpr inline size_t deque_block_size()
        {
            return sizeof(T) < DEQUE_BUF_SIZE ? size_t(DEQUE_BUF_SIZE / sizeof(T)) : size_t(1);
        }


        T** allocateMap(size_t mapSize_)
        {
            return reinterpret_cast<T**>(new char[sizeof(T*) * mapSize_]);
        }

        void deallocateMap()
        {
            delete [] reinterpret_cast<char*>(map); 
        }

        T* allocateBlock()
        {
            return reinterpret_cast<T*>(new char[sizeof(T) * deque_block_size()]);
        }

        void deallocateBlock(size_t blockIndex)
        {
            delete[] reinterpret_cast<char*>(map[blockIndex]);
        }

    };





    
    template<typename T>
    deque<T>::
    deque():
        map{},
        start_{},
        end_{},
        mapSize{0}
    {

    }



    template<typename T>
    deque<T>::
    deque(size_type count)
    {
        size_t blockSize = deque_block_size();
        size_t numberOfBlocks = count / blockSize + 1;

        mapSize = numberOfBlocks;

        map = allocateMap(mapSize);

        for (size_t i = 0; i < numberOfBlocks; i++)
        {
            map[i] = allocateBlock();
        }

        start_ = iterator(&map[0][0], map);
        end_ = start_ + count;

        for(iterator it = start_; it != end_; ++it)
        {
            new (it.base()) T(); 
        }
        
    }



    template<typename T>
    deque<T>::
    deque(size_type count, const T& value)
    {
        size_t blockSize = deque_block_size();
        size_t numberOfBlocks = count / blockSize + 1;

        mapSize = numberOfBlocks;

        map = allocateMap(mapSize);

        for (size_t i = 0; i < numberOfBlocks; i++)
        {
            map[i] = allocateBlock();
        }

        start_ = iterator(&map[0][0], map);
        end_ = start_ + count;

        for(iterator it = start_; it != end_; ++it)
        {
            new (it.base()) T(value); 
        }
    }


    template<typename T>
    deque<T>::
    deque(const deque& other)
    {
        mapSize = other.mapSize;

        map = allocateMap(mapSize);

        for (size_t i = 0; i < mapSize; i++)
        {
            map[i] = allocateBlock();
        }

        start_ = iterator(&map[0][0], map);
        end_ = start_ + other.size();
        size_t i = 0;

        for(iterator it = start_; it != end_; ++it)
        {
            new (it.base()) T(other[i++]); 
        }
        
    }


    template<typename T>
    deque<T>::
    deque(deque&& other) noexcept
    {
        if (&other == *this)
        {
            return ;
        }

        mapSize = other.mapSize;
        start_ = other.start_;
        end_ = other.end_;
        map = other.map;

        other.mapSize = 0;
        other.start_ = iterator();
        other.end_ = iterator();
        other.map = nullptr;
    }


    template<typename T>
    deque<T>::
    deque(std::initializer_list<T> li)
    {
        size_t count = li.size();

        size_t blockSize = deque_block_size();
        size_t numberOfBlocks = count / blockSize + 1;

        mapSize = numberOfBlocks;

        map = allocateMap(mapSize);

        for (size_t i = 0; i < numberOfBlocks; i++)
        {
            map[i] = allocateBlock();
        }

        start_ = iterator(&map[0][0], map);
        end_ = start_ + count;
        size_t i = 0;

        for(iterator it = start_; it != end_; ++it)
        {
            new (it.base()) T(*(li.begin() + i++)); 
        }

    }


    template<typename T>
    deque<T>::
    ~deque() noexcept
    {
        
        for(iterator it = start_; it != end_; ++it)
        {
            (*it).~T();
        }

        for (size_t i = 0; i < mapSize; i++)
        {
            deallocateBlock(i);
        }


        deallocateMap();
        
    }

    template<typename T>
    deque<T>& deque<T>::operator=(const deque& other)
    {
        if (&other == *this)
        {
            return *this;
        }
        

        for(iterator it = start_; it != end_; ++it)
        {
            (*it).~T();
        }

        if (mapSize != other.mapSize)
        {
            if (mapSize < other.mapSize)
            {

                T** tempMap = allocateMap(other.mapSize);

                for (size_t i = 0; i < mapSize; i++)
                {
                    tempMap[i] = map[i];
                }

                for (size_t i = mapSize; i < other.mapSize; i++)
                {
                    tempMap[i] = allocateBlock();
                }

                deallocateMap();

                map = tempMap;
                mapSize = other.mapSize;
            }
            else
            {
                T** tempMap = allocateMap(other.mapSize);

                for (size_t i = 0; i < other.mapSize; i++)
                {
                    tempMap[i] = map[i];
                }

                for (size_t i = other.mapSize; i < mapSize; i++)
                {
                    deallocateBlock(i);
                }
            
                deallocateMap();
            
                map = tempMap;
                mapSize = other.mapSize;  
            }
                
            
        }

        start_ = iterator(&map[0][0], map);
        end_ = start_ + other.size();

        size_t i = 0;
        for(iterator it = start_; it != end_; ++it)
        {
            new (it.base()) T(other[i++]);
        }

        return *this;
    }



    template<typename T>
    deque<T>& deque<T>::operator=(deque&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        mapSize = other.mapSize;
        map = other.map;
        start_ = other.start_;
        end_ = other.end_;

        other.mapSize = 0;
        other.map = nullptr;
        other.start_ = iterator();
        other.end_ = iterator();
    }



    template<typename T>
    typename
    deque<T>::
    reference deque<T>::at(size_type index)
    {
        if (index >= size())
        {
            throw std::runtime_error("deque::size() : index out bound");
        }
        
        return *(start_ + index);
    }


    template<typename T>
    typename
    deque<T>::
    const_reference deque<T>::at(size_type index) const
    {
        if (index >= size())
        {
            throw std::runtime_error("deque::size() : index out bound");
        }

        return *(start_ + index);
    }


    template<typename T>
    typename
    deque<T>::
    reference deque<T>::operator[](size_type index)
    {
        return *(start_ + index);
    }


    template<typename T>
    typename
    deque<T>::
    const_reference deque<T>::operator[](size_type index) const
    {
        return *(start_ + index);
    }


    template<typename T>
    typename
    deque<T>::
    reference deque<T>::front()
    {
        return *start_;
    }


    template<typename T>
    typename
    deque<T>::
    const_reference deque<T>::front() const
    {
        return *start_;
    }



    template<typename T>
    typename
    deque<T>::
    reference deque<T>::back()
    {
        return *(start_ + size() -1);
    }



    template<typename T>
    typename
    deque<T>::
    const_reference deque<T>::back() const
    {
        return *(start_ + size() -1);
    }
}