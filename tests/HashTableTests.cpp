//
// Created by shalasheg on 07.10.23.
//
#include "gtest/gtest.h"
#include "HashTable.hpp"
#include <vector>
#include <string>
#include "initializer_list"


struct IS_Param {
    std::vector<int> keys;
    std::vector<std::string> values;

    IS_Param(std::initializer_list<int> l1 = {}, std::initializer_list<std::string> l2 = {}) :
            keys(l1), values(l2) {}
};

class IS_Param_Test : public ::testing::TestWithParam<IS_Param> {
};


TEST_P(IS_Param_Test, Constructor_and_accessors_test_int) {
    using T = int;
    //auto const &[keys, values] = GetParam();
    auto keys = GetParam().keys;
    auto values = GetParam().values;
    EXPECT_EQ(keys.size(), values.size());
    HashTable<int, std::string> table1;

    for (int i = 0; i < keys.size(); i++) {
        table1.insert(keys[i], values[i]);
    }

    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table1.at(keys[i]), values[i]) << "key " << std::to_string(keys[i]) << " val " << values[i];
    }
    table1.resize(20);
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table1.at(keys[i]), values[i]) << "key " << std::to_string(keys[i]) << " val " << values[i];
    }

}
//TODO: resize test, deletion test, custom hash test, проверить можно ли сделать hash table и hash table iterator друзьями без дыры в интерфейсе
//TODO: разбить на constructor test, accessors + resize test, custom hash test
//передавать разные значения ключей в список иначе тест ломается
INSTANTIATE_TEST_SUITE_P(HashTable, IS_Param_Test,
                         testing::Values(IS_Param({1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
                                                  {"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}),
                                         IS_Param({-1, -2, -3, -4, -5}, {"Vasya", "Petya", "Sasha", "Oleg", "Dima"}),
                                         IS_Param({101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000},
                                                  {"A", "B", "C", "D", "E", "F", "G", "H", "I", "J"}),
                                         IS_Param({18, 5, 1, 7, 6, 11},
                                                  {"3", "9", "28", "124", "21", "112"}),
                                         IS_Param({3914914, 5, 281240, 1249124, 21412, 12412},
                                                  {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"})
                         ));

class MyHashFunction {
public:

    size_t operator()(const std::string &p) const {
        size_t sum = 0;
        for (char i: p) {
            sum += i;
        }
        return sum;
    }
};


struct SI_Param {
    std::vector<std::string> keys;
    std::vector<int> values;

    SI_Param(std::initializer_list<std::string> l1 = {}, std::initializer_list<int> l2 = {}) :
            keys(l1), values(l2) {}
};

class SI_Param_Test : public ::testing::TestWithParam<SI_Param> {
};


TEST_P(SI_Param_Test, Constructor_and_accessors_test_string_key) {
    using T = std::string;

    //auto const &[keys, values] = GetParam();
    auto keys = GetParam().keys;
    auto values = GetParam().values;
    EXPECT_EQ(keys.size(), values.size());
    HashTable<std::string, int, MyHashFunction> table1;

    for (int i = 0; i < keys.size(); i++) {
        table1.insert(keys[i], values[i]);
    }

    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table1.at(keys[i]), values[i]) << "key " << keys[i] << " val " << values[i];
    }
    table1.resize(20);
    for (int i = 0; i < keys.size(); i++) {
        EXPECT_EQ(table1.at(keys[i]), values[i]) << "key " << keys[i] << " val " << values[i];
    }

}
//TODO: resize test, deletion test, custom hash test, проверить можно ли сделать hash table и hash table iterator друзьями без дыры в интерфейсе
//TODO: разбить на constructor test, accessors + resize test, custom hash test
//передавать разные значения ключей в список иначе тест ломается
INSTANTIATE_TEST_SUITE_P(HashTable, SI_Param_Test,
                         testing::Values(SI_Param({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"},
                                                  {1, 2, 3, 4, 5, 6, 7, 8, 9, 10}
                                         ),
                                         SI_Param({"Vasya", "Petya", "Sasha", "Oleg", "Dima"}, {-1, -2, -3, -4, -5}),
                                         SI_Param({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"},
                                                  {101, 201, 301, 450, 580, 630, 710, 820, 1900, 10000}
                                         ),
                                         SI_Param(
                                                 {"3", "9", "28", "124", "21", "112"}, {18, 5, 1, 7, 6, 11}),
                                         SI_Param(
                                                 {"3914914", "9011914-90000", "281240", "1249124", "21412", "12412"},
                                                 {3914914, 5, 281240, 1249124, 21412, 12412})
                         ));


