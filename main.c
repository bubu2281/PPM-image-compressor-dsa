/* STEFAN Alexandru - 315CB */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"

int main(int argc, char *argv[]) {
    if (!strcmp(argv[1], "-c1")) {
        int factor = atoi(argv[2]);
        FILE *input = fopen(argv[3], "rb");
        FILE *output = fopen(argv[4], "wt");
        char file_type[10];
        int width, height, max_color_value;
        //citirea tuturor datelor dinaintea matricei a imaginii
        fscanf(input, "%s", file_type);
        fscanf(input, "%d", &width);
        fscanf(input, "%d", &height);
        fscanf(input, "%d", &max_color_value);
        fgetc(input);
        //alocarea matricei a imaginii
        TColor **matrix = (TColor **)malloc(height * sizeof(TColor *));
        for (int i = 0; i < height; i++) {
            matrix[i] = (TColor *)malloc(width * sizeof(TColor));
        }
        //citirea matricicei din fisierul binar
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fread(&matrix[i][j], sizeof(TColor), 1, input);
            }
        }
        //construirea arborelui necompresat
        TTree quadtree;
        quadtree = Generate_tree(quadtree, matrix, factor, 1, 1, width, height);

        //cerinta 1 in fisierul de output
        fprintf(output, "%d\n", Number_levels(quadtree));
        fprintf(output, "%d\n", Number_blocks_similarity(quadtree));
        fprintf(output, "%d\n", Max_block_size(quadtree, width));

        //eliberare memorie arbore cuaternar
        Free_tree(&quadtree);
        //dezalocare memorie matrice a imaginii
        for (int i = 0; i < height; i++) {
            free(matrix[i]);
        }
        free(matrix);
        fclose(input);
        fclose(output);
    }
    if (!strcmp(argv[1], "-c2")) {
        int factor = atoi(argv[2]);
        FILE *input = fopen(argv[3], "rb");
        FILE *output = fopen(argv[4], "wb");
        char file_type[10];
        int width, height, max_color_value;
        //citirea tuturor datelor dinaintea matricei a imaginii
        fscanf(input, "%s", file_type);
        fscanf(input, "%d", &width);
        fscanf(input, "%d", &height);
        fscanf(input, "%d", &max_color_value);
        fgetc(input);
        //alocarea matricei a imaginii
        TColor **matrix = (TColor **)malloc(height * sizeof(TColor *));
        for (int i = 0; i < height; i++) {
            matrix[i] = (TColor *)malloc(width * sizeof(TColor));
        }
        //citirea matricicei din fisierul binar
        for (int i = 0; i < height; i++) {
            for (int j = 0; j < width; j++) {
                fread(&matrix[i][j], sizeof(TColor), 1, input);
            }
        }
        //construirea arborelui necompresat
        TTree quadtree;
        quadtree = Generate_tree(quadtree, matrix, factor, 1, 1, width, height);
        
        //cerinta 2 in fisierul de output
        Generate_compression_file(quadtree, output, width);

        //eliberare memorie arbore cuaternar
        Free_tree(&quadtree);
        //dezalocare memorie matrice a imaginii
        for (int i = 0; i < height; i++) {
            free(matrix[i]);
        }
        free(matrix);
        fclose(input);
        fclose(output);
    }
    if (!strcmp(argv[1], "-d")) {
        FILE *input = fopen(argv[2], "rb");
        FILE *output = fopen(argv[3], "wb");
        unsigned int size;
        //citirea dimensiunii imaginii
        fread(&size, sizeof(unsigned int), 1, input);
        //generarea arborelui
        TTree quadtree = Aloc_node_tree();
        Generate_tree_2(quadtree, input);

        //alocarea matricei a imaginii
        TColor **matrix = (TColor **)malloc(size * sizeof(TColor *));
        for (int i = 0; i < size; i++) {
            matrix[i] = (TColor *)malloc(size * sizeof(TColor));
        }
        //generarea matricei pe baza arborelui
        Generare_tree_matrix(quadtree, matrix, size, 0, 0);
        //generarea imaginii pe baza matricei
        Generate_image_from_matrix(matrix, size, output);
        //dezalocare memorie matrice a imaginii
        for (int i = 0; i < size; i++) {
            free(matrix[i]);
        }
        free(matrix);
        Free_tree(&quadtree);
        fclose(input);
        fclose(output);
    }
    return 0;
}