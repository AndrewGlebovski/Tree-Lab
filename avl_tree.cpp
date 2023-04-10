#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const size_t MIN_ARRAY_SIZE = 100000;
const size_t MAX_ARRAY_SIZE = 1000000;
const size_t STEP = 100000;
const size_t TEST_COUNT = 20;


typedef int data_t;

struct Node {
    int height;
    data_t key;
    struct Node *left;
    struct Node *right;
};

typedef struct Node Node;

typedef struct {
    Node *root;
} Tree;

int tree_constructor(Tree *tree);

int tree_destructor(Tree *tree);

int tree_insert(Tree *tree, data_t key);

int tree_remove(Tree *tree, data_t key);

Node *node_insert(Node *node, data_t key);

Node *node_remove(Node *node, data_t key);

Node *rotate_left(Node *node);

Node *rotate_right(Node *node);

Node *create_node(data_t key);

void free_node(Node *node);

void update_height(Node *node);

int balance_factor(Node *node);

int height(Node *node);

Node *balance(Node *node);

Node *find_min(Node *node);

Node *detach_min(Node *node);




int main() {
    srand(time(NULL));

    data_t *arr = (data_t *) calloc(MAX_ARRAY_SIZE, sizeof(data_t));

    double result_insert_time = 0, result_erase_time = 0;

    for (size_t size = MIN_ARRAY_SIZE; size <= MAX_ARRAY_SIZE; size += STEP) {
        for (size_t test = 0; test < TEST_COUNT; test++) {
            for (size_t i = 0; i < size; i++) arr[i] = rand();

            Tree tree = {};
            tree_constructor(&tree);

            clock_t t1 = clock();

            for (size_t i = 0; i < size; i++)
                tree_insert(&tree, arr[i]);

            clock_t t2 = clock();

            result_insert_time += 1000.0 * (t2 - t1) / CLOCKS_PER_SEC;

            t1 = clock();

            for (size_t i = 0; i < size / 2; i++)
                tree_remove(&tree, arr[(size_t) rand() % size]);

            t2 = clock();

            result_erase_time += 1000.0 * (t2 - t1) / CLOCKS_PER_SEC;

            tree_destructor(&tree);
        }

        printf("Size: %lu\nInsert time: %lf\nErase time: %lf\n", size, result_insert_time / TEST_COUNT, result_erase_time / TEST_COUNT);
    }

    free(arr);

    return 0;
}




int tree_constructor(Tree *tree) {
    assert(tree && "Can't construct null ptr!\n");
    
    tree -> root = NULL;

    return 0;
}


int tree_destructor(Tree *tree) {
    assert(tree && "Can't destruct null ptr!\n");

    free_node(tree -> root);

    tree -> root = NULL;

    return 0;
}


int tree_insert(Tree *tree, data_t key) {
    assert(tree && "Can't insert in null ptr!\n");

    tree -> root = node_insert(tree -> root, key);

    return 0;
}


int tree_remove(Tree *tree, data_t key) {
    assert(tree && "Can't erase in null ptr!\n");

    tree -> root = node_remove(tree -> root, key);

    return 0;
}


Node *node_insert(Node *node, data_t key) {
    if (!node) return create_node(key);

    if (key == node -> key) return node;

    if (key > node -> key)
        node -> right = node_insert(node -> right, key);
    else
        node -> left = node_insert(node -> left, key);
    
    return balance(node);
}


Node *node_remove(Node *node, data_t key) {
    if (!node) return NULL;

    if (key < node -> key) {
        node -> left = node_remove(node -> left, key);
        return balance(node);
    }

    if (key > node -> key) {
        node -> right = node_remove(node -> right, key);
        return balance(node);
    }

    if (!node -> right) {
        Node *left = node -> left;
        free(node);
        return left;
    }

    Node *right_min = find_min(node -> right);
    node -> right = detach_min(node -> right);

    node -> key = right_min -> key;
    free(right_min);

    return balance(node);
}


Node *rotate_left(Node *node) {
    assert(node && "Can't rotate left with null node!\n");
    assert(node -> right && "Can't rotate left with null right node\n");

    Node *right = node -> right;
    node -> right = right -> left;
    right -> left = node;

    update_height(node);
    update_height(right);

    return right;
}

Node *rotate_right(Node *node) {
    assert(node && "Can't rotate right with null node!\n");
    assert(node -> left && "Can't rotate right with null left node\n");

    Node *left = node -> left;
    node -> left = left -> right;
    left -> right = node;

    update_height(node);
    update_height(left);

    return left;
}


Node *create_node(data_t key) {
    Node *new_node = (Node *) calloc(1, sizeof(Node));
    assert(new_node && "Failed to allocate new node!\n");
    new_node -> height = 1;
    new_node -> key = key;
    return new_node;
}


void free_node(Node *node) {
    if (!node) return;

    free_node(node -> left);
    free_node(node -> right);

    node -> left = NULL;
    node -> right = NULL;

    free(node);
}


void update_height(Node *node) {
    assert(node && "Can't update null node height!\n");

    int height_max = (height(node -> left) > height(node -> right)) ? height(node -> left) : height(node -> right);
    node -> height = height_max + 1;
}


int balance_factor(Node *node) {
    assert(node && "Can't get balance factor for null node height!\n");

    return height(node -> right) - height(node -> left);
}


int height(Node *node) {
    return (node) ? node -> height : 0;
}


Node *balance(Node *node) {
    assert(node && "Can't balance null node!\n");

    update_height(node);
    int bf = balance_factor(node);

    if (bf == -2) {
        if (balance_factor(node -> left) == 1)
            node -> left = rotate_left(node -> left);
        
        return rotate_right(node);
    }
    else if (bf == 2) {
        if (balance_factor(node -> right) == -1)
            node -> right = rotate_right(node -> right);
        
        return rotate_left(node);
    }

    return node;
}


Node *find_min(Node *node) {
    for (; node && node -> left; node = node -> left);
    return node;
}


Node *detach_min(Node *node) {
    if (!node) return NULL;

    if (!node -> left) return node -> right;

    node -> left = detach_min(node -> left);

    return balance(node);
}
