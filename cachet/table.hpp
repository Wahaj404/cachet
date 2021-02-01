#ifndef __TABLE_HPP__
#define __TABLE_HPP__

#include "b-plus-tree.hpp"
#include "condition.hpp"
#include "hash-map.hpp"
#include "vector.hpp"

#include <string>

class Table {
private:
    BPlusTree<std::string, Vector<std::string>, 4> data;
    HashMap<std::string, int> columns;
    std::string pKey;

public:
    Table() = default;
    Table(std::string const &pKey, Vector<std::string> const &columns) :
        pKey(pKey) {
        for (int i = 0; i < columns.size(); ++i) {
            this->columns.insert(columns[i], i);
        }
    }
    void insert(HashMap<std::string, std::string> const &values) {
        Vector<std::string> val(values.size());
        for (auto const &kvp : values) {
            int idx = columns.get(kvp.first);
            val[idx] = kvp.second;
        }
        data.insert(values.get(pKey), val);
    }
    Vector<std::string> *find(std::string const &key) {
        return data.findLeaf(key)->lookup(key);
    }

    Vector<HashMap<std::string, std::string>>
    select(std::string const &cond) const {
        auto conds = Condition::parse(cond);
        auto v = data.range("", "zzzzzzzzzzzzzzzzzzzzzz");
        Vector<HashMap<std::string, std::string>> ret;
        for (auto p : v) {
            HashMap<std::string, std::string> mp;
            for (auto kvp : columns) {
                mp.insert(kvp.first, get<1>(p)[kvp.second]);
            }
            if (Condition::evaluate(conds, mp)) {
                ret.pushBack(mp);
            }
        }
        return ret;
    }

    void update(Vector<HashMap<std::string, std::string>> const &records,
                HashMap<std::string, std::string> const &mp) {
        for (auto const &rec : records) {
            auto vec = find(rec.get(primary()));
            for (auto [key, val] : mp) {
                int i = columns.get(key);
                (*vec)[i] = val;
            }
        }
    }
    void remove(std::string const &pKey) {
        data.remove(pKey);
    }
    std::string const &primary() const {
        return pKey;
    }
};

#endif // __TABLE_HPP__
