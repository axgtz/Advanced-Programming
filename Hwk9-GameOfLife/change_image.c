/*
        Simple program to use the pgm library

        Gilberto Echeverria
        17/11/2019
    Roberto Alejandro Gutierrez Guillen

*/

#include <stdio.h>
#include <stdlib.h>
#include "pgm_image.h"

#define STRSIZE 50


void printBoard(pgm_t * image);
void masterLife(pgm_t * image), int iterations;
void checkNeighbours(pgm_t * image, pgm_t * newImage);
int countNeighbours(pgm_t * oldImage,int i, int j);
int mod(int a, int b);

int main(){
    char * in_filename = "Boards/pulsar.pgm";
    // char * in_filename = "sample_1.pgm";
    // char * out_filename = "sample_2.pgm";
    pgm_t image;

    readPGMFile(in_filename, &image);

    masterLife(&image, 10);

    /*
    // Convert the ASCII format into Binary, to generate smaller images
    // Change the P2 format to P5
    image.magic_number[1] = '5';
    negativePGM(&image);
    writePGMFile(out_filename, &image);
    */
    return 0;
}

void masterLife(pgm_t * image, int iterations){
    pgm_t newImage;
    char * iterationName = "resultImage";
    char out_filename[STRSIZE];

    // Allocate properties to image
    newImage.image.width = image->image.width;
    newImage.image.height = image->image.height;
    sprintf(newImage.magic_number,"%s",image->magic_number);
    newImage.max_value = image->max_value;

    allocateImage(&(newImage.image));

    printBoard(image);
 
    for(int i = 0;i < iterations ; i++){ // TODO MAXIT
        printf("Iteration %d\n",i);

        checkNeighbours(image, &newImage);

        sprintf(out_filename, "Result/%s_%d.pgm",iterationName,i);
        printf("%s\n",out_filename);
        writePGMFile(out_filename, &newImage);
        
        image = &newImage;// copy new to old
        printBoard(image);
    }

    freeImage(&newImage.image);
}

void checkNeighbours(pgm_t * image, pgm_t * newImage){
    int numNeighbours = 0;
    for(int i = 0;i<image->image.height;i++){
        for(int j = 0;j<image->image.width;j++){
            numNeighbours = countNeighbours(image, i, j);
            switch(image->image.pixels[i][j].value){
                case 0:     // Dead Cell
                    // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                    if(3 == numNeighbours){
                        newImage->image.pixels[i][j].value = 1;
                    }
                    break;
                case 1:     // Happy and Alive Cell
                    /*  Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
                        Any live cell with two or three live neighbours lives on to the next generation.
                        Any live cell with more than three live neighbours dies, as if by overpopulation.
                    */
                    if(numNeighbours < 2 || numNeighbours > 3){ // omae wa mou shindeiru
                         newImage->image.pixels[i][j].value = 0;
                    }
                    break;
                default:
                    printf("ERROR ZOMBIE CELL\n");
                    exit(0);
                    break;

            }
        }
    }

}

int countNeighbours(pgm_t * oldImage,int i, int j){ // TODO 
    int numNeighbours = 0;
    int height = oldImage->image.height;
    int width = oldImage->image.width;
    /*
        1 2 3   [0][0] [0][1] [0][2]            [i-1][j-1] [i-1][j] [i-1][j+1]
        4 i 6   [1][0] [1][1] [1][2]            [i][j-1] [i][j] [i][j+1]
        7 8 9   [2][0] [2][1] [2][2]            [i+1][j-1] [i+1][j] [i+1][j+1]
    */
    if(oldImage->image.pixels[mod(i-1,height)][mod(j-1,width)].value == 1){numNeighbours++;}
    if(oldImage->image.pixels[mod(i,height)][mod(j-1,width)].value == 1){  numNeighbours++;}
    if(oldImage->image.pixels[mod(i+1,height)][mod(j-1,width)].value == 1){numNeighbours++;}
    if(oldImage->image.pixels[mod(i-1,height)][mod(j,width)].value == 1){  numNeighbours++;}
    if(oldImage->image.pixels[mod(i+1,height)][mod(j,width)].value == 1){  numNeighbours++;}
    if(oldImage->image.pixels[mod(i-1,height)][mod(j+1,width)].value == 1){numNeighbours++;}
    if(oldImage->image.pixels[mod(i,height)][mod(j+1,width)].value == 1){  numNeighbours++;}
    if(oldImage->image.pixels[mod(i+1,height)][mod(j+1,width)].value == 1){numNeighbours++;}

    return numNeighbours;
}

void printBoard(pgm_t * image){
    for(int i = 0;i < image->image.height;i++){
        for(int j = 0;j < image->image.width;j++){
            printf("%u", image->image.pixels[i][j].value);
        }
        printf("\n");
    }
}

int mod(int a, int b){
    int r = a % b;
    return r < 0 ? r + b : r;
}

/*
typedef struct pixel_struct
{
    unsigned char value;
} pixel_t;

// Structure to store full image data of any size
typedef struct image_struct
{
    int width;
    int height;
    pixel_t ** pixels;
} image_t;

// Structure for an image in PGM format
typedef struct pgm_struct
{
    char magic_number[3];           // String for the code indicating the type of PGM file
    int max_value;                  // Maximum value for pixel data in each component
    image_t image;
} pgm_t;
*/
