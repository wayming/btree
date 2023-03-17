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
    long GetNumOfChildren() { return oNodes.size(); }

    void AddChild(Node* n) { oNodes.push_back(n); }

    void SetLeaf() { oLeaf = true; }
    bool IsLeaf() { return oLeaf; }

    void InsertKey(long key);
    long FirstGreaterThan(long key);
private:
    vector<Node*> oNodes;
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