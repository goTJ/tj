// vim:ft=cpp:foldmethod=marker
#include<cstdio>
#ifndef TREE_H
#define TREE_H

// {{{ template<typename T> typename BiTree{
template<typename T> class BiTree{
	// {{{ struct Node{
	struct Node{
		Node *parent;
		Node *left;
		Node *right;
		T val;
		Node(T val, Node *parent=0, Node *left=0, Node *right=0);
	};
	// }}} struct Node{
public:
	BiTree();
	void insert(T val);
	void remove(T val);
	void display();
private:
	Node *successor(Node *p);
	Node *index(T val);
	Node *root;
};
// }}} template<typename T> typename BiTree{

// {{{ template<typename T> BiTree<T>::Node::Node(T val, Node *parent, Node *left, Node *right)
template<typename T> BiTree<T>::Node::Node(T val, Node *parent, Node *left, Node *right)
{
	this->val = val;
	this->parent = parent;
	this->left = left;
	this->right = right;
}
// }}} template<typename T> BiTree<T>::Node::Node(T val, Node *parent, Node *left, Node *right)

// {{{ template<typename T> BiTree<T>::BiTree()
template<typename T> BiTree<T>::BiTree()
{
	root = 0;
}
// }}} template<typename T> BiTree<T>::BiTree()

// {{{ template<typename T> void BiTree<T>::insert(T val)
template<typename T> void BiTree<T>::insert(T val)
{
	if(root == 0){
		root = new Node(val);
		return;
	}
	Node *p=root;
	do{
		if(val > p->val){
			if(p->right == 0){
				p->right = new Node(val, p, 0, 0);
				return;
			}
			p = p->right;
		}else if(val < p->val){
			if(p->left == 0){
				p->left = new Node(val, p, 0, 0);
				return;
			}
			p = p->left;
		}else
			return;
	}while(1);
}
// }}} template<typename T> void BiTree<T>::insert(T val)

// {{{ template<typename T> typename BiTree<T>::Node *BiTree<T>::successor(Node *p)
template<typename T> typename BiTree<T>::Node *BiTree<T>::successor(Node *n)
{
	if(n->right){
		Node *p=n->right;
		while(p->left)
			p = p->left;
		return p;
	}
	while(n->parent && n->parent->right == n)
		n = n->parent;
	return n->parent;
}
// }}} template<typename T> typename BiTree<T>::Node *BiTree<T>::successor(Node *p)

// {{{ template<typename T> typename BiTree<T>::Node *BiTree<T>::index(T val)
template<typename T> typename BiTree<T>::Node *BiTree<T>::index(T val)
{
	Node *p=root;

	while(p){
		if(p->val < val)
			p = p->right;
		else if(p->val > val)
			p = p->left;
		else
			return p;
	}
	return 0;
}
// }}} template<typename T> typename BiTree<T>::Node *BiTree<T>::index(T val)

// {{{ template<typename T> void BiTree<T>::remove(T val)
template<typename T> void BiTree<T>::remove(T val)
{
	Node *ori=index(val);

	if(ori){
		Node *bye, *child;
		bye = (ori->left && ori->right) ? successor(ori) : ori;
		ori->val = bye->val;
		child = (bye->left) ? bye->left : bye->right;
		if(bye->parent){
			if(bye == bye->parent->left)
				bye->parent->left = child;
			else
				bye->parent->right = child;
			if(child)
				child->parent = bye->parent;
		}else{
			root = child;
		}
		delete bye;
	}
}
// }}} template<typename T> void BiTree<T>::remove(T val)

template<typename T> void BiTree<T>::display()
{
	printf("%d %d %d\n", root->val, root->val, root->val);
}

int main()
{
	BiTree<int> a;

	a.insert(15);
	a.insert(13);
	a.insert(18);
	a.remove(15);
	a.display();
	return 0;
}
#endif
