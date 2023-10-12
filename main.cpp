#include <iostream>
#include <string>
#include "HashTable.hpp"
#include <unordered_map>

//Валгринд фейлится на гуглтесте или на хэш таблице?
int main() {
    std::vector<std::vector<int>> keys = {
            {1,       2,   3,      4,       5,     6,   7,   8,   9,    10},
            {-1,      -2,  -3,     -4,      -5},
            {101,     201, 301,    450,     580,   630, 710, 820, 1900, 10000},
            {18,      5,   1,      7,       6,     11},
            {3914914, 5,   281240, 1249124, 21412, 12412},
    };
    std::vector<std::vector<std::string>> values = {
            {"1",       "2",             "3",      "4",       "5",     "6", "7", "8", "9", "10"},
            {"Vasya",   "Petya",         "Sasha",  "Oleg",    "Dima"},

            {"1",       "2",             "3",      "4",       "5",     "6", "7", "8", "9", "10"},

            {"3",       "9",             "28",     "124",     "21",    "112"},

            {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"}
    };

    for (int i = 0; i < keys.size(); i++) {
        HashTable<int, std::string> table;
        std::unordered_map<int, std::string> map;
        for (int j = 0; j < keys[i].size(); j++) {
            table.insert(keys[i][j], values[i][j]);
            map[keys[i][j]] = values[i][j];
        }
        for (int j = 0; j < keys[i].size(); j++) {
            if (table.at(keys[i][j]) != values[i][j]) {
                std::cout << "WRONG";
                return 1;
            }
        }
        auto it = --table.end();
        while (it != table.begin()) {
            std::cout << "bucket: " << it.cur_bucket << " key: " << it->key_ << " val: " << it->val_ << std::endl;
            --it;
        }

//        auto it = map.begin();
//        while (it != map.end()) {
//            std::cout << " key: " << it->first << " val: " << it->second << std::endl;
//            ++it;
//        }
//        std::cout << "\n\n\n\n";
    }
    return 0;
}
