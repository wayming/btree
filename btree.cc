#include "btree.h"
#include <algorithm>
#include <utility>

using namespace std;

void
Node::InsertKey(long key)
{
   
    // Double the reserved size of the vector
    if (oKeys.capacity() <= oKeys.size() + 1) {
        oKeys.reserve(2 * oKeys.capacity());
    }

    vector<long>::iterator iter = oKeys.begin();    
    while (key > *iter) {
        iter++;
    }
 
    std::shift_right(iter, oKeys.end(), 1);
    *iter = key;
}

long
Node::FirstGreaterThan(long key) {
    long idx = 0;
    while (key >= oKeys.at(idx)) {
        idx++;
    }

    return idx;
}

BTree::BTree(long degree)
{
    oRoot = new Node();
    oDegree = degree;
}

void
BTree::Insert(long key)
{
    if (oRoot->GetNumOfKeys() >= 2 * oDegree - 1) {
        Node* newRoot = new Node();
        newRoot->AddChild(oRoot);
        oRoot = newRoot;
        InsertIntoFull(key);
    } else {
        InsertIntoNotFull(oRoot, key);
    }
}

void
BTree::InsertIntoFull(long key)
{

}

void
BTree::InsertIntoNotFull(Node* curr, long key)
{
    if (curr->IsLeaf()) {
        curr->InsertKey(key);
    } else {
        long idx = curr->FirstGreaterThan(key);
    }

}