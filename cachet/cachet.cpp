#include "cachet.hpp"

#include "../condition/condition.hpp"

#include <iomanip>

HashMap<std::string, void (Cachet::*)(const std::string &)> Cachet::funcs;

void Cachet::create(std::string const &cmd) {
    int nameStart = cmd.find_first_of(whitespace()) + 1;
    int nameEnd = cmd.find_first_of(whitespace() + "(", nameStart);
    auto name = cmd.substr(nameStart, nameEnd - nameStart);
    Vector<std::string> columns;
    int lim = cmd.find_last_of(')');
    for (int i = cmd.find_first_of('(') + 1; i < lim; ++i) {
        int end = cmd.find_first_of(whitespace() + ",)", i);
        if (i == end)
            continue;
        auto col = cmd.substr(i, end - i);
        columns.pushBack(col);
        i = end;
    }
    tables.insert(name, Table(columns[0], columns));
}

void Cachet::insert(std::string const &cmd) {
    constexpr int len = sizeof("INSERT INTO ") - 1;
    int nameEnd = cmd.find_first_of(whitespace() + "(", len);
    auto name = cmd.substr(len, nameEnd - len);
    int colStart = cmd.find_first_of('('),
        colEnd = cmd.find_first_of(')', colStart),
        valStart = cmd.find_first_of('(', colEnd),
        valEnd = cmd.find_last_of(')');
    auto cols = cmd.substr(colStart + 1, colEnd - colStart - 1);
    auto vals = cmd.substr(valStart + 1, valEnd - valStart - 1);
    auto columns = tokenize(cols, ", ");
    auto values = tokenize(vals, ",");
    for (int i = 0; i < values.size(); ++i) {
        trim(values[i]);
        unquote(values[i]);
        if (values[i].empty()) {
            values.remove(i);
            --i;
        }
    }
    HashMap<std::string, std::string> final;
    for (int i = 0; i < columns.size(); ++i) {
        final.insert(columns[i], values[i]);
    }
    tables.get(name).insert(final);
}

void Cachet::update(std::string const &cmd) {
    constexpr int len = sizeof("UPDATE ") - 1;
    int nameEnd = cmd.find_first_of(whitespace(), len);
    auto name = cmd.substr(len, nameEnd - len);

    HashMap<std::string, std::string> mp;

    int valStart = nameEnd + sizeof(" SET ");
    int i = valStart - 1;
    while (in(cmd.at(i), whitespace() + ",")) {
        ++i;
    }
    while (i < cmd.length() && cmd.substr(i, 5) != "where") {
        auto col = cmd.substr(i, cmd.find_first_of(whitespace() + ",", i) - i);
        if (col == tables.get(name).primary()) {
            throw CachetException("Cannot change primary key's value");
        }
        int vStart =
                cmd.find_first_not_of(whitespace() + "=", i + col.length()),
            vEnd = 0;
        if (cmd.at(vStart) == '\'') {
            vEnd = cmd.find_first_of("'", ++vStart);
        } else {
            vEnd = cmd.find_first_of(whitespace() + ",;", vStart);
        }
        auto val = cmd.substr(vStart, vEnd - vStart);
        mp.insert(col, val);
        i = vEnd;
        while (i < cmd.length() && in(cmd.at(i), whitespace() + ",")) {
            ++i;
        }
    }

    auto records = selector(name, cmd);
    tables.get(name).update(records, mp);
}

void Cachet::remove(std::string const &cmd) {
    int nameStart = sizeof("REMOVE FROM ") - 1,
        nameEnd = cmd.find_first_of(whitespace() + ";", nameStart);
    auto name = cmd.substr(nameStart, nameEnd - nameStart);
    auto records = selector(name, cmd);
    for (auto const &rec : records) {
        auto &tab = tables.get(name);
        tab.remove(rec.get((tab.primary())));
    }
}

void Cachet::select(std::string const &cmd) {
    int nameStart = sizeof("SELECT FROM ") - 1,
        nameEnd = cmd.find_first_of(whitespace() + ";", nameStart);
    auto name = cmd.substr(nameStart, nameEnd - nameStart);
    if (auto records = selector(name, cmd); !records.empty()) {
        for (auto [col, _] : records[0]) {
            std::cerr << Center(col, 20) << '|';
        }
        std::cerr << '\n'
                  << std::setfill('-') << std::setw(21 * records[0].size())
                  << '\n'
                  << std::setfill(' ');
        for (auto const &rec : records) {
            for (auto [_, val] : rec) {
                std::cerr << Center(val, 20) << '|';
            }
            std::cerr << '\n';
        }
    }
}

Vector<HashMap<std::string, std::string>>
Cachet::selector(std::string const &name, std::string const &cmd) {
    return tables.get(name).select(cmd);
}
