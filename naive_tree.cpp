#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


const size_t MIN_ARRAY_SIZE = 100000;
const size_t MAX_ARRAY_SIZE = 1000000;
const size_t STEP = 100000;
const size_t TEST_COUNT = 20;


typedef int data_t;

typedef struct Node {
    data_t key = 0;
    Node *left = nullptr;
    Node *right = nullptr;
} Node;

typedef struct {
    Node *root = nullptr;
} Tree;

int tree_constructor(Tree *tree);

int tree_destructor(Tree *tree);

int tree_insert(Tree *tree, data_t key);

int tree_remove(Tree *tree, data_t key);

Node *node_constructor(data_t key);

void node_destructor(Node *node);

Node *node_remove(Node *node, data_t key);

Node *node_min(Node *node);




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
    
    tree -> root = nullptr;

    return 0;
}


int tree_destructor(Tree *tree) {
    assert(tree && "Can't destruct null ptr!\n");

    node_destructor(tree -> root);

    tree -> root = nullptr;

    return 0;
}


int tree_insert(Tree *tree, data_t key) {
    assert(tree && "Can't insert in null ptr!\n");

    Node *node = tree -> root;

    if (!node) {
        tree -> root = node_constructor(key);
        return 0;
    }

    while (node) {
        if (key > node -> key) {
            if (!node -> right) {
                node -> right = node_constructor(key);
                return 0;
            }

            node = node -> right;
        }
        else {
            if (!node -> left) {
                node -> left = node_constructor(key);
                return 0;
            }

            node = node -> left;
        }
    }

    return 1;
}


int tree_remove(Tree *tree, data_t key) {
    assert(tree && "Can't erase in null ptr!\n");

    tree -> root = node_remove(tree -> root, key);

    return 0;
}


Node *node_constructor(data_t key) {
    Node *new_node = (Node *) calloc(1, sizeof(Node));
    assert(new_node && "Failed to allocate new node!\n");
    new_node -> key = key;
    return new_node;
}


void node_destructor(Node *node) {
    if (!node) return;

    node_destructor(node -> left);
    node_destructor(node -> right);

    node -> left = nullptr;
    node -> right = nullptr;

    free(node);
}


Node *node_remove(Node *node, data_t key) {
    if (!node) return nullptr;

    if (key > node -> key) {
        node -> right = node_remove(node -> right, key);
    }
    else if (key < node -> key) {
        node -> left = node_remove(node -> left, key);
    }
    else if (node -> left && node -> right) {
        node -> key = node_min(node -> right) -> key;
        node -> right = node_remove(node -> right, node -> key);
    }
    else {
        if (node -> left) node = node -> left;
        else if (node -> right) node = node -> right;
        else node = nullptr;
    }

    return node;
}


Node *node_min(Node *node) {
    while (node && node -> left) node = node -> left;
    return node;
}
