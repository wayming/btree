#include "btree.h"
#include <algorithm>
#include <utility>

using namespace std;

Node::Node(long degree, vector<long> keys, vector<Node*> children)
    : oDegree(degree), oKeys(keys), oChildren(children)
{

}


void
Node::SplitChild(long index)
{
    Node* childLeft = GetChild(index);

    // Construct right child
    vector<long> keys = childLeft->ChopKeys(index);
    long parentKey = keys.front();
    keys.erase(keys.begin());
    vector<Node*> children = childLeft->ChopChildren(index);
    Node* childRight = new Node(oDegree, move(keys), move(children));

    InsertKey(parentKey);
    oChildren.insert(oChildren.begin()+index+1, childRight);
}

void
Node::InsertKeyToLeaves(long key)
{
    // Double the reserved size of the vector
    if (oKeys.capacity() <= oKeys.size() + 1) {
        oKeys.reserve(2 * oKeys.capacity());
    }

    vector<long>::iterator iter = oKeys.begin();    
    while (key > *iter) {
        iter++;
    }
 
    oKeys.insert(iter, key);
}

void
Node::InsertKey(long key)
{
    if (IsLeaf()) {
        // Assumes not full. The full leaf nodes should have ben split when visiting the parent
        InsertKeyToLeaves(key);
    } else {
        // Intermidiate node
        long idx = FirstGreaterThan(key);
        if (GetChild(idx)->GetNumOfKeys() >= 2 * oDegree - 1) {
            SplitChild(key);
        }

        if (key > GetKey(idx)) {
            // Insert into right subtree
            GetChild(idx+1)->InsertKey(key);
        } else {
            // Insert into left subtree
            GetChild(idx)->InsertKey(key);
        }
    }
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
    oDegree = degree;
    oRoot = new Node(oDegree);
}

void
BTree::Insert(long key)
{
    if (oRoot->GetNumOfKeys() >= 2 * oDegree - 1) {
        // New root
        Node* newRoot = new Node(oDegree);
        newRoot->AddChild(oRoot);
        oRoot = newRoot;
        oRoot->SplitChild(1);
        oRoot->InsertKey(key);
    }
    
    oRoot->InsertKey(key);
}