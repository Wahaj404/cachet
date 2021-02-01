#ifndef __CACHET_HPP__
#define __CACHET_HPP__

#include "b-plus-tree.hpp"
#include "table.hpp"
#include "utility.hpp"
#include "vector.hpp"

#include <fstream>
#include <functional>
#include <iostream>
#include <string>

class CachetException : public std::exception {
private:
    std::string msg;

public:
    CachetException(std::string const &msg) : msg(msg) {}
    char const *what() const noexcept override {
        return msg.c_str();
    }
};

class Cachet {
    Cachet(Cachet const &) = delete;
    Cachet &operator=(Cachet const &) = delete;

private:
    HashMap<std::string, Table> tables;
    void create(std::string const &);
    void insert(std::string const &);
    void update(std::string const &);
    void remove(std::string const &);
    void select(std::string const &);
    Vector<HashMap<std::string, std::string>> selector(std::string const &,
                                                       std::string const &);
    static HashMap<std::string, void (Cachet::*)(const std::string &)> funcs;
    std::ofstream file;

public:
    void loadFunctions() const {
        if (funcs.size() == 0) {
            funcs.insert("create", &Cachet::create);
            funcs.insert("insert", &Cachet::insert);
            funcs.insert("update", &Cachet::update);
            funcs.insert("remove", &Cachet::remove);
            funcs.insert("select", &Cachet::select);
        }
    }
    Cachet(std::string const &name) {
        loadFunctions();
        std::ifstream db(name);
        std::string input;
        while (std::getline(db, input, ';')) {
            while (in(input.front(), whitespace())) {
                input.erase(input.begin());
            }
            (*this)(input);
        }
        db.close();
        file.open(name, std::ios_base::app);
    }
    void operator()(std::string const &cmd) {
        try {
            auto func = funcs.get(cmd.substr(0, 6));
            std::invoke(func, *this, cmd);
        } catch (ItemNotFound) {
            throw CachetException("Unrecognized command");
        }
        if (cmd.substr(0, 6) != "select")
            file << cmd << ';';
    }
};

/*
    create $table (
        name
        age
    );
*/

#endif // __CACHET_HPP__
