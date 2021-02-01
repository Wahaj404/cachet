#ifndef __CONDITION_HPP__
#define __CONDITION_HPP__

#include "comparator.hpp"
#include "hash-map.hpp"
#include "logical.hpp"
#include "vector.hpp"

#include <string>

class Condition {
private:
    std::string col;
    Comparator *op;
    Logical *con;
    std::string val;
    static Comparator *makeComparator(std::string const &);
    static Logical *makeLogical(std::string const &);

public:
    static Vector<Condition> parse(std::string const &);
    static bool evaluate(Vector<Condition> const &,
                         HashMap<std::string, std::string> const &);
    Condition();
    Condition(Condition const &);
    Condition &operator=(Condition const &);
    bool evaluate(std::string const &) const;
    bool evaluate(HashMap<std::string, std::string> const &) const;
    bool connect(bool, bool) const;
    Condition(std::string const &, std::string const &, std::string const &,
              std::string const &);
    ~Condition();
};

#endif // __CONDITION_HPP__
