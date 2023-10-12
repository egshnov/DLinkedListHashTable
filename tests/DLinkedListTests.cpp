#include "gtest/gtest.h"
#include "DLinkedList.hpp"
#include <vector>
#include "initializer_list"

//TODO: push_front, insert_after

struct IS_Param {
    std::vector<int> vec_inp;
    std::vector<int> indices;

    IS_Param(std::initializer_list<int> l1 = {}, std::initializer_list<int> l2 = {}) :
            vec_inp(l1), indices(l2) {}
};

class TestCAInt : public ::testing::TestWithParam<IS_Param> {
};


TEST_P(TestCAInt, Constructor_and_accessors_test_int) {
    using T = int;
    auto const &[vec_inp, indices] = GetParam();
    DLinkedList<T> l1;
    for (auto i: vec_inp) {
        l1.push_back(i);
    }
    DLinkedList<T> l2 = l1;
    EXPECT_EQ(l1, l2);
    DLinkedList<T> l3(std::move(l2));
    EXPECT_EQ(l1, l3);
    for (int i: indices) {
        l1.del(i);
    }
    EXPECT_EQ(l1 != l3, indices.size() != 0);
    DLinkedList<T> l4;
    //std::vector<int> tmp;
    for (int i = 0; i < vec_inp.size(); i++) {
        if (std::find(l1.begin(), l1.end(), vec_inp[i]) != l1.end()) {
            l4.push_back(vec_inp[i]);
        }
    }
    EXPECT_EQ(l1, l4); //<< l1.to_string() << "\n" << l4.to_string();
}
//передавать разные значения в списко иначе тест ломается
INSTANTIATE_TEST_SUITE_P(ConstAccess, TestCAInt,
                         testing::Values(IS_Param({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, {1, 2, 2, 3}),
                                         IS_Param({100, 124, 12125, 153, 1511, 1315135, 112, 1,
                                                   -1000, 1224}, {0, 0, 0, 0, 0, 0, 0, 0}),
                                         IS_Param({1, 2, 3,}),
                                         IS_Param({1, 2, 3, 4, 5, 6, 7, 8}, {1, 2, 3, 4})));


struct CAParam_string {
    std::vector<std::string> vec_inp;
    std::vector<int> indices;

    CAParam_string(std::initializer_list<std::string> l1 = {}, std::initializer_list<int> l2 = {}) :
            vec_inp(l1), indices(l2) {}
};

class TestCAString : public ::testing::TestWithParam<CAParam_string> {
};


TEST_P(TestCAString, Constructor_and_accessors_test_int) {
    using T = std::string;
    auto const &[vec_inp, indices] = GetParam();
    DLinkedList<T> l1;
    for (auto i: vec_inp) {
        l1.push_back(i);
    }
    DLinkedList<T> l2 = l1;
    EXPECT_EQ(l1, l2);
    DLinkedList<T> l3(std::move(l2));
    EXPECT_EQ(l1, l3);
    for (int i: indices) {
        l1.del(i);
    }
    EXPECT_EQ(l1 != l3, indices.size() != 0);
    DLinkedList<T> l4;
    //std::vector<int> tmp;
    for (int i = 0; i < vec_inp.size(); i++) {
        if (std::find(l1.begin(), l1.end(), vec_inp[i]) != l1.end()) {
            l4.push_back(vec_inp[i]);
        }
    }
    EXPECT_EQ(l1, l4); //<< l1.to_string() << "\n" << l4.to_string();
}
//передавать разные значения в список иначе тест ломается
INSTANTIATE_TEST_SUITE_P(ConstAccess, TestCAString,
                         testing::Values(CAParam_string({"aavav","adfsaf","favadv","sdvsvdv","savsadva","dvwvwv","wnviv","vwervwvw"}, {1, 2, 2, 3}),
                                         CAParam_string({"A", "B", "C", "D", "E", "F","G", "H",
                                                      "I", "J"}, {0, 0, 0, 0, 0, 0, 0, 0}),
                                         CAParam_string({"IVan", "Petr", "Vasya",}),
                                         CAParam_string({"1", "2", "3", "4", "5", "6", "7", "8"}, {1, 2, 3, 4})));

