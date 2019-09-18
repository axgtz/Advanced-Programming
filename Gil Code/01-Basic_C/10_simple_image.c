/*
   Definition of an image data structure
   Uses typedef, enum, struct, union

   Gilberto Echeverria
   gilecheverria@yahoo.com
   06/10/2016
*/

#include <stdio.h>

#define IMAGE_SIZE 4

//// TYPE DECLARATIONS
// An enum for the possible types of data that the image can be encoded as
// Int values in the range 0 to 255
// or Float values in the range 0.0 to 1.0
typedef enum encoding_enum {INTEGER, FLOAT} encoding_type;

// Create a union to store either of the previous two types
typedef union component_union
{
    unsigned char i;
    float f;
} component_type;

// A structure for a single pixel image, consisting of 5 color chanels
typedef struct pixel_struct
{
    // This variable indicates what part of the union is usable
    encoding_type pixel_encoding;
    component_type r;
    component_type g;
    component_type b;
} pixel_type;

//// FUNCTION PROTOTYPES
void createImage(encoding_type image_type, pixel_type image[]);
void printImage(encoding_type image_type, pixel_type image[]);

int main()
{
    pixel_type image[IMAGE_SIZE];
    encoding_type image_type = INTEGER;

    printf("An integer image:\n");
    createImage(image_type, image);
    printImage(image_type, image);

    image_type = FLOAT;
    printf("A float image:\n");
    createImage(image_type, image);
    printImage(image_type, image);


    return 0;
}

//// FUNCTION DECLARATIONS
void createImage(encoding_type image_type, pixel_type image[])
{
    for (int i=0; i<IMAGE_SIZE; i++)
    {
        // Take different actions depending on the type of the data
        if (image_type == FLOAT)
        {
            image[i].pixel_encoding = FLOAT;
            image[i].r.f = 0.0;
            image[i].g.f = 1.0;
            image[i].b.f = 0.0;
        }
        if (image_type == INTEGER)
        {
            image[i].pixel_encoding = INTEGER;
            image[i].r.i = 0;
            image[i].g.i = 255;
            image[i].b.i = 0;
        }
    }
}

void printImage(encoding_type image_type, pixel_type image[])
{
    for (int i=0; i<IMAGE_SIZE; i++)
    {
        // Take different actions depending on the type of the data
        if (image_type == FLOAT)
        {
            printf("[%.1f, %.1f, %.1f]\n", image[i].r.f, image[i].g.f, image[i].b.f);
        }
        if (image_type == INTEGER)
        {
            printf("[%3d, %3d, %3d]\n", image[i].r.i, image[i].g.i, image[i].b.i);
        }
    }
}
