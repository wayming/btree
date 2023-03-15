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
private:
    vector<Node> oNodes;
    vector<long> oKeys;
};

class BTree {
public:
    void Insert(long key) {}
    void Find(long key) {}
    void Remove(long key) {}
    string Dump() { return string("BTree Content"); }
private:
    Node* oRoot;    
};