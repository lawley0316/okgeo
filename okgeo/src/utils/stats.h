#ifndef STATS_H
#define STATS_H

#include <vector>
#include <numeric>
#include <algorithm>

namespace stats {

template<typename It>
auto Sum(It begin, It end) {
    using T = typename std::iterator_traits<It>::value_type;
    static_assert(std::is_arithmetic<T>::value, "Sum only supports arithmetic types");
    return std::accumulate(begin, end, T{0});
}

template<typename It>
double Mean(It begin, It end) {
    using T = typename std::iterator_traits<It>::value_type;
    static_assert(std::is_arithmetic<T>::value, "Mean only support arithmetic types");

    if (begin == end) throw std::invalid_argument("no mean for empty data");

    double sum = 0.0;
    std::size_t n = 0;
    for (It it=begin; it!=end; ++it) {
        sum += static_cast<double>(*it);
        ++n;
    }
    return sum / n;
}

template<typename It>
double MedianInplace(It begin, It end) {
    using T = typename std::iterator_traits<It>::value_type;
    using C = typename std::iterator_traits<It>::iterator_category;
    static_assert(
        std::is_base_of<std::random_access_iterator_tag, C>::value,
        "MedianInplace requires RandomAccessIterator"
    );

    if (begin == end) throw std::invalid_argument("no median for empty data");
    std::size_t n = std::distance(begin, end);
    std::size_t m = n / 2;
    std::nth_element(begin, begin+m, end);
    if (n % 2 == 0) {
        T left = *std::max_element(begin, begin + m);
        T right = *(begin + m);
        return (static_cast<double>(left) + static_cast<double>(right)) / 2.0;
    } else {
        return static_cast<double>(*(begin + m));
    }
}

template<typename It>
double Median(It begin, It end) {
    using T = typename std::iterator_traits<It>::value_type;
    std::vector<T> copy(begin, end);
    return MedianInplace(copy.begin(), copy.end());
}

}

#endif // STATS_H
