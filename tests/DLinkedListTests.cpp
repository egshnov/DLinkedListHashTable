#include "gtest/gtest.h"
#include "DLinkedList.hpp"


TEST(ListTestSuite, ExampleTest) {
    DLinkedList<int> l1;
    for (int i = 1; i < 10; i++) {
        l1.push_back(i);
    }
    DLinkedList<int> l2 = l1;
    EXPECT_EQ(l1, l2);

}

//
//TEST(SuiteName, TestName) {
//
//}
