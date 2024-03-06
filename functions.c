/* STEFAN Alexandru - 315CB */

#include "functions.h"
#include <stdlib.h>
#include <stdio.h>

/*functie ce genereaza arborele de compresie pe baza matricei
imaginii. are ca ultimi parametrii coordonatele coltului
din stanga sus si cel din dreapta jos*/
TTree Generate_tree(TTree quadtree, TColor **matrix, int factor,
int width_s, int height_s, int width_f, int height_f) {
    quadtree = (TTree)malloc(sizeof(TNode));
    if(!quadtree) {
        return NULL;
    }
    //calcularea valorii medii a culorii
    long long int red = 0, green = 0, blue = 0;
    for (int i = height_s - 1; i < height_f; i++) {
        for (int j = width_s - 1; j < width_f; j++) {
            red += matrix[i][j].red;
            green += matrix[i][j].green;
            blue += matrix[i][j].blue;
        }
    }
    red /= ((height_f-height_s+1)*(width_f-width_s+1));
    green /= ((height_f-height_s+1)*(width_f-width_s+1));
    blue /= ((height_f-height_s+1)*(width_f-width_s+1));
    long long mean = 0;
    //calcularea mean-ului
    for (int i = height_s - 1; i < height_f; i++) {
        for (int j = width_s - 1; j < width_f; j++) {
            mean += (red - matrix[i][j].red)*(red - matrix[i][j].red) +
            (green - matrix[i][j].green)*(green - matrix[i][j].green) +
            (blue - matrix[i][j].blue)*(blue - matrix[i][j].blue);
        }
    }
    mean /= 3*((height_f-height_s+1)*(width_f-width_s+1));
    //construire arborbore in functie de mean
    if(mean <= factor) {
        quadtree->child1 = NULL;
        quadtree->child2 = NULL;
        quadtree->child3 = NULL;
        quadtree->child4 = NULL;
        quadtree->info.red = red;
        quadtree->info.green = green;
        quadtree->info.blue = blue;
        quadtree->type = 0;
        return quadtree;
    } else {
        quadtree->info.red = 0;
        quadtree->info.green = 0;
        quadtree->info.blue = 0;
        quadtree->type = 1;
        quadtree->child1 = Generate_tree(quadtree->child1, matrix, factor,
        width_s, height_s, (width_s+width_f)/2, (height_s+height_f)/2);
        quadtree->child2 = Generate_tree(quadtree->child2, matrix, factor,
        (width_s+width_f)/2+1, height_s, width_f, (height_s+height_f)/2);
        quadtree->child3 = Generate_tree(quadtree->child3, matrix, factor,
        (width_s+width_f)/2+1, (height_s+height_f)/2+1, width_f, height_f);
        quadtree->child4 = Generate_tree(quadtree->child4, matrix, factor,
        width_s, (height_s+height_f)/2+1, (width_s+width_f)/2, height_f);
        return quadtree;
    }
}

//functie de dezaloca memoria acordata pentru arbore
void Free_tree(TTree *quadtree) {
    if(!*quadtree) {
        return;
    }
    Free_tree(&(*quadtree)->child1);
    Free_tree(&(*quadtree)->child2);
    Free_tree(&(*quadtree)->child3);
    Free_tree(&(*quadtree)->child4);
    free(*quadtree);
    *quadtree = NULL;
}

//functie ce printeza arborele(folosita pt debugging)
void Print_tree(TTree quadtree) {
    if(quadtree == NULL) {
        return;
    }
    printf("{type = %u, r:%d, g:%d, b:%d}",
    quadtree->type, quadtree->info.red, quadtree->info.green, quadtree->info.blue);
    Print_tree(quadtree->child1);
    Print_tree(quadtree->child2);
    Print_tree(quadtree->child3);
    Print_tree(quadtree->child4);
    printf("\n");
}

//functie maxim a 2 nr
int Max_2 (int a, int b) {
    if (a < b) {
        return b;
    } else {
        return a;
    }
}

