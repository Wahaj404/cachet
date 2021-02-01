#include "condition.hpp"

#include "hash-map.hpp"
#include "utility.hpp"

Comparator *Condition::makeComparator(std::string const &op) {
    if (op == "==") {
        return new Equal;
    } else if (op == "!=") {
        return new NotEqual;
    } else if (op == "<") {
        return new Less;
    } else if (op == "<=") {
        return new LessEqual;
    } else if (op == ">") {
        return new Greater;
    } else if (op == ">=") {
        return new GreaterEqual;
    }
    throw 1;
}

Logical *Condition::makeLogical(std::string const &con) {
    if (con == "&&") {
        return new And;
    } else if (con == "||") {
        return new Or;
    } else if (con == "^") {
        return new Xor;
    } else if (con.empty()) {
        return nullptr;
    }
    throw 1;
}

bool Condition::evaluate(Vector<Condition> const &conds,
                         HashMap<std::string, std::string> const &mp) {
    bool ret = conds.empty() || conds.front().evaluate(mp);
    for (int i = 1; i < conds.size(); ++i) {
        ret = conds[i - 1].connect(ret, conds[i].evaluate(mp));
    }
    return ret;
}

Vector<Condition> Condition::parse(std::string const &str) {
    Vector<Condition> ret;
    int i = str.find("where");
    if (i < str.length())
        i += sizeof("where");
    while (i < str.length() && in(str.at(i), whitespace())) {
        ++i;
    }
    while (i < str.length()) {
        int colStart = i, colEnd = str.find_first_of(whitespace(), colStart);
        auto col = str.substr(colStart, colEnd - colStart);

        i = colEnd;
        while (i < str.length() && in(str.at(i), whitespace())) {
            ++i;
        }
        int opStart = i, opEnd = str.find_first_of(whitespace(), opStart);
        auto op = str.substr(opStart, opEnd - opStart);

        i = opEnd;
        while (i < str.length() && in(str.at(i), whitespace())) {
            ++i;
        }
        int valStart = i, valEnd = 0;
        if (str.at(valStart) == '\'') {
            valEnd = str.find_first_of("'", ++valStart);
        } else {
            valEnd = str.find_first_of(whitespace() + ",", valStart);
        }
        auto val = str.substr(valStart, valEnd - valStart);

        if (valEnd >= str.length()) {
            ret.pushBack(Condition(col, op, "", val));
            break;
        }

        i = valEnd + (str.at(valEnd) == '\'');
        while (i < str.length() && in(str.at(i), whitespace())) {
            ++i;
        }

        int conStart = i,
            conEnd = str.find_first_of(whitespace() + ";", conStart);
        auto con = str.substr(conStart, conEnd - conStart);
        ret.pushBack(Condition(col, op, con, val));

        i = conEnd;
        i = str.find_first_not_of(whitespace(), i);
    }
    return ret;
}

Condition::Condition() : op(nullptr), con(nullptr) {}

Condition::Condition(std::string const &col, std::string const &op,
                     std::string const &con, std::string const &val) :
    col(col),
    op(makeComparator(op)), con(makeLogical(con)), val(val) {}

Condition::Condition(Condition const &other) :
    col(other.col), op(other.op->copy()),
    con(other.con == nullptr ? nullptr : other.con->copy()), val(other.val) {}

Condition &Condition::operator=(Condition const &other) {
    if (this != &other) {
        delete op;
        delete con;
        op = other.op->copy();
        con = other.con == nullptr ? nullptr : other.con->copy();
        col = other.col;
        val = other.val;
    }
    return *this;
}

bool Condition::evaluate(std::string const &check) const {
    return (*op)(check, val);
}

bool Condition::evaluate(HashMap<std::string, std::string> const &mp) const {
    return evaluate(mp.get(col));
}

bool Condition::connect(bool a, bool b) const {
    return (*con)(a, b);
}

Condition::~Condition() {
    delete op;
}