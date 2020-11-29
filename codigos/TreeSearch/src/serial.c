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

size_t ran(size_t l, size_t r) { 
    return (size_t) (l + (rand() % (r - l + 1))); 
} 

void print_sequence(size_t *sequence, int n){
    printf("%ld", sequence[0]);
    for (int i = 1; i < n; ++i) {
        printf(", %ld", sequence[i]);
    }
    printf("\n");
}

size_t* createPruferSequence(unsigned int seed, size_t true_size) {
    if (true_size < 2) {
        return NULL;
    }
    srand(seed);
    size_t size = true_size - 2;
    size_t* sequence = malloc(size*sizeof(size_t));
    for (size_t i = 0; i < size; ++i) {
        sequence[i] = ran(1, size); 
    }
    return sequence;
}

// https://stackoverflow.com/questions/14878228/creating-a-random-tree
// https://en.wikipedia.org/wiki/Pr%C3%BCfer_sequence
Node* convertPruferToTree(size_t* sequence, size_t size) {
    size_t true_size = size + 2;
    Node** tree = malloc(true_size*sizeof(Node*));
    for (size_t i = 0; i < true_size; ++i) {
        tree[i] = malloc(sizeof(Node));
        tree[i]->n_children = 1;
        tree[i]->parent = NULL;
        tree[i]->children = NULL;
        tree[i]->value = i+1;//(double) rand() / INT_MAX;
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

// https://en.wikipedia.org/wiki/Tree_traversal
// https://en.wikipedia.org/wiki/Breadth-first_search
// https://en.wikipedia.org/wiki/Depth-first_search
// https://en.wikipedia.org/wiki/Search_tree
// https://github.com/JohnVithor/EDB1-2/blob/master/Arvores/include/BinaryTree/BinaryTree.inl
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

    if (argc != 4) {
        printf("É necessário informar os seguintes argumentos:\n");
        return -1;
    }

    unsigned int seed = convert_str_long(argv[1]);
    size_t tree_size = convert_str_long(argv[2]);
    size_t tree_number = convert_str_long(argv[3]);

    Node** trees = malloc(tree_number*sizeof(Node*));

    for (size_t i = 0; i < tree_number; ++i) {
        size_t* sequence = createPruferSequence(seed, tree_size);    
        print_sequence(sequence, tree_size - 2);
        trees[i] = convertPruferToTree(sequence, tree_size-2);
        free(sequence);
        seed = seed + rand();
    }
   
    //depthFirstSearch(tree, -1);

    for (size_t i = 0; i < tree_number; ++i) {
        destroyTree(trees[i]);
    }
    free(trees);
    
    return 0;
} /* main */