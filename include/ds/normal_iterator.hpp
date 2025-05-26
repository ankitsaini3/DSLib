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


    template<typename Iter>
    NormalIterator(const NormalIterator<Iter, Container>& other,
     std::enable_if_t< std::is_convertible_v<Iter, IteratorType>, int > = 0) : current(other.base()) {}


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

    IteratorType base() const { return current; }

private:
    IteratorType current;
};

template <typename Iter1, typename Iter2, typename Container>
bool operator!=(const NormalIterator<Iter1, Container>& lhs, const NormalIterator<Iter2, Container>& rhs) {
    return lhs.base() != rhs.base();
}

template <typename Iter1, typename Iter2, typename Container>
bool operator==(const NormalIterator<Iter1, Container>& lhs, const NormalIterator<Iter2, Container>& rhs) {
    return lhs.base() == rhs.base();
}

template <typename Iter1, typename Iter2, typename Container>
bool operator<(const NormalIterator<Iter1, Container>& lhs, const NormalIterator<Iter2, Container>& rhs) {
    return lhs.base() < rhs.base();
}

template <typename Iter1, typename Iter2, typename Container>
bool operator>(const NormalIterator<Iter1, Container>& lhs, const NormalIterator<Iter2, Container>& rhs) {
    return lhs.base() > rhs.base();
}

template <typename Iter1, typename Iter2, typename Container>
bool operator<=(const NormalIterator<Iter1, Container>& lhs, const NormalIterator<Iter2, Container>& rhs) {
    return lhs.base() <= rhs.base();
}

template <typename Iter1, typename Iter2, typename Container>
bool operator>=(const NormalIterator<Iter1, Container>& lhs, const NormalIterator<Iter2, Container>& rhs) {
    return lhs.base() >= rhs.base();
}
