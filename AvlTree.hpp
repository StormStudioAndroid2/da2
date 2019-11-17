const char KEY_SIZE = 256;
struct node {
    char key[KEY_SIZE];
    unsigned long long value; 
    node* left;
    int height;
    node* right;

};

