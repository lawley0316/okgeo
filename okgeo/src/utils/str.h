#ifndef STR_H
#define STR_H

#include <sys/stat.h>

#include <random>
#include <cstring>
#include <algorithm>

namespace str {
static const std::string DIGITS = "0123456789";
static const std::string LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
static const std::string UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static const std::string LETTERS = LOWERCASE + UPPERCASE;
static const std::string ALPHANUM = DIGITS + LETTERS;

inline void Split(const std::string& str, char sep, std::vector<std::string>& substrs) {
    std::string::size_type i = 0;
    std::string::size_type j = str.find(sep);
    while (j != std::string::npos) {
        substrs.push_back(str.substr(i, j-i));
        i = j + 1;
        j = str.find(sep, i);
    }
    substrs.push_back(str.substr(i));
}

inline void Split(const std::string& str, const std::string& sep, std::vector<std::string>& substrs) {
    if (sep.empty()) {
        substrs.push_back(str);
        return;
    }
    std::string::size_type i = 0;
    std::string::size_type j = str.find(sep);
    std::string::size_type l = sep.length();
    while (j != std::string::npos) {
        substrs.push_back(str.substr(i, j-i));
        i = j + l;
        j = str.find(sep, i);
    }
    substrs.push_back(str.substr(i));
}

template<typename It>
std::string Join(It begin, It end, const std::string& sep) {
    std::string str;
    if (begin == end) return str;
    for (auto it=begin; it!=end; ++it) {
        str += *it;
        if (it != end - 1) str += sep;
    }
    return str;
}

inline bool StartsWith(const std::string& str, const char* prefix) {
    if (strlen(prefix) == 0) return true;
    return str.rfind(prefix, 0) == 0;
}

inline bool StartsWith(const std::string& str, const std::string& prefix) {
    if (prefix.empty()) return true;
    return str.rfind(prefix, 0) == 0;
}

inline void Replace(std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return;
    std::string::size_type pos = str.find(from);
    while (pos != std::string::npos) {
        str.replace(pos, from.length(), to);
        pos = str.find(from, pos+to.length());
    }
}

inline void Upper(std::basic_string<char>& str) {
    for (auto it=str.begin(); it!=str.end(); ++it) {
        *it = toupper(*it);
    }
}

inline void Upper(std::basic_string<wchar_t>& str) {
    for (auto it=str.begin(); it!=str.end(); ++it) {
        *it = towupper(*it);
    }
}

inline void Lower(std::basic_string<char>& str) {
    for (auto it=str.begin(); it!=str.end(); ++it) {
        *it = tolower(*it);
    }
}

inline void Lower(std::basic_string<wchar_t>& str) {
    for (auto it=str.begin(); it!=str.end(); ++it) {
        *it = towlower(*it);
    }
}

inline std::string Random(std::size_t n) {
    if (n == 0) return {};
    std::string str(n, 0);
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dist(0, ALPHANUM.length()-1);
    std::generate_n(str.begin(), n, [&]() -> char {
        std::size_t i = dist(gen);
        return ALPHANUM[i];
    });
    return str;
}
}

#endif // STR_H
