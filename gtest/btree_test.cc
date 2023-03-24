#include "gtest/gtest.h"
#include "btree.h"

using ::testing::InitGoogleTest;

namespace {
TEST(Btree, Sanity) {
    BTree t(3);
    for (auto i = 0; i < 100; i++) {
        t.Insert(i);
        cout << t.Export() << endl;
    }
    cout << t.Root()->GetNumOfChildren() << endl;
    ASSERT_TRUE(t.Export().size() > 0);
}

}

int main(int argc, char** argv) {
    InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}