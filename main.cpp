#include <iostream>
#include <string.h>
#include <fstream>
#include <stdlib.h>

const int KEY_SIZE = 260;
const char ADD = '+';
const char DELETE = '-';
const char FILE_OPERATION = '!';
const int PATH_SIZE = 1000;

struct TNode {
    unsigned long long Key; 
    unsigned long long Value; 
    TNode* Left;
    long long Height;
    TNode* Right;
TNode(unsigned long long  key1, unsigned long long value1) {
    Value = value1; 
    Key = key1;   
    Height = 1;
    Left = 0;
    Right = 0;  
}

};

long long GetHeight(TNode* n) {
    if (n!=NULL) {
        return n->Height;
    }
    return 0;
}
long long GetBalance(TNode* n) {
    return GetHeight(n->Right)-GetHeight(n->Left);
}

void CountHeight(TNode* n)
{
	int hl = GetHeight(n->Left);
	int hr = GetHeight(n->Right);
	n->Height = (hl>hr?hl:hr)+1;
}
TNode* RotateLeft(TNode* q) {
    TNode*p = q->Right;

    q->Right = p->Left;
    p->Left = q;
    CountHeight(q);
    CountHeight(p);
    return p;

}
TNode* RotateRight(TNode* q) {
    TNode* p = q->Left;
    q->Left = p->Right;
    p->Right = q;
    CountHeight(q);
    CountHeight(p);
    return p;
}
TNode* BalanceTree(TNode* p) {
    CountHeight(p);
    if (GetBalance(p)==2) {
        TNode* q = p->Right;
        if (GetBalance(q)<0) {
            p->Right = RotateRight(q);
        } 
        return RotateLeft(p);
    }
    if (GetBalance(p)==-2) {
        TNode* q = p->Left;
        if (GetBalance(q)>0) {
            p->Left=RotateLeft(q);
        } 
        return RotateRight(p);
    }
    return p;
}

TNode* AddElement(TNode* p, unsigned long long k,unsigned long long value ) {
	if( !p ) {
        return new TNode(k,value);   
    }
    if (k<p->Key) {
		p->Left = AddElement(p->Left,k,value);
    }
	else if (k>p->Key) {
		p->Right = AddElement(p->Right,k,value);
    }
	return BalanceTree(p);
}
TNode* FindMinimum(TNode* p)  {
    if (p->Left==0) {
        return p;
    }
    return FindMinimum(p->Left);
}
TNode* RemoveMinimum(TNode* p) 
{
	if( p->Left==0 ) {
		return p->Right;
    }
	p->Left = RemoveMinimum(p->Left);
	return BalanceTree(p);
}
TNode* RemoveElement(TNode* p, unsigned long long k) {
    if (!p) {
        return 0;
    }
	if(k<p->Key) {
		p->Left = RemoveElement(p->Left,k);
    }
	else if(k>p->Key) {
		p->Right = RemoveElement(p->Right,k);
    }
	if (k==p->Key) {
        TNode* q = p->Left;
		TNode* r = p->Right;
		delete p;
        if (r==0) {
            return q;
        }
		TNode* min = FindMinimum(r);
		min->Right = RemoveMinimum(r);
		min->Left = q;
		return BalanceTree(min);
    }
    return BalanceTree(p);
	
}
void RemoveTree(TNode* p) {
    if (!p) {
        return;
    }
    RemoveTree(p->Left);
    RemoveTree(p->Right);
    delete p;
}
TNode* FindByKey(TNode* p, unsigned long long key) {
    if (p==0) {
        return 0;
    }
  
    if (key<p->Key) {
        return FindByKey(p->Left,key);
    }
    if (key>p->Key) {
        return FindByKey(p->Right,key);
    }
    return p;
}
std::ofstream fs;
std::ifstream input;
unsigned long long Key;
unsigned long long value;
void WriteTree(char path[PATH_SIZE], TNode* p) {
    
    if (p==0) {
        unsigned long long v = 0;
        unsigned long long k = 0;

        fs.write((char*)(&k), sizeof(unsigned long long ));
        fs.write((char*)(&v), sizeof (unsigned long long ));
        return;
    }
    fs.write((char*)(&p->Key), sizeof(unsigned long long));
    fs.write((char*)(&p->Value), sizeof (unsigned long long));
    WriteTree(path,p->Left);
    WriteTree(path,p->Right);    
  
}
TNode* ReadTree() {
    TNode* q = 0;
    if (input.read((char*)(&Key), sizeof(unsigned long long int))) {
        input.read((char*)(&value), sizeof (unsigned long long int));
        if (Key==0) {
            return q;
        }
        q = new TNode(Key,value);
    }
    q->Left = ReadTree();
    q->Right = ReadTree();    
    return q;
}
char command[KEY_SIZE];
char operation[KEY_SIZE];
unsigned long long GetHash(char key[KEY_SIZE]) {
    int i = 0;
    unsigned long long a=0;
    while ((key[i]))
    {
        a*=28;
        a+=(key[i]-('a'-1));
        ++i;
    }
    return a;
}

