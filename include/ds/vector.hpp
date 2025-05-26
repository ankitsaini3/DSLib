#pragma once

#include <cstddef>
#include <utility>
#include <iostream>
#include <new>
#include <initializer_list>

#include "normal_iterator.hpp"

namespace ds
{
    template <typename T>
    class vector
    {

    public:
        using value_type = T;
        using pointer = T *;
        using const_pointer = const T *;
        using reference = T &;
        using const_reference = const T &;
        using size_type = std::size_t;

        using iterator = NormalIterator<pointer, vector>;
        using const_iterator = NormalIterator<const_pointer, vector>;

        // constructors
        vector() noexcept;
        explicit vector(size_type _count);
        vector(size_type count, const T &_value);
        vector(const vector &_other);
        vector(vector &&_temp) noexcept;
        vector(std::initializer_list<T> _li);

        // destructors
        ~vector() noexcept;

        // operator=
        vector &operator=(const vector<T> &_other);
        vector &operator=(vector<T> &&_other) noexcept;

        // element access
        reference at(size_type _index);
        const_reference at(size_type _index) const;

        reference operator[](size_type _index) { return *(array + _index); };
        const_reference operator[](size_type _index) const { return *(array + _index); };

        reference front() { return *array; };
        const_reference front() const { return *array; };

        reference back() { return *(array + vectorSize - 1); };
        const_reference back() const { return *(array + vectorSize - 1); };

        // iterators
        iterator begin() { return iterator(array); }
        const_iterator begin() const { return const_iterator(array); }

        iterator end() { return iterator(array + vectorSize); }
        const_iterator end() const { return const_iterator(array + vectorSize); }

        const_iterator cbegin() const { return const_iterator(array); }
        const_iterator cend() const { return const_iterator(array + vectorSize); }

        // capacity
        size_type size() { return vectorSize; }
        size_type capacity() { return reservedSize; }
        bool empty() { return vectorSize == 0; };
        void reserve(size_type new_cap);
        void shrink_to_fit();

        // modifiers
        void clear();
        iterator insert(const_iterator _position, const T &_value);
        iterator insert(const_iterator _position, T &&_value);
        iterator insert(const_iterator _position, size_type _count, const T &_value_);
        iterator insert(const_iterator _position, const std::initializer_list<T> _li);

        template <typename... Args>
        iterator emplace(const_iterator _position, Args &&...args);

        void push_back(const T &_value);
        void push_back(T &&_value);
        void pop_back();

    private:
        pointer array;

        size_type reservedSize = 0;
        size_type vectorSize = 0;

        inline void reallocate();
    };

    template <typename T>
    void vector<T>::reallocate()
    {
        pointer tempArray = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

        for (size_type i = 0; i < vectorSize; i++)
        {
            new (tempArray + i) T(std::move(array[i]));
        }

        for (size_type i = 0; i < vectorSize; i++)
        {
            array[i].~T();
        }

        ::operator delete(array);

        array = tempArray;
    }

    // default constructor
    template <typename T>
    vector<T>::vector() noexcept
    {
        std::cout << "default constructor called " << this << "\n";
        array = static_cast<T *>(::operator new(reservedSize * sizeof(T)));
    }

    // default destructor
    template <typename T>
    vector<T>::~vector() noexcept
    {
        std::cout << "default destructor called" << this << "\n";

        for (size_type i = 0; i < vectorSize; ++i)
        {
            array[i].~T();
        }

        ::operator delete(array);
    }

    // parameterised constructor
    template <typename T>
    vector<T>::vector(size_type _count) : reservedSize(_count),
                                          vectorSize(_count)
    {
        std::cout << "parameterized constructor (size_type n) called " << this << "\n";

        array = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

        for (size_type i = 0; i < vectorSize; i++)
        {
            new (array + i) T();
        }
    }

    // parameterised constructor
    template <typename T>
    vector<T>::vector(size_type _count, const T &_value) : reservedSize(_count),
                                                           vectorSize(_count)
    {
        std::cout << "parameterized constructor (size_type n, const T& value) called\n";

        array = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

        for (size_type i = 0; i < vectorSize; i++)
        {
            new (array + i) T(_value);
        }
    }

    // initializer list constructor
    template <typename T>
    vector<T>::vector(std::initializer_list<T> _li) : reservedSize(_li.size())
    {

        std::cout << "initializer list constructor called" << this << "\n";

        array = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

        for (typename std::initializer_list<T>::const_iterator it = _li.begin(); it != _li.end(); it++)
        {
            new (array + vectorSize) T(*it);
            ++vectorSize;
        }
    }

    // copy constructor
    template <typename T>
    vector<T>::vector(const vector<T> &_other) : reservedSize(_other.reservedSize),
                                                 vectorSize(_other.vectorSize)
    {

        std::cout << "copy constructor called " << this << "\n";

        array = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

        for (size_type i = 0; i < vectorSize; ++i)
        {
            new (array + i) T(_other[i]);
        }
    }

