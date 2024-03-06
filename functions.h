/* STEFAN Alexandru - 315CB */

#include <stdio.h>

//structura culoare
typedef struct color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} TColor;

//structura arbore cu 4 fii
typedef struct tree {
    TColor info;
    unsigned char type;
    struct tree *child1, *child2, *child3, *child4;
} TNode,*TTree;

//structura lista cu info de timp arb(pt coada)
typedef struct list {
    TTree info;
    struct list *next;
}TNodeList, *TList;

//structura coada
typedef struct queue {
    TList start, finish;
}TQueueNode, *TQueue;

TTree Generate_tree(TTree quadtree, TColor **matrix, int factor,
int width_s, int height_s, int width_f, int height_f);
void Print_tree(TTree quadtree);
void Free_tree(TTree *quadtree);
void Analyse_tree(TTree quadtree);
int Max_2 (int a, int b);
int Min_2 (int a, int b);
int Max_4(int a, int b, int c, int d);
int Min_4(int a, int b, int c, int d);
int Number_levels(TTree quadtree);
int Number_blocks_similarity(TTree quadtree);
int Max_block_size(TTree quadtree, int size);
int Min_level(TTree quadtree);
void PushQueue(TQueue queue, TTree node_tree);
int PopQueue(TQueue queue, TTree *node_tree);
void Generate_compression_file(TTree quadtree, FILE *output, unsigned int size);
TTree Aloc_node_tree();
void Generate_tree_2(TTree quadtree, FILE *input);
void Generare_tree_matrix(TTree quadtree, TColor **matrix, int size, int poz_x, int poz_y);
void Generate_image_from_matrix(TColor **matrix, int size, FILE *output);