#pragma once

#include <iterator>

template<typename IteratorType, typename Container>
class NormalIterator {
public:
    using iterator_type     = IteratorType;
    using iterator_category = std::random_access_iterator_tag;
    using value_type        = typename std::iterator_traits<IteratorType>::value_type;
    using difference_type   = typename std::iterator_traits<IteratorType>::difference_type;
    using pointer           = typename std::iterator_traits<IteratorType>::pointer;
    using reference         = typename std::iterator_traits<IteratorType>::reference;

    NormalIterator() : current() {}
    explicit NormalIterator(const IteratorType& it) : current(it) {}

    reference operator*() const { return *current; }
    pointer operator->() const { return current; }

    NormalIterator& operator++() { ++current; return *this; }
    NormalIterator operator++(int) { NormalIterator tmp = *this; ++current; return tmp; }

    NormalIterator& operator--() { --current; return *this; }
    NormalIterator operator--(int) { NormalIterator tmp = *this; --current; return tmp; }

    NormalIterator& operator+=(difference_type n) { current += n; return *this; }
    NormalIterator operator+(difference_type n) const { return NormalIterator(current + n); }

    NormalIterator& operator-=(difference_type n) { current -= n; return *this; }
    NormalIterator operator-(difference_type n) const { return NormalIterator(current - n); }

    difference_type operator-(const NormalIterator& other) const { return current - other.current; }

    reference operator[](difference_type n) const { return current[n]; }

    bool operator==(const NormalIterator& other) const { return current == other.current; }
    bool operator!=(const NormalIterator& other) const { return current != other.current; }
    bool operator<(const NormalIterator& other) const { return current < other.current; }
    bool operator<=(const NormalIterator& other) const { return current <= other.current; }
    bool operator>(const NormalIterator& other) const { return current > other.current; }
    bool operator>=(const NormalIterator& other) const { return current >= other.current; }

    IteratorType base() const { return current; }

private:
    IteratorType current;
};