    // move constructor
    template <typename T>
    vector<T>::vector(vector<T> &&_temp) noexcept : array(_temp.array),
                                                    reservedSize(_temp.reservedSize),
                                                    vectorSize(_temp.vectorSize)
    {
        std::cout << "move constructor called\n";
        _temp.array = nullptr;
        _temp.vectorSize = _temp.reservedSize = 0;
    }

    // copy assignment
    template <typename T>
    vector<T> &vector<T>::operator=(const vector<T> &_other)
    {

        std::cout << "copy assignment called\n";

        if (this == &_other)
        {
            return *this;
        }

        if (vectorSize < _other.vectorSize)
        {

            for (size_type i = 0; i < vectorSize; i++)
            {
                array[i].~T();
            }

            ::operator delete(array);

            T *tempArray = static_cast<T *>(::operator new(_other.reservedSize * sizeof(T)));

            for (size_type i = 0; i < _other.vectorSize; i++)
            {
                new (tempArray + i) T(_other[i]);
            }

            array = tempArray;

            reservedSize = _other.reservedSize;
            vectorSize = _other.vectorSize;
        }
        else
        {
            for (size_type i = 0; i < _other.vectorSize; i++)
            {
                array[i] = _other[i];
            }

            for (size_type i = _other.vectorSize; i < vectorSize; i++)
            {
                array[i].~T();
            }

            vectorSize = _other.vectorSize;
            reservedSize = _other.reservedSize;
        }

        return *this;
    }

    // move assignment
    template <typename T>
    vector<T> &vector<T>::operator=(vector<T> &&_other) noexcept
    {
        std::cout << "Move assignment called\n";

        if (this == &_other)
        {
            return *this;
        }

        for (size_type i = 0; i < reservedSize; ++i)
        {
            array[i].~T();
        }

        ::operator delete(array);

        array = _other.array;
        vectorSize = _other.vectorSize;
        reservedSize = _other.reservedSize;

        _other.array = nullptr;
        _other.vectorSize = 0;
        _other.reservedSize = 0;

        return *this;
    }

    template <typename T>
    typename vector<T>::reference vector<T>::at(size_type _index)
    {
        if (_index >= vectorSize)
        {
            throw std::out_of_range("ds::vector - index is out of bounds");
        }
        return *(array + _index);
    }

    template <typename T>
    typename vector<T>::const_reference vector<T>::at(size_type _index) const
    {
        if (_index >= vectorSize)
        {
            throw std::out_of_range("ds::vector - index is out of bounds");
        }
        return *(array + _index);
    }

    template <typename T>
    void vector<T>::push_back(const T &_value)
    {
        if (reservedSize == 0)
        {
            reservedSize = 1;
            reallocate();
        }
        else if (vectorSize == reservedSize)
        {
            reservedSize = reservedSize * 2;
            reallocate();
        }

        new (array + vectorSize) T(_value);
        vectorSize++;
    }

    template <typename T>
    void vector<T>::push_back(T &&_value)
    {
        if (reservedSize == 0)
        {
            reservedSize = 1;
            reallocate();
        }
        else if (vectorSize == reservedSize)
        {
            reservedSize = reservedSize * 2;
            reallocate();
        }

        new (array + vectorSize) T(std::move(_value));
        vectorSize++;
    }

    template <typename T>
    void vector<T>::pop_back()
    {
        if (vectorSize != 0)
        {
            --vectorSize;
            array[vectorSize].~T();
        }
    }