int main() {
    TNode* p = 0;
    char word[KEY_SIZE];
    fs.sync_with_stdio(false);
    fs.tie(nullptr);

    input.sync_with_stdio(false);
    input.tie(nullptr);
    std::ios::sync_with_stdio(false);

    std::cin.tie(nullptr);
    
    while (std::cin >> command) {
        if (command[0]==ADD) {
            std::cin >> word;
            int i = 0;
            while (word[i]) {
                word[i] = tolower(word[i]);
                i++;
            }
            unsigned long long num; 
            std::cin >> num;

            if (FindByKey(p,GetHash(word))==0) {
                p = AddElement(p,GetHash(word),num);
                std::cout << "OK" << std::endl;
            } else {
                std::cout << "Exist" << std::endl;
            }
        }
        else if (command[0]==DELETE) {
            std::cin >> word;
            int i = 0;
            while (word[i]) {
                word[i] = tolower(word[i]);
                i++;
            }
            if (FindByKey(p,GetHash(word))==0) {
                std::cout << "NoSuchWord" << std::endl;
            } else {
                p = RemoveElement(p,GetHash(word));
                std::cout << "OK" <<  std::endl;
            }
        }
        else if (command[0]==FILE_OPERATION) {
            std::cin >> operation;
            if (operation[0]=='S') {

                char path[PATH_SIZE];
                std::cin >> path;
                try {
                    fs.open(path, std::ios::out | std::ios::binary);
                    fs.sync_with_stdio(false);
                    fs.tie(nullptr);
                    WriteTree(path,p);
                    fs.close();
                    
                        std::cout << "OK" << std::endl;
                
                }
                catch (const std::exception &e) {
                    std::cout<< "ERROR: ";
                    std::cerr << e.what();
                    std::cout << std::endl;
                }   
            }
            if (operation[0]=='L') {
                try {
                    char path[PATH_SIZE];
                    std::cin >> path;
                    TNode* q = 0;
                    input.open(path, std::ios::binary);
                    input.sync_with_stdio(false);
                    input.tie(nullptr);
                    input.seekg(0,std::ios::beg);
                    if (input) {
                        q = ReadTree();
                        input.close();
                        RemoveTree(p);
                        p = q;
                    std::cout << "OK" << std::endl;
                } else {
                    std::cout<< "ERROR: cannot read from file" << std::endl;

                }

                }
                catch (const std::exception &e) {
                    std::cout<< "ERROR: ";
                    std::cerr << e.what();
                    std::cout << std::endl;

                }                
            }

        }
        else {
            int i = 0;
            while (command[i]) {
                command[i] = tolower(command[i]);
                i++;
            }
            TNode* q = FindByKey(p,GetHash(command));
            if (!q) {
                std::cout << "NoSuchWord" << std::endl;
            } else {
                std::cout << "OK: " << q->Value << std::endl;
            }
        }

    }
    RemoveTree(p);
}