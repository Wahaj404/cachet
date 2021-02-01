#include "cachet.hpp"
#include "condition.hpp"

#include <bits/stdc++.h>
using namespace std;

int main() {
    Cachet cachet("test.db");
    string input;
    while (true) {
        cout << "cachet> ";
        std::getline(cin, input, ';');
        while (in(input.front(), whitespace())) {
            input.erase(input.begin());
        }
        if (input.front() == '.') {
            if (input == ".quit") {
                break;
            }
        } else {
            try {
                cachet(input);
            } catch (CachetException &ex) {
                std::cerr << ex.what() << '\n';
            } catch (...) {}
        }
    }
}