    template <typename T>
    void vector<T>::clear()
    {
        for (size_type i = 0; i < vectorSize; i++)
        {
            array[i].~T();
        }

        vectorSize = 0;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator _position, const T &_value)
    {
        std::cout << "inside insert\n";

        const size_type insert_index = _position.base() - array;
        iterator pos = begin() + insert_index; // convert to non-const iterator

        if (vectorSize < reservedSize)
        {
            if (vectorSize != 0)
            {
                new (array + vectorSize) T(std::move(array[vectorSize - 1]));
            }

            for (size_t i = vectorSize - 1; i - 1 >= insert_index; i--)
            {
                array[i] = std::move(array[i - 1]);
            }

            new (pos.base()) T(_value);

            vectorSize++;
        }

        if (vectorSize == reservedSize)
        {
            if (reservedSize == 0)
            {
                reservedSize = 1;
            }
            else
            {
                reservedSize = reservedSize * 2;
            }

            T *tempArray = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

            for (size_type i = 0; i < insert_index; i++)
            {
                new (tempArray + i) T(std::move(array[i]));
            }

            new (tempArray + insert_index) T(_value);

            for (size_type i = insert_index; i < vectorSize; i++)
            {
                new (tempArray + i + 1) T(std::move(array[i]));
            }

            for (size_t i = 0; i < vectorSize; i++)
            {
                array[i].~T();
            }

            ::operator delete(array);

            array = tempArray;

            vectorSize++;
        }

        pos = begin() + insert_index;

        return pos;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator _position, T &&_value)
    {
        const size_type insert_index = _position.base() - array;
        iterator pos = begin() + insert_index; // convert to non-const iterator

        if (vectorSize < reservedSize)
        {
            if (vectorSize != 0)
            {
                new (array + vectorSize) T(std::move(array[vectorSize - 1]));
            }

            for (size_t i = vectorSize - 1; i - 1 >= insert_index; i--)
            {
                array[i] = std::move(array[i - 1]);
            }

            new (pos.base()) T(std::move(_value));

            vectorSize++;
        }

        if (vectorSize >= reservedSize)
        {

            if (reservedSize == 0)
            {
                reservedSize = 1;
            }
            else
            {
                reservedSize = reservedSize * 2;
            }

            T *tempArray = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

            for (size_type i = 0; i < insert_index; i++)
            {
                new (tempArray + i) T(std::move(array[i]));
            }

            new (tempArray + insert_index) T(std::move(_value));

            for (size_type i = insert_index; i < vectorSize; i++)
            {
                new (tempArray + i + 1) T(std::move(array[i]));
            }

            for (size_t i = 0; i < vectorSize; i++)
            {
                array[i].~T();
            }

            ::operator delete(array);

            array = tempArray;

            vectorSize++;
        }

        pos = begin() + insert_index;

        return pos;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator _position, size_type _count, const T &_value)
    {
        const size_type insert_index = _position.base() - array;
        iterator pos = begin() + insert_index; // convert to non-const iterator

        if (vectorSize + _count - 1 < reservedSize)
        {
            for (size_t i = vectorSize - 1 + _count; i - _count >= insert_index; i--)
            {
                array[i] = T(std::move(array[i - _count]));
            }

            for (size_t i = 0; i < _count; i++)
            {
                new (pos.base()) T(_value);
                pos++;
            }
        }

        if (vectorSize + _count - 1 >= reservedSize)
        {
            reservedSize = vectorSize + _count;

            T *tempArray = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

            for (size_type i = 0; i < insert_index; i++)
            {
                new (tempArray + i) T(std::move(array[i]));
            }

            for (size_t i = 0; i < _count; i++)
            {
                new (tempArray + insert_index + i) T(_value);
            }

            for (size_type i = insert_index; i < vectorSize; i++)
            {
                new (tempArray + i + _count) T(std::move(array[i]));
            }

            for (size_t i = 0; i < vectorSize; i++)
            {
                array[i].~T();
            }

            ::operator delete(array);

            array = tempArray;
        }

        vectorSize = vectorSize + _count;

        pos = begin() + insert_index;

        return pos;
    }

    template <typename T>
    typename vector<T>::iterator vector<T>::insert(vector<T>::const_iterator _position, const std::initializer_list<T> _li)
    {
        const size_type insert_index = _position.base() - array;
        iterator pos = begin() + insert_index; // convert to non-const iterator
        size_t count = _li.size();

        if (vectorSize + count - 1 < reservedSize)
        {
            for (size_t i = vectorSize - 1 + count; i - count >= insert_index; i--)
            {
                array[i] = T(std::move(array[i - count]));
            }

            for (typename std::initializer_list<T>::const_iterator it = _li.begin(); it != _li.end(); it++)
            {
                new (pos.base()) T(std::move(*it));
                pos++;
            }
        }

        if (vectorSize + count - 1 >= reservedSize)
        {

            reservedSize = reservedSize * 2;

            T *tempArray = static_cast<T *>(::operator new(reservedSize * sizeof(T)));

            for (size_type i = 0; i < insert_index; i++)
            {
                new (tempArray + i) T(std::move(array[i]));
            }

            for (size_t i = 0; i < count; i++)
            {
                new (tempArray + insert_index + i) T(std::move(*(_li.begin() + i)));
            }

            for (size_type i = insert_index; i < vectorSize; i++)
            {
                new (tempArray + i + count) T(std::move(array[i]));
            }

            for (size_t i = 0; i < vectorSize; i++)
            {
                array[i].~T();
            }

            ::operator delete(array);

            array = tempArray;
        }

        vectorSize = vectorSize + count;

        pos = begin() + insert_index;

        return pos;
    }

    template <typename T>
    template <typename... Args>
    typename vector<T>::iterator vector<T>::emplace(const_iterator _position, Args &&...args)
    {
        const size_type insert_index = _position.base() - array;
        iterator pos = begin() + insert_index;

        T tempObj(std::forward<Args>(args)...);

        if (vectorSize == reservedSize)
        {
            if (reservedSize == 0)
            {
                reservedSize = 1;
            }
            else
            {
                reservedSize = reservedSize * 2;
            }

            reallocate();
        }

        if (insert_index <= vectorSize)
        {
            // construct element at some other place then move assign it at _position

            for (size_type i = vectorSize; i - 1 >= insert_index; --i)
            {
                array[i] = std::move(array[i - 1]);
            }

            new (array + insert_index) T(std::move(tempObj));
        }

        vectorSize++;
        pos = begin() + insert_index;

        return pos;
    }

}