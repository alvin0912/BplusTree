#include<iostream>
#include"BPtree.h"
#include<string>
using namespace std;

Node::Node(int order){
    key=new int[order];
	value=new string[order];
    ptr=new Node *[order+1];
	isleaf=false;
	lptr=NULL;
	rptr=NULL;
}

BPtree::BPtree(int order){
    root=NULL;
	level=0;
	lebel=1;
    this->order=order;
}

Node* BPtree::getRoot(){
	return root;
}

void BPtree::drop(int target){
	if(this->root==NULL)return;

	Node *cursor=root;
	Node *parent=cursor;
	while(!cursor->isleaf){
		for(int i=0;i<cursor->size;i++){
			if(target<cursor->key[i]){
				parent=cursor;
				cursor=cursor->ptr[i];
				break;
			}
			if(i==cursor->size-1){
				parent=cursor;
				cursor=cursor->ptr[cursor->size];
				break;
			}
		}
	}
	for(int i=0;i<cursor->size+1;i++){
		if(target==cursor->key[i]){
			//drop detail
			for(int j=i;j<cursor->size-1;j++){
				cursor->key[j]=cursor->key[j+1];
				cursor->value[j]=cursor->value[j+1];
			}
			cursor->size--;
			//drop leaf
			if(cursor->size==0){
				//deal linked list
				if(cursor->lptr)cursor->lptr->rptr=cursor->rptr;
				if(cursor->rptr)cursor->rptr->lptr=cursor->lptr;
				//deal node
				int idx=0;
				for(idx=0;idx<parent->size+1;idx++)if(parent->ptr[idx]==cursor)break;
				for(int k=idx-1;k<parent->size-1;k++)parent->key[k]=parent->key[k+1];
				for(int k=idx;k<parent->size;k++)parent->ptr[k]=parent->ptr[k+1];
				parent->size--;
				delete cursor;
			}
			cout << "Drop Success!" << endl;
			return;
		}
		else{
			cout << "Not Found" << endl;
			return;
		}
	}
	return;
}

void BPtree::insert(int key,string value){
	if (root == NULL){
		root = new Node(this->order);
		root->key[0] = key;
		root->value[0]=value;
		root->isleaf = true;
		root->size = 1;
		cerr << "insert successed!" << endl;
	}
	else{
		Node *cursor=root;
        Node *parent;
        //尋找下一個要插的位子
        while(cursor->isleaf==false){
            parent=cursor;
            //在節點中找要往哪一個路線走
            for(int i=0;i<cursor->size;i++){
                if(key<cursor->key[i]){
                    cursor=cursor->ptr[i];
                    break;
                }
                if(i==cursor->size-1){
                    cursor=cursor->ptr[i+1];
                    break;
                }
            }
        }
		//考慮是否滿的情況
        if(cursor->size < this->order){
            //挪出空間插入
            int idx=0;
            while(key > cursor->key[idx] && idx < cursor->size)idx++;
            for(int j=cursor->size;j>idx;j--){
                cursor->key[j]=cursor->key[j-1];
				cursor->value[j]=cursor->value[j-1];
            }
            cursor->key[idx]=key;
            cursor->value[idx]=value;
            cursor->size++;
            cursor->ptr[cursor->size]=cursor->ptr[cursor->size-1];
            cursor->ptr[cursor->size-1]=NULL;
			cerr << "insert successed!" << endl;
        }
		else{
			Node *tmpNode=new Node(this->order+1);
			for(int i=0;i<this->order;i++){
				tmpNode->key[i]=cursor->key[i];
				tmpNode->value[i]=cursor->value[i];
			}
			int idx=0;
			//找到新的key該在的位置
			while(key>tmpNode->key[idx] && idx<this->order)idx++;
			for(int j=this->order;j>idx;j--){
				tmpNode->key[j]=tmpNode->key[j-1];
				tmpNode->value[j]=tmpNode->value[j-1];
			}
			tmpNode->key[idx]=key;
			tmpNode->value[idx]=value;

			Node *newLeaf=new Node(this->order);
			newLeaf->isleaf=true;
			cursor->size=(this->order+1)/2;
			newLeaf->size=this->order+1-(this->order+1)/2;
			//處理葉子間的linked_list
			newLeaf->rptr=cursor->rptr;
			cursor->rptr=newLeaf;
			setLeftPointer(cursor,newLeaf);
			newLeaf->lptr=cursor;
			//抄錄到新的葉片
			for(int i=0;i<cursor->size;i++){
				cursor->key[i]=tmpNode->key[i];
				cursor->value[i]=tmpNode->value[i];
			}
			for(int i=0,j=cursor->size;i<newLeaf->size;i++,j++){
				newLeaf->key[i]=tmpNode->key[j];
				newLeaf->value[i]=tmpNode->value[j];
			}
			//重新調整整棵樹
			cerr << "insert successed!" << endl;
			if(cursor==root){
				Node *newRoot=new Node(this->order);
				newRoot->key[0]=newLeaf->key[0];
				newRoot->ptr[0]=cursor;
				newRoot->ptr[1]=newLeaf;
				newRoot->isleaf=false;
				newRoot->size=1;
				root=newRoot;
			}
			else insertInternal(newLeaf->key[0],parent,newLeaf);
		}

	}
}