//functie min a doua nr
int Min_2 (int a, int b) {
    if (a > b) {
        return b;
    } else {
        return a;
    }
}

//functie max a 4 nr
int Max_4(int a, int b, int c, int d) {
    return Max_2(a,Max_2(b,Max_2(c,d)));
}

//functie min a 4 nr
int Min_4(int a, int b, int c, int d) {
    return Min_2(a,Max_2(b,Max_2(c,d)));
}

//functie ce determina nr de niveluri dintr-un arbore
int Number_levels(TTree quadtree) {
    if (!quadtree) {
        return 0;
    }
    int levels1 = Number_levels(quadtree->child1);
    int levels2 = Number_levels(quadtree->child2);
    int levels3 = Number_levels(quadtree->child3);
    int levels4 = Number_levels(quadtree->child4);
    return 1 + Max_4(levels1,levels2,levels3,levels4);
}

//functie ce determina nr. de frunze dintr-un arbore
int Number_blocks_similarity(TTree quadtree) {
    int counter = 0;
    if (quadtree->type == 0) {
        counter++;
    } else {
        counter += Number_blocks_similarity(quadtree->child1);
        counter += Number_blocks_similarity(quadtree->child2);
        counter += Number_blocks_similarity(quadtree->child3);
        counter += Number_blocks_similarity(quadtree->child4);        
    }
    return counter;
}

//functie ce determina nivelul minim al pe care se afla o frunza in arbore
int Min_level(TTree quadtree) {
    if (!quadtree) {
        return 0;
    }
    int min_level = 1 << (sizeof(int) - 1);
    int levels1 = Min_level(quadtree->child1);
    if(levels1 >= 0) {
        min_level = Min_2(min_level, levels1 + 1);
    }
    int levels2 = Min_level(quadtree->child2);
    if(levels2 >= 0) {
        min_level = Min_2(min_level, levels2 + 1);
    }
    int levels3 = Min_level(quadtree->child3);
    if(levels3 >= 0) {
        min_level = Min_2(min_level, levels3 + 1);
    }
    int levels4 = Min_level(quadtree->child4);
    if(levels4 >= 0) {
        min_level = Min_2(min_level, levels4 + 1);
    }
    return min_level;
}

//functie ce determina dimensiunea blocului cu dimensiune maxima
int Max_block_size(TTree quadtree, int size) {
    for (int i = 1; i < Min_level(quadtree); i++) {
        size/=2;
    }
    return size;
}

//functie de push in coada a info de tip arbore 
void PushQueue(TQueue queue, TTree node_tree) {
    if (queue->finish == NULL) {
        queue->finish = (TList)malloc(sizeof(TNodeList));
        queue->start = queue->finish;
        queue->finish->info = node_tree;
        queue->finish->next = NULL;
    } else {
        TList aux = (TList)malloc(sizeof(TNodeList));
        aux->next = NULL;
        aux->info = node_tree;
        queue->finish->next = aux;
        queue->finish = aux;
    }
}

//functie de pop in coada a info de tip arbore
int PopQueue(TQueue queue, TTree *node_tree) {
    if (queue->start != NULL) {
        if(queue->start == queue->finish) {
            *node_tree = queue->start->info;
            TList aux = queue->start;
            queue->start = NULL;
            queue->finish = NULL;
            free(aux);
            return 1;
        }
        *node_tree = queue->start->info;
        TList aux = queue->start;
        queue->start = queue->start->next;
        free(aux);
        return 1;
    } else {
        return 0;
    }
}

