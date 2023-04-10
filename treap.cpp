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
    int priority = 0;
    data_t key = 0;
    struct Node *left = nullptr;
    struct Node *right = nullptr;
};

typedef struct Node Node;

typedef struct {
    Node *root = nullptr;
} Tree;

typedef struct {
    Node *L = nullptr;
    Node *R = nullptr;
} NodePair;

int tree_constructor(Tree *tree);

int tree_destructor(Tree *tree);

int tree_insert(Tree *tree, data_t key);

int tree_remove(Tree *tree, data_t key);

Node *tree_find(Tree *tree, data_t key);

Node *create_node(data_t key);

void free_node(Node *node);

Node *merge(Node *L, Node *R);

NodePair split(Node *node, data_t key);

Node *find_min(Node *node);

Node *find_max(Node *node);




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

    free_node(tree -> root);

    tree -> root = nullptr;

    return 0;
}


int tree_insert(Tree *tree, data_t key) {
    assert(tree && "Can't insert in null ptr!\n");

    NodePair pair = split(tree -> root, key);

    Node *new_node = create_node(key);
    tree -> root = merge(merge(pair.L, new_node), pair.R);

    return 0;
}


int tree_remove(Tree *tree, data_t key) {
    assert(tree && "Can't erase in null ptr!\n");

    NodePair left_pair = split(tree -> root, key);
    NodePair right_pair = split(left_pair.R, key + 1);

    if (right_pair.L) {
        Node *old_node = right_pair.L;
        right_pair.L = merge(old_node -> left, old_node -> right);
        
        tree -> root = merge(left_pair.L, merge(right_pair.L, right_pair.R));

        free(old_node);
    }
    else {
        tree -> root = merge(left_pair.L, right_pair.R);
    }

    return 0;
}


Node *create_node(data_t key) {
    Node *new_node = (Node *) calloc(1, sizeof(Node));
    assert(new_node && "Failed to allocate new node!\n");
    new_node -> priority = rand();
    new_node -> key = key;
    return new_node;
}


void free_node(Node *node) {
    if (!node) return;

    free_node(node -> left);
    free_node(node -> right);

    node -> left = nullptr;
    node -> right = nullptr;

    free(node);
}


Node *merge(Node *L, Node *R) {
    if (!L) return R;
    if (!R) return L;

    if (R -> priority >= L -> priority) {
        R -> left = merge(L, R -> left);
        return R;
    }
    else {
        L -> right = merge(L -> right, R);
        return L;
    }
}


NodePair split(Node *node, data_t key) {
	NodePair result = {nullptr, nullptr};

    if (!node) return result;

    if (key > node -> key) {
        NodePair pair = split(node -> right, key);
        node -> right = pair.L;
    	result.L = node;
    	result.R = pair.R;
    }
    else {
        NodePair pair = split(node -> left, key);
        node -> left = pair.R;
    	result.L = pair.L;
    	result.R = node;
    }

	return result;
}


Node *find_min(Node *node) {
    for (; node && node -> left; node = node -> left);
    return node;
}


Node *find_max(Node *node) {
    for (; node && node -> right; node = node -> right);
    return node;
}


Node *tree_find(Tree *tree, data_t key) {
    Node *node = tree -> root;

    while (node) {
        if (key < node -> key) node = node -> left;
        else if (key > node -> key) node = node -> right;
        else break;
    }

    return node;
}