void BPtree::insertInternal(int key,Node *cursor,Node *newLeaf){
	if(cursor->size < this->order){
		int idx=0;
		while(key>cursor->key[idx] && idx<cursor->size)idx++;
		for(int j=cursor->size;j>idx;j--)cursor->key[j]=cursor->key[j-1];
		for(int j=cursor->size+1;j>idx+1;j--)cursor->ptr[j]=cursor->ptr[j-1];
		cursor->key[idx]=key;
		cursor->size++;
		cursor->ptr[idx+1]=newLeaf;
	}
	else{
		Node *tmpNode=new Node(this->order+1);
		for(int i=0;i<this->order;i++)tmpNode->key[i]=cursor->key[i];
		for(int i=0;i<this->order+1;i++)tmpNode->ptr[i]=cursor->ptr[i];
		
		int idx=0;
		while(key > tmpNode->key[idx] && idx < this->order)idx++;
		for(int j=this->order;j>idx;j--)tmpNode->key[j]=tmpNode->key[j-1];
		for(int j=this->order+1;j>idx+1;j--)tmpNode->ptr[j]=tmpNode->ptr[j-1];
		tmpNode->key[idx]=key;
		tmpNode->ptr[idx+1]=newLeaf;

		Node *newInternal=new Node(this->order);
		newInternal->isleaf=false;
		cursor->size=(this->order+1)/2;
		newInternal->size=this->order - (this->order+1)/2;
		for(int i=0;i<newInternal->size;i++)newInternal->key[i]=tmpNode->key[i+cursor->size+1];
		for(int i=0;i<newInternal->size+1;i++)newInternal->ptr[i]=tmpNode->ptr[i+cursor->size+1];

		if(cursor==root){
			Node *newRoot=new Node(this->order);
			newRoot->key[0]=key;
			newRoot->ptr[0]=cursor;
			newRoot->ptr[1]=newInternal;
			newRoot->isleaf=false;
			newRoot->size=1;
			root=newRoot;
		}
		else {
			insertInternal(key,findParent(root,cursor),newInternal);
		}
	}
}

Node *BPtree::findParent(Node *cursor,Node *child){
	if(cursor->isleaf || (cursor->ptr[0])->isleaf)return NULL;
	
	Node *parent;	
	for(int i=0;i<cursor->size+1;i++){
		if(cursor->ptr[i]==child)return cursor;
		else {
			parent=findParent(cursor->ptr[i],child);
			if(parent!=NULL)return parent;
		}
	}
	return NULL;
}

void BPtree::setLeftPointer(Node *target,Node *newLeaf){
	Node *cursor=this->root;
	while(!cursor->isleaf)cursor=cursor->ptr[cursor->size];
	while(cursor){
		if(cursor==target)return;
		if(cursor->lptr==target){
			cursor->lptr=newLeaf;
			return;
		}
		else cursor=cursor->lptr;
	}
}

void BPtree::showList(int director){
	Node *cursor=this->root;
	Node *last;
	if(director==1)while(!cursor->isleaf)cursor=cursor->ptr[0];
	else while(!cursor->isleaf)cursor=cursor->ptr[cursor->size];
	while(cursor){
		if(director==1)for(int i=0;i<cursor->size;i++)cout << cursor->key[i] << " ";
		else for(int i=cursor->size-1;i>=0;i--)cout << cursor->key[i] << " ";
		last=cursor;
		if(director==1)cursor=cursor->rptr;
		else cursor=cursor->lptr;
		if(cursor==last)break;
	}
	cout << endl;
}

void BPtree::display(Node *cursor){
	if(cursor==NULL && cursor==root)cout << "1:[]" << endl;
	else if(cursor==NULL)return;

	if(cursor){
		for(int j=0;j<level;j++)cout << "\t";
		level++;
		cout << level << ".";
		cout << lebel << ": [" << cursor->key[0];
		lebel++;
		for(int i=1;i<cursor->size;i++)cout << ", " << cursor->key[i];
		cout << "]" << endl;
		if(cursor->isleaf==true){
			for(int i=0;i<cursor->size;i++){
				for(int j=0;j<level;j++)cout << "\t";
				cout << cursor->key[i] << ": " << cursor->value[i] << endl;;
			}
		}
		else{
			for(int i=0;i<cursor->size+1;i++){
				lebel=i+1;
				this->display(cursor->ptr[i]);
			}
		}
	}
	level--;
	lebel=1;
}