//functie ce generaza fisierul de compresie a unui arbore de compresie dat
void Generate_compression_file(TTree quadtree, FILE *output, unsigned int size) {
    TQueue queue = (TQueue)malloc(sizeof(TQueueNode));
    queue->finish = NULL;
    queue->start = NULL;
    TTree aux = NULL;
    unsigned char zero = 0, one = 1;
    PushQueue(queue, quadtree);
    fwrite(&(size), sizeof(unsigned int), 1, output);
    while (PopQueue(queue, &aux) == 1) {
        if (aux != NULL) {
            PushQueue(queue, aux->child1);
            PushQueue(queue, aux->child2);
            PushQueue(queue, aux->child3);
            PushQueue(queue, aux->child4);
            if (aux->type == 1) {
                fwrite(&(zero), sizeof(unsigned char), 1, output);
            } else {
                fwrite(&(one), sizeof(unsigned char), 1, output);
                fwrite(&(aux->info.red), sizeof(unsigned char), 1, output);
                fwrite(&(aux->info.green), sizeof(unsigned char), 1, output);
                fwrite(&(aux->info.blue), sizeof(unsigned char), 1, output);
            }
        }
    }
    free(queue);
}

//functie ce aloca un element de tip TTree si initializeaza
//toate valorile cu 0
TTree Aloc_node_tree() {
    TTree node = (TTree)malloc(sizeof(TNode));
    if(!node) {
        return NULL;
    }
    node->child1 = NULL; 
    node->child2 = NULL; 
    node->child3 = NULL; 
    node->child4 = NULL; 
    node->info.red = 0;
    node->info.green = 0;
    node->info.blue = 0;
    node->type = 0;
    return node;
}

//functie ce genereaza arborele de compriesie pe baza 
// fisierului de compresie
void Generate_tree_2(TTree quadtree, FILE *input) {
    TQueue queue = (TQueue)malloc(sizeof(TQueueNode));
    queue->start = NULL;
    queue->finish = NULL;
    TTree aux;
    PushQueue(queue, quadtree);
    while (PopQueue(queue, &aux)) {
        fread(&(aux->type), sizeof(unsigned char), 1, input);
        if(aux->type == 0) {
            aux->info.red = 0;
            aux->info.green = 0;
            aux->info.blue = 0;
            aux->child1 = Aloc_node_tree();
            aux->child2 = Aloc_node_tree();
            aux->child3 = Aloc_node_tree();
            aux->child4 = Aloc_node_tree();
            PushQueue(queue, aux->child1);
            PushQueue(queue, aux->child2);
            PushQueue(queue, aux->child3);
            PushQueue(queue, aux->child4);
        } else {
            fread(&(aux->info.red), sizeof(unsigned char), 1, input);
            fread(&(aux->info.green), sizeof(unsigned char), 1, input);
            fread(&(aux->info.blue), sizeof(unsigned char), 1, input);
            aux->child1 = NULL;
            aux->child2 = NULL;
            aux->child3 = NULL;
            aux->child4 = NULL;
        }
    }
    free(queue);
}

//functie ce genereaza matricea imaginii pe baza arborelui de compresie
void Generare_tree_matrix(TTree quadtree, TColor **matrix, int size, int poz_x, int poz_y) {
    if(quadtree->type == 1) {
        for(int i = poz_x; i < size + poz_x; i++) {
            for (int j = poz_y; j < size + poz_y; j++) {
                matrix[i][j] = quadtree->info;
            }
        }
    } else {
        Generare_tree_matrix(quadtree->child1, matrix, size/2, poz_x, poz_y);
        Generare_tree_matrix(quadtree->child2, matrix, size/2, poz_x + size/2, poz_y);
        Generare_tree_matrix(quadtree->child3, matrix, size/2, poz_x + size/2, poz_y + size/2);
        Generare_tree_matrix(quadtree->child4, matrix, size/2, poz_x, poz_y + size/2);
    }
}

//functie ce genereaza imaginea pe baza matricei a imaginii
void Generate_image_from_matrix(TColor **matrix, int size, FILE *output) {
    fprintf(output, "P6\n%u %u\n255\n", size, size);
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            fwrite(&(matrix[j][i].red), sizeof(unsigned char), 1, output);
            fwrite(&(matrix[j][i].green), sizeof(unsigned char), 1, output);
            fwrite(&(matrix[j][i].blue), sizeof(unsigned char), 1, output);
        }
    }
}