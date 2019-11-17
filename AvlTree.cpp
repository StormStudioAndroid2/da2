#include "AvlTree.hpp"
#include <iostream>
#include <string.h>
int GetBalance(node* n) {
    return GetHeight(n->right)-GetHeight(n->left);
}
int GetHeight(node* n) {
    if (n!=NULL) {
        return n->height;
    }
    return 0;
}
void CountHeight(node* n)
{
	int hl = GetHeight(n->left);
	int hr = GetHeight(n->right);
	n->height = (hl>hr?hl:hr)+1;
}
node* RotateLeft(node* q) {
    node*p = q->right;

    q->right = p->left;
    p->left = q;
    CountHeight(q);
    CountHeight(p);
    return p;

}
node* RotateRight(node* q) {
    node* p = q->left;
    q->left = p->right;
    p->right = q;
    CountHeight(q);
    CountHeight(p);
    return p;
}
node* BalanceTree(node* p) {
    if (GetBalance(p)==2) {
        node* q = p->right;
        if (GetHeight(q->left)<=GetHeight(q->right)) {
           return RotateLeft(p);
        } else {
            q=RotateRight(q);
        }
        return RotateLeft(p);
    }
    if (GetBalance(p)==-2) {
        node* q = p->left;
        if (GetHeight(q->right)<=GetHeight(q->left)) {
           return RotateRight(p);
        } else {
            q=RotateLeft(q);
        }
        return RotateRight(p);
    }
    return p;
}
node* insert(node* p, char k[KEY_SIZE],unsigned long long value ) {
	if( !p ) {
        node q;
        q.value = value;
        for (int i = 0; i<KEY_SIZE; ++i) {
            q.key[i] = k[i]; 
        }
        q.height = 0;
        q.left = 0;
        q.right = 0;
        return &q;
    }
	if( strcmp(k,p->key)<0 )
		p->left = insert(p->left,k,value);
	else
		p->right = insert(p->right,k,value);
	return BalanceTree(p);
}