/*
    Simple program to use the pgm library

    Gilberto Echeverria
    17/11/2019
*/

#include <stdio.h>
#include <stdlib.h>
#include "pgm_image.h"


void printBoard(pgm_t * image);

int main(){
    char * in_filename = "Boards/pulsar.pgm";
    char * out_filename = "sample_2.pgm";
    pgm_t image;

    readPGMFile(in_filename, &image);

    printBoard(&image);
    

    // Convert the ASCII format into Binary, to generate smaller images
    // Change the P2 format to P5
    image.magic_number[1] = '2';
    //negativePGM(&image);
    writePGMFile(out_filename, &image);

    return 0;
}

void printBoard(pgm_t * image){
    for(int i = 0;i<image->image.height;i++){
        for(int j = 0;j<image->image.width;j++){
            printf("%u", image->image.pixels[i][j].value);
        }
        printf("\n");
    }
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
