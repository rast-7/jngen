#pragma once

#include <bits/stdc++.h>

#include "common.h"
#include "random.h"
#include "repr.h"

namespace impl {

typedef std::pair<size_t, size_t> Range;

template<typename T>
class GenericArray : public Repr<GenericArray<T>>, public std::vector<T>  {
public:
    typedef std::vector<T> Base;

    using Base::Base;

    GenericArray() {}
    GenericArray(const GenericArray<T>&) = default;
    GenericArray& operator=(const GenericArray<T>&) = default;
    GenericArray(GenericArray<T>&&) = default;
    GenericArray& operator=(GenericArray<T>&&) = default;

    ~GenericArray() {}

    /* implicit */ GenericArray(const Base& base) :
            Base(base)
    {  }

    // TODO(ifsmirnov): 'use' all methods and make inheritance private
    using Base::at;
    using Base::operator[];
    using Base::size;
    using Base::begin;
    using Base::end;

    static GenericArray<T> random(size_t size, T max);
    static GenericArray<T> random(size_t size, T min, T max);
    static GenericArray<T> random(const Range& size, T max);
    static GenericArray<T> random(const Range& size, T min, T max);

    static GenericArray<T> id(size_t size, T start = T{});

    GenericArray<T>& shuffle();
    GenericArray<T> shuffled() const;

    GenericArray<T>& reverse();
    GenericArray<T> reversed() const;

    GenericArray<T>& sort();
    GenericArray<T> sorted() const;

    // TODO(ifsmirnov): think about naming
    GenericArray<T>& add(T value);
    GenericArray<T> added(T value) const;

    template<typename Integer>
    GenericArray<T> subseq(const std::vector<Integer>& indices) const;

    template<typename Integer>
    GenericArray<T> subseq(
        const std::initializer_list<Integer>& indices) const;

    const T& choice() const;
    GenericArray<T> choice(size_t count) const;
    GenericArray<T> choiceWithRepetition(size_t count) const;
};

template<typename T>
GenericArray<T> GenericArray<T>::random(size_t size, T max) {
    GenericArray<T> result(size);
    for (T& x: result) {
        x = rnd.next(max);
    }
    return result;
}

template<typename T>
GenericArray<T> GenericArray<T>::random(size_t size, T min, T max) {
    GenericArray<T> result(size);
    for (T& x: result) {
        x = rnd.next(min, max);
    }
    return result;
}

template<typename T>
GenericArray<T> GenericArray<T>::random(const Range& size, T max) {
    return random(rnd.next(size.first, size.second), max);
}

template<typename T>
GenericArray<T> GenericArray<T>::random(const Range& size, T min, T max) {
    return random(rnd.next(size.first, size.second), min, max);
}

template<typename T>
auto genericArrayIdHelper(size_t size, T start)
    -> typename std::enable_if<
        std::is_integral<T>::value, GenericArray<T>
    >::type
{
    GenericArray<T> result(size);
    std::iota(result.begin(), result.end(), start);
    return result;
}

template<typename T>
auto genericArrayIdHelper(size_t size, const T& start)
    -> typename std::enable_if<
        !std::is_integral<T>::value, GenericArray<T>
    >::type
{
    ensure("Cannot take GenericArray<T>::id() when T is non-integral");
    return {};
}

template<typename T>
GenericArray<T> GenericArray<T>::id(size_t size, T start) {
    return genericArrayIdHelper<T>(size, start);
}

template<typename T>
GenericArray<T>& GenericArray<T>::shuffle() {
    for (size_t i = 1; i < size(); ++i) {
        std::swap(at(i), at(rnd.next(i + 1)));
    }
    return *this;
}

template<typename T>
GenericArray<T> GenericArray<T>::shuffled() const {
    auto res = *this;
    res.shuffle();
    return res;
}

template<typename T>
GenericArray<T>& GenericArray<T>::reverse() {
    std::reverse(begin(), end());
    return *this;
}

template<typename T>
GenericArray<T> GenericArray<T>::reversed() const {
    auto res = *this;
    res.reverse();
    return res;
}

template<typename T>
GenericArray<T>& GenericArray<T>::sort() {
    std::sort(begin(), end());
    return *this;
}

template<typename T>
GenericArray<T> GenericArray<T>::sorted() const {
    auto res = *this;
    res.sort();
    return res;
}

template<typename T>
GenericArray<T>& GenericArray<T>::add(T value) {
    for (T& x: *this) {
        x += value;
    }
    return *this;
}

template<typename T>
GenericArray<T> GenericArray<T>::added(T value) const {
    auto res = *this;
    res.add(value);
    return res;
}

template<typename T>
template<typename Integer>
GenericArray<T> GenericArray<T>::subseq(
        const std::vector<Integer>& indices) const
{
    GenericArray<T> result;
    result.reserve(indices.size());
    for (Integer idx: indices) {
        result.push_back(at(idx));
    }
    return result;
}

// TODO(ifsmirnov): ever need to make it faster?
template<typename T>
template<typename Integer>
GenericArray<T> GenericArray<T>::subseq(
        const std::initializer_list<Integer>& indices) const
{
    return subseq(std::vector<T>(indices));
}

template<typename T>
const T& GenericArray<T>::choice() const {
    return at(rnd.next(size()));
}

template<typename T>
GenericArray<T> GenericArray<T>::choice(size_t count) const {
    ensure(count <= size());

    size_t n = size();

    std::unordered_map<T, T> used;
    std::vector<size_t> res;
    for (size_t i = 0; i < count; ++i) {
        size_t oldValue = used.count(n-i-1) ? used[n-i-1] : n-i-1;
        size_t index = rnd.next(static_cast<size_t>(n-i));
        res.push_back(used.count(index) ? used[index] : index);
        used[index] = oldValue;
    }

    return subseq(res);
}

// not sure if it would be needed ever
/*
template<typename T>
std::ostream& operator<<(std::ostream& out, const GenericArray<T>& array) {
    return out << repr(array);
}
*/

typedef GenericArray<int> Array;
typedef GenericArray<long long> Array64;
typedef GenericArray<double> Arrayf;

} // namespace impl

using impl::Array;
using impl::Array64;
using impl::Arrayf;

template<typename T>
impl::GenericArray<T> makeArray(const std::vector<T>& values) {
    return impl::GenericArray<T>(values);
}

template<typename T>
impl::GenericArray<T> makeArray(const std::initializer_list<T>& values) {
    return impl::GenericArray<T>(values);
}
