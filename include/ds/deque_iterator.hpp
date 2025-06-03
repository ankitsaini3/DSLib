#include <cstddef>
#include <iterator>
#include <type_traits>

namespace ds
{

#define DEQUE_BUF_SIZE 512

    template <typename T, typename PTR, typename REF>
    class DequeIterator
    {
    public:
        using iterator_type = T;
        using value_type = T;
        using pointer = PTR;
        using reference = REF;
        using iterator_category = std::random_access_iterator_tag;
        using difference_type = typename std::iterator_traits<PTR>::difference_type;

        DequeIterator();

        DequeIterator(T *x, T **y);

        DequeIterator(const DequeIterator& it);

        reference operator*() const { return *current; }
        pointer operator->() const { return current; }

        DequeIterator &operator++();
        DequeIterator operator++(int);

        DequeIterator &operator--();
        DequeIterator operator--(int);

        DequeIterator &operator+=(difference_type n);
        DequeIterator &operator-=(difference_type n);

        friend DequeIterator operator+(const DequeIterator it, difference_type n)
        {
            DequeIterator temp = it;
            temp += n;
            return temp;
        }

        friend DequeIterator operator-(const DequeIterator it, difference_type n)
        {
            DequeIterator temp = it;
            temp -= n;
            return temp;
        }

        size_t operator-(const DequeIterator& it) const
        {
            if (node == it.node)
            {
                return current - it.current;
            }

            size_t nodes_in_between = (node - it.node);
            nodes_in_between--;

            
            return (it.last - it.current) + nodes_in_between * 128 + (current - first);
        }

        friend bool operator==(const DequeIterator& first, const DequeIterator &it)
        {
            return first.current == it.current;
        }

        friend bool operator!=(const DequeIterator& first, const DequeIterator &it)
        {
            return !(first == it);
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        T* base() const { return current; }

    private:
        T *current;
        T *first;
        T *last;
        T **node;

        void setNode(T **newNode)
        {
            node = newNode;
            first = *node;
            last = first + deque_block_size();
        }

        inline size_t deque_block_size()
        {
            return ( sizeof(T) < DEQUE_BUF_SIZE ) ?  size_t(DEQUE_BUF_SIZE / sizeof(T)) : size_t(1);
        }
    };

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF> &DequeIterator<T, PTR, REF>::operator++()
    {
        ++current;

        if (current == last)
        {
            setNode(node + 1);
            current = first;
        }

        return *this;
    }

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF> DequeIterator<T, PTR, REF>::operator++(int)
    {
        DequeIterator temp = *this;

        ++(*this);

        return temp;
    }

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF> &DequeIterator<T, PTR, REF>::operator--()
    {
        if (current == first)
        {
            setNode(node - 1);
            current = last;
        }
        --current;

        return *this;
    }

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF> DequeIterator<T, PTR, REF>::operator--(int)
    {
        DequeIterator temp = *this;

        --(*this);

        return temp;
    }

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF> &DequeIterator<T, PTR, REF>::operator+=(difference_type n)
    {
        const difference_type offset = n + (current - first);

        if (offset >= 0 && offset < difference_type(deque_block_size()))
        {
            current += n;
        }
        else
        {
            const difference_type node_offset =
                offset > 0 ? offset / difference_type(deque_block_size())
                           : -difference_type((-offset - 1) / deque_block_size()) - 1;

            setNode(node + node_offset);
            current = first + (offset - node_offset * difference_type(deque_block_size()));
        }

        return *this;
    }

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF>::DequeIterator() : 
                                                  current{nullptr},
                                                  first{nullptr},
                                                  last{nullptr},
                                                  node{nullptr}
    {
    }

    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF>::DequeIterator(T *current_, T **mapPointer_): 
                                                             current{current_},
                                                             first{*mapPointer_},
                                                             last{*mapPointer_ + deque_block_size()},
                                                             node{mapPointer_}
    {
    }




    template <typename T, typename PTR, typename REF>
    DequeIterator<T, PTR, REF>::DequeIterator(const DequeIterator& it):
                                                             current{it.current},
                                                             first{it.first},
                                                             last{it.last},
                                                             node{it.node}
    {
    }

}