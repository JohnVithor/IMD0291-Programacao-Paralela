#include <stdio.h>
#include <errno.h>  // for errno
#include <math.h>
#include <limits.h> // for INT_MAX
#include <stdlib.h> // for strtol
#include <time.h>

typedef struct Node
{
    struct Node* parent;
    double value;
    struct Node** children;
    size_t n_children;
} Node;

void printNode(Node* node){
    if(node->parent != NULL){
        printf("\nparent: %lf\n", node->parent->value);
    } else {
        printf("\nparent: NULL\n");
    }
    
    printf("value: %lf\n", node->value);
    printf("children: ");
    for (size_t i = 0; i < node->n_children; ++i) {
        printf("%lf ", node->children[i]->value);
    }
    printf("\n");
}


Node* convertPruferToTree(size_t* sequence, size_t size) {
    size_t true_size = size + 2;
    Node** tree = malloc(true_size*sizeof(Node*));
    for (size_t i = 0; i < true_size; ++i) {
        tree[i] = malloc(sizeof(Node));
        tree[i]->n_children = 1;
        tree[i]->parent = NULL;
        tree[i]->children = NULL;
        tree[i]->value = i+1;(double) rand() / INT_MAX;
    }
    
    size_t* degrees = malloc(true_size*sizeof(size_t));
    for (size_t i = 0; i < true_size; ++i) {
        degrees[i] = 1;
    }
    for (size_t i = 0; i < size; ++i) {
        ++(degrees[sequence[i]-1]);
        ++(tree[sequence[i]-1]->n_children);
    }
    for (size_t i = 0; i < true_size; ++i) {
        tree[i]->children = malloc(tree[i]->n_children*sizeof(Node*));
        for (size_t j = 0; j < tree[i]->n_children; ++j) {
            tree[i]->children[j] = NULL;
        }

    }
    size_t* child_counter = calloc(true_size,sizeof(size_t));
    for (size_t i = 0; i < size; ++i) {
        for (size_t j = 0; j < true_size; ++j) {
            if(degrees[j] == 1) {
                tree[sequence[i]-1]->children[child_counter[sequence[i]-1]] = tree[j];
                tree[j]->parent = tree[sequence[i]-1];
                ++child_counter[sequence[i]-1];
                --degrees[sequence[i]-1];
                --degrees[j];
                break;
            }
        }
    }
    size_t first = -1;
    size_t second = -1;
    for (size_t i = 0; i < true_size; ++i) {
        if(degrees[i] == 1) {
            if(first == -1){
                first = i;
            } else {
                second = i;
                break;
            }
        }
    }
    tree[first]->children[child_counter[first]] = tree[second];
    ++child_counter[first];
    tree[second]->parent = tree[first];
    --degrees[first];
    --degrees[second];

    Node* root = NULL;

    for (size_t i = 0; i < true_size; ++i){
        if(child_counter[i] == 0){
            free(tree[i]->children);
            tree[i]->n_children = 0;
        }else {
            tree[i]->n_children = child_counter[i];
        }
        if(tree[i]->parent == NULL){
            root = tree[i];
        }
    }
    
    free(degrees);
    free(child_counter);
    free(tree);

    return root;
}

void destroyTree(Node* tree) {
    if( tree->n_children == 0){
        free(tree);
        return;
    }
    for (size_t i = 0; i < tree->n_children; ++i){
        destroyTree(tree->children[i]);
    }
    free(tree->children);
    free(tree);
}


Node* depthFirstSearch(Node* tree, double value) {
    if (tree == NULL) {
        return NULL;
    }
    printNode(tree);
    if( tree->value == value){
        return tree;
    }
    if( tree->n_children == 0){
        return NULL;
    }
    for (size_t i = 0; i < tree->n_children; ++i){
        depthFirstSearch(tree->children[i], value);
    }
    return NULL;
}



long convert_str_long(char *str){
    char *p;
    errno = 0;
    long conv = strtol(str, &p, 10);

    if (errno != 0 || *p != '\0')
    {
        printf("%s não é um número!\n", str);
        exit(-1);
    }
    return (long)conv;
}

int main(int argc, char **argv){

    if (argc != 2) {
        printf("É necessário informar os seguintes argumentos:\n");
        return -1;
    }

    int show_matrix = convert_str_long(argv[1]);

    size_t array[4] = {1, 4, 4, 5};

    Node* tree = convertPruferToTree(array, 4);
    depthFirstSearch(tree, -1);
    destroyTree(tree);
    return 0;
} /* main */