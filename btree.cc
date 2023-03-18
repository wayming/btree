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
 
    oKeys.insert(iter, key);
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
        // Intermidiate node
        long idx = curr->FirstGreaterThan(key);
        if (curr->GetChild(idx)->GetNumOfKeys() >= 2 * oDegree - 1) {
            curr->SplitChild(key);
        }

        if (key > curr->GetKey(idx)) {
            // Insert into right subtree
            InsertIntoNotFull(curr->GetChild(idx+1), key);
        } else {
            // Insert into left subtree
            InsertIntoNotFull(curr->GetChild(idx), key);
        }
    }

}