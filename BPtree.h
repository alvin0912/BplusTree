#include<iostream>
#include<string>
using namespace std;

class Node{
    friend class BPtree;

    private:
	int size;
    int *key;
    string *value;
    Node **ptr;
    bool isleaf;
	Node *lptr;
	Node *rptr;

    public:
    Node(int);
};

class BPtree{
    private:
    Node *root;
    int order;
    int level;
	int lebel;
	void insertInternal(int,Node *,Node *);
    Node *findParent(Node *,Node *);
	void setLeftPointer(Node *,Node *);

    public:
    BPtree(int);
    void drop(int);
    void insert(int,string);
    void display(Node *);
	void showList(int);
    Node *getRoot();
};
