#include <vector>
#include <string>
#include <iostream>

using namespace std;


class Logger {
public:
    Logger(string funName) : oFunName(funName) { cout << "> " << oFunName << endl; }
    ~Logger() { cout << "< " << oFunName << endl; }
    Logger(const Logger& l) = delete;
    Logger(Logger&& l) = delete;
    Logger& operator =(const Logger& l) = delete;
    Logger& operator =(Logger&& l) = delete;

    template<typename T> 
    Logger& operator<< (const T& data) { cout << data; return *this; }

    // this is the type of std::cout
    typedef std::basic_ostream<char, std::char_traits<char> > CoutType;

    // this is the function signature of std::endl
    typedef CoutType& (*StandardEndLine)(CoutType&);

    // define an operator<< to take in std::endl
    Logger& operator<<(StandardEndLine manip)
    {
        // call the function, but we cannot return it's value
        manip(std::cout);

        return *this;
    }

private:
    string oFunName;
};

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
    long GetNumOfKeys() const { return oKeys.size(); }
    long GetNumOfChildren() const { return oChildren.size(); }

    void AddChild(Node* n) { oChildren.push_back(n); }

    void IsLeaf(bool isLeaf) { oLeaf = isLeaf; }
    bool IsLeaf() { return oLeaf; }

    void InsertKey(long key);
    void InsertKeyToLeaves(long key);

    long GetKey(long idx) { return oKeys.at(idx); }
    Node* GetChild(long idx) { return oChildren.at(idx); }
    Node* GetChildByKey(long key);
    
    vector<long> ChopKeys(long idx) { return SplitVector<long>(oKeys, idx); }
    vector<Node*> ChopChildren(long idx) { return SplitVector<Node*>(oChildren, idx); }

    vector<string> Export();

    friend ostream& operator << (ostream& s, const Node& n)
    {
        s << "keys=";
        for (auto key : n.oKeys) {
            s << key << ",";
        }
        s << "#children=" << n.GetNumOfChildren();
        return s;
    }
private:
    long FirstChildIdxGreaterThanKey(long key);
    // Erase the elements that start from the index. Return the erased elements.
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
    BTree(const BTree& t) = delete;
    BTree(BTree&& t) = delete;
    BTree& operator =(const BTree& t) = delete;
    BTree& operator =(BTree&& t) = delete;

    void Insert(long key);
    void Find(long key) {}
    void Remove(long key) {}
    string Export();


private:
    Node* oRoot;
    long oDegree;  
};
