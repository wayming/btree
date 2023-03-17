#include <vector>
#include <string>

using namespace std;

class Node {
public:
    Node() = default;
    ~Node() = default;
    Node(const Node& n) = delete;
    Node(Node&& n) = delete;
    Node& operator =(const Node& n) = delete;
    Node& operator =(Node&& n) = delete;

    void SplitChild(long key) {}
    long GetNumOfKeys() { return oKeys.size(); }
    long GetNumOfChildren() { return oChildren.size(); }

    void AddChild(Node* n) { oChildren.push_back(n); }

    void SetLeaf() { oLeaf = true; }
    bool IsLeaf() { return oLeaf; }

    void InsertKey(long key);
    long GetKey(long idx) { return oKeys.at(idx); }
    Node* GetChild(long idx) { return oChildren.at(idx); }
    long FirstGreaterThan(long key);
private:
    vector<Node*> oChildren;
    vector<long> oKeys;
    bool oLeaf{false};
};

class BTree {
public:
    BTree(long degree);
    ~BTree() { /* Destroy nodes */}

    void Insert(long key);
    void Find(long key) {}
    void Remove(long key) {}
    string Dump() { return string("BTree Content"); }


private:
    void InsertIntoFull(long key);
    void InsertIntoNotFull(Node* curr, long key);
    Node* oRoot;
    long oDegree;  
};