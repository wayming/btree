#include <vector>
#include <string>

using namespace std;

class Node {
public:
    Node(long degree) : oDegree(degree) {};
    Node(long degree, vector<long> keys, vector<Node*> children);
    ~Node() = default;
    Node(const Node& n) = delete;
    Node(Node&& n) = delete;
    Node& operator =(const Node& n) = delete;
    Node& operator =(Node&& n) = delete;

    void SplitChild(long key);
    long GetNumOfKeys() { return oKeys.size(); }
    long GetNumOfChildren() { return oChildren.size(); }

    void AddChild(Node* n) { oChildren.push_back(n); }

    void IsLeaf(bool isLeaf) { oLeaf = isLeaf; }
    bool IsLeaf() { return oLeaf; }

    void InsertKey(long key);
    void InsertKeyToLeaves(long key);

    long GetKey(long idx) { return oKeys.at(idx); }
    Node* GetChild(long idx) { return oChildren.at(idx); }
    
    vector<long> ChopKeys(long idx) { return SplitVector<long>(oKeys, idx); }
    vector<Node*> ChopChildren(long idx) { return SplitVector<Node*>(oChildren, idx); }

    vector<string> Export();
private:
    long FirstChildIdxGreaterThanKey(long key);
    // Erase the elements(start from the index). Return the erased elements.
    template <class T>
    vector<T> SplitVector(vector<T>& source, long idx) {
        vector<T> remaining(source.begin()+idx, source.end());
        source.erase(source.begin()+idx, source.end());
        return remaining;
    }
    vector<Node*> oChildren;
    vector<long> oKeys;
    bool oLeaf{false};
    long oDegree;
};

class BTree {
public:
    BTree(long degree);
    ~BTree() { /* Destroy nodes */}

    void Insert(long key);
    void Find(long key) {}
    void Remove(long key) {}
    string Export();


private:
    Node* oRoot;
    long oDegree;  
};