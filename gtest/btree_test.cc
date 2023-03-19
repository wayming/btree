#include "gtest/gtest.h"
#include "btree.h"

using ::testing::InitGoogleTest;

namespace {
TEST(Btree, Sanity) {
    BTree t(5);
    ASSERT_TRUE(t.Export().size() > 0);
}

}

int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}