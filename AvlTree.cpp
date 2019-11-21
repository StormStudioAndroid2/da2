#include "AvlTree.hpp"

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
node* AddElement(node* p, char k[KEY_SIZE],unsigned long long value ) {
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
		p->left = AddElement(p->left,k,value);
	else if ( strcmp(k,p->key)>0 ) {
		p->right = AddElement(p->right,k,value);
    }
	return BalanceTree(p);
}
node* FindMinimum(node* p)  {
    if (p->left==0) {
        return p;
    }
    return FindMinimum(p->left);
}
node* RemoveMinimum(node* p) 
{
	if( p->left==0 )
		return p->right;
	p->left = RemoveMinimum(p->left);
	return BalanceTree(p);
}
node* RemoveElement(node* p, char k[256]) {
    if (!p) {
        return 0;
    }
	if( strcmp(k,p->key)<0) {
		p->left = RemoveElement(p->left,k);
    }
	else if( strcmp(k,p->key)>0 ) {
		p->right = RemoveElement(p->right,k);
    }
    else {
		node* q = p->left;
		node* r = p->right;
		free(p);
        if (r==0) {
            return q;
        }
		node* min = FindMinimum(r);
		min->right = RemoveMinimum(r);
		min->left = q;
		return BalanceTree(min);
	}
}
node* FindByKey(node* p, char key[KEY_SIZE]) {
    if (p==0) {
        return 0;
    }
    if (strcmp(key,p->key)<0) {
        return FindByKey(p->left,key);
    }
    if (strcmp(key,p->key)>0) {
        return FindByKey(p->right,key);
    }
    return p;
}
