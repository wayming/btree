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
Node::SplitChild(long key)
{
    Logger log(string("SplitChild key=") + to_string(key));
    Node* childLeft = GetChildByKey(key);

    // split keys
    int parentKeyPos = childLeft->GetNumOfKeys()/2;
    vector<long> keys = childLeft->ChopKeys(parentKeyPos);
    log << "childLeft:" << *childLeft << endl;
    
    long parentKey = keys.front();
    log << "parentKey=" << parentKey << endl;
    keys.erase(keys.begin());

    // Split child nodes
    vector<Node*> children;
    if (!childLeft->IsLeaf()) {
        children = childLeft->ChopChildren(parentKeyPos);
    }
    Node* childRight = new Node(oDegree, move(keys), move(children));
    childRight->IsLeaf(childLeft->IsLeaf());
    log << "childRight:" << *childRight << endl;

    long idx = FirstChildIdxGreaterThanKey(key);
    oKeys.insert(oKeys.begin()+idx, parentKey);
    oChildren.insert(oChildren.begin()+idx+1, childRight);
    log << "Parent:" << *this << endl;
}

void
Node::InsertKeyToLeaves(long key)
{
    // Double the reserved size of the vector
    if (oKeys.capacity() <= oKeys.size() + 1) {
        oKeys.reserve(2 * oKeys.capacity());
    }

    vector<long>::iterator iter = oKeys.begin();    
    while (iter != oKeys.end() && key > *iter) {
        iter++;
    }
 
    oKeys.insert(iter, key);
}
Node*
Node::GetChildByKey(long key)
{
    Logger log(string("GetChildByKey key=") + to_string(key));

    if (GetNumOfKeys() == 0 && GetNumOfChildren() > 0) {
        // No keys, return the first child, this can be the case when a new root is created
        return oChildren.front();
    } else if (key > oKeys.back()) {
        return oChildren.back();
    } else {
        long idx = FirstChildIdxGreaterThanKey(key);
        log << "idx=" << idx << endl;
        return GetChild(idx);
    }
}

void
Node::InsertKey(long key)
{
    Logger log(string("InsertKey key=") + to_string(key));

    if (IsLeaf()) {
        // Assumes not full. The full leaf nodes should have ben split when visiting the parent
        log << "Insert into leaves" << endl;
        InsertKeyToLeaves(key);
    } else {
        // Intermidiate or root node
        Node* child = GetChildByKey(key);
        if (child->GetNumOfKeys() >= 2 * oDegree - 1) {
            SplitChild(key);
        }

        // Key value of at the index position may have changed
        GetChildByKey(key)->InsertKey(key);
    }
}

// Retrun the index of first child that has the keys
// greater than the specified key.
long
Node::FirstChildIdxGreaterThanKey(long key)
{
    long idx = 0;
    for (auto currKey : oKeys) {
        if (currKey >= key) {
            break;
        }
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
        string padding("-", 2);

        for (auto const& c : oChildren) {
            vector<string> childOutput = c->Export();
            for (auto& co : childOutput) {
                co = padding + co;
            }
            output.insert(end(output), begin(childOutput), end(childOutput));
        }
    }

    return output;
}

BTree::BTree(long degree)
{
    oDegree = degree;
    oRoot = new Node(oDegree);
    oRoot->IsLeaf(true);
}

void
BTree::Insert(long key)
{
    if (oRoot->GetNumOfKeys() >= 2 * oDegree - 1) {
        // New root
        Node* newRoot = new Node(oDegree);
        Node* child = oRoot;
        newRoot->AddChild(child);
        newRoot->IsLeaf(false);
        oRoot = newRoot;
        oRoot->SplitChild(0);// Specified key is ignored
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