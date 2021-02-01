#ifndef __COMPARATOR_HPP__
#define __COMPARATOR_HPP__

#include <string>

struct Comparator {
    virtual bool operator()(std::string const &, std::string const &) const = 0;
    virtual Comparator *copy() const = 0;
};

struct Less : Comparator {
    bool operator()(std::string const &, std::string const &) const override;
    Comparator *copy() const override;
};

struct Equal : Comparator {
    bool operator()(std::string const &, std::string const &) const override;
    Comparator *copy() const override;
};

struct Greater : Comparator {
    bool operator()(std::string const &, std::string const &) const override;
    Comparator *copy() const override;
};

struct LessEqual : Comparator {
    bool operator()(std::string const &, std::string const &) const override;
    Comparator *copy() const override;
};

struct NotEqual : Comparator {
    bool operator()(std::string const &, std::string const &) const override;
    Comparator *copy() const override;
};

struct GreaterEqual : Comparator {
    bool operator()(std::string const &, std::string const &) const override;
    Comparator *copy() const override;
};

#endif // __COMPARATOR_HPP__
