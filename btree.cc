#include "btree.h"
#include <algorithm>
#include <utility>
#include <sstream>
#include <iterator>

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
    childRight->IsLeaf(childLeft->IsLeaf());
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
Node::FirstGreaterThan(long key)
{
    long idx = 0;
    while (key >= oKeys.at(idx)) {
        idx++;
    }

    return idx;
}

vector<string>
Node::Export()
{
    vector<string> output;
    stringstream outputStream;
    copy(oKeys.begin(), oKeys.end(), ostream_iterator<long>(outputStream, ","));
    output.push_back(outputStream.str());

    // Children node
    if (!IsLeaf()) {
        long size = outputStream.str().size();
        string padding(" ", size);

        for (auto const& c : oChildren) {
            vector<string> childOutput = c->Export();
            for (auto& co : childOutput) {
                co = padding + co;
            }
        }
    }

    return output;
}

BTree::BTree(long degree)
{
    oDegree = degree;
    oRoot = new Node(oDegree);
    oRoot->IsLeaf(true); // First node
}

void
BTree::Insert(long key)
{
    if (oRoot->GetNumOfKeys() >= 2 * oDegree - 1) {
        // New root
        Node* newRoot = new Node(oDegree);
        newRoot->AddChild(oRoot);
        newRoot->IsLeaf(false);
        oRoot = newRoot;
        oRoot->SplitChild(1);
        oRoot->InsertKey(key);
    }
    
    oRoot->InsertKey(key);
}

string
BTree::Export()
{
    vector<string> output = oRoot->Export();
    stringstream outputStream;
    copy(output.begin(), output.end(), ostream_iterator<string>(outputStream, "\n"));
    return outputStream.str();
}