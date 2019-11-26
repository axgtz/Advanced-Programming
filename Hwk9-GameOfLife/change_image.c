/*
        Simple program to use the pgm library

        Gilberto Echeverria
        17/11/2019
    Roberto Alejandro Gutierrez Guillen
*/
#include <stdio.h>
#include <stdlib.h>
#include "pgm_image.h"

// Library for OpenMP
#include <omp.h>

#include <pthread.h>

#define STRSIZE 50

void printBoard(pgm_t * image);
void masterLife(pgm_t * image, int iterations, int mode);
void runSimulation(pgm_t * image, pgm_t * newImage);
void runSimulationOMP(pgm_t * image, pgm_t * newImage);
void runSimulationThreads(pgm_t * image, pgm_t * newImage, int threadNum); // TODO
int countNeighbours(pgm_t * oldImage,int i, int j);
int mod(int a, int b);

int main(){
    //char * in_filename = "Boards/pulsar.pgm";
    char * in_filename = "Boards/bichitos.pgm";
    //char * in_filename = "sample_1.pgm";
    // char * out_filename = "sample_2.pgm";
    pgm_t image;

    readPGMFile(in_filename, &image);
    // mode 
    masterLife(&image, 500, 0);
    return 0;
}

void masterLife(pgm_t * image, int iterations, int mode){
    pgm_t newImage;
    char * iterationName = "resultImage";
    char out_filename[STRSIZE];

    // Allocate properties to image
    newImage.image.width = image->image.width;
    newImage.image.height = image->image.height;
    sprintf(newImage.magic_number,"%s",image->magic_number);
    newImage.max_value = image->max_value;

    // Allocate Array
    allocateImage(&(newImage.image));

    printf("\n\t----Starting----\n\t");

    switch(mode){
        case 0: // Sequential
            for(int i = 0;i < iterations ; i++){         
                runSimulation(image, &newImage);

                // Prepare name for file
                sprintf(out_filename, "Result/%s_%d.pgm",iterationName,i);
                
                
                // Deep copy from old to new
                copyPGM(&newImage, image); // Source, destination
                writePGMFile(out_filename, &newImage);
            }
            
            break;
        case 1: // OMP
            for(int i = 0;i < iterations ; i++){     
                runSimulationOMP(image, &newImage); // frame

                // Prepare name for file
                sprintf(out_filename, "Result/%s_%d.pgm",iterationName,i);
                
                    
                // Deep copy from old to new
                copyPGM(&newImage, image); 
                writePGMFile(out_filename, &newImage);
            }
            
            break;
        case 2: // Threads
            for(int i = 0;i < iterations ; i++){         
                runSimulationThreads(image, &newImage, 4);

                // Prepare name for file
                sprintf(out_filename, "Result/%s_%d.pgm",iterationName,i);
                writePGMFile(out_filename, &newImage);
                
                // Deep copy from old to new
                copyPGM(&newImage, image);
            }
            break;
        default:
            printf("ERROR WRONG OPTION - TERMINATING\n");
            exit(0);
            break;
    }
    freeImage(&newImage.image);
}

void runSimulation(pgm_t * image, pgm_t * newImage){
    int numNeighbours = 0;
    
    for(int i = 0;i < image->image.height;i++){
        for(int j = 0;j<image->image.width;j++){
            numNeighbours = countNeighbours(image, i, j);
            switch(image->image.pixels[i][j].value){
                case 0:     // Dead Cell
                    // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                    if(numNeighbours == 3){
                        newImage->image.pixels[i][j].value = 1;
                    } else { // Still dead #sorryNotSorry
                        newImage->image.pixels[i][j].value = 0;
                    }
                    break;
                case 1:     // Happy and Alive Cell
                    /*  Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
                        Any live cell with two or three live neighbours lives on to the next generation.
                        Any live cell with more than three live neighbours dies, as if by overpopulation.
                    */
                    if(numNeighbours < 2 || numNeighbours > 3){ // omae wa mou shindeiru
                        newImage->image.pixels[i][j].value = 0;
                    } else { // You get to live once more little one, 2 or 3 neighbours
                        newImage->image.pixels[i][j].value = 1;
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

void runSimulationOMP(pgm_t * image, pgm_t * newImage){
    // Divide the rows for each thread 
    // Default, no variables are to be asumed private or shared by the computer, need to be explicitly stated by user
    #pragma omp parallel for default(none) shared(image, newImage)
    for(int i = 0;i < image->image.height;i++){
        for(int j = 0;j<image->image.width;j++){
            int numNeighbours = countNeighbours(image, i, j);
            switch(image->image.pixels[i][j].value){
                case 0:     // Dead Cell
                    // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                    if(numNeighbours == 3){
                        newImage->image.pixels[i][j].value = 1;
                    } else { // Still dead #sorryNotSorry
                        newImage->image.pixels[i][j].value = 0;
                    }
                    break;
                case 1:     // Happy and Alive Cell
                    /*  Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
                        Any live cell with two or three live neighbours lives on to the next generation.
                        Any live cell with more than three live neighbours dies, as if by overpopulation.
                    */
                    if(numNeighbours < 2 || numNeighbours > 3){ // omae wa mou shindeiru
                        newImage->image.pixels[i][j].value = 0;
                    } else { // You get to live once more little one, 2 or 3 neighbours
                        newImage->image.pixels[i][j].value = 1;
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

void runSimulationThreads(pgm_t * image, pgm_t * newImage, int threadNum){ // TODO
    // Thread Stuff
    // pthread_t * tid = NULL;

    int numNeighbours = 0;
    for(int i = 0;i < image->image.height;i++){
        for(int j = 0;j<image->image.width;j++){
            numNeighbours = countNeighbours(image, i, j);
            switch(image->image.pixels[i][j].value){
                case 0:     // Dead Cell
                    // Any dead cell with exactly three live neighbours becomes a live cell, as if by reproduction.
                    if(numNeighbours == 3){
                        newImage->image.pixels[i][j].value = 1;
                    } else { // Still dead #sorryNotSorry
                        newImage->image.pixels[i][j].value = 0;
                    }
                    break;
                case 1:     // Happy and Alive Cell
                    /*  Any live cell with fewer than two live neighbours dies, as if caused by underpopulation.
                        Any live cell with two or three live neighbours lives on to the next generation.
                        Any live cell with more than three live neighbours dies, as if by overpopulation.
                    */
                    if(numNeighbours < 2 || numNeighbours > 3){ // omae wa mou shindeiru
                        newImage->image.pixels[i][j].value = 0;
                    } else { // You get to live once more little one, 2 or 3 neighbours
                        newImage->image.pixels[i][j].value = 1;
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
    // The mod helps to handle edge cases
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
