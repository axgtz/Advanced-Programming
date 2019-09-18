/*
The C pre-processor
Compile with the following line to see the results of the pre-processing:
    gcc -E preprocessor.c -o preprocessor.i

Examples of predefined macros
From: http://www.improgrammer.net/interesting-facts-c-language/

Gilberto Echeverria
gilecheverria@yahoo.com
14/08/2017
*/

// Add the contents of the included file here
#include <stdio.h>

// Define a constant to be used in the program
#ifndef PI
#define PI 3.14159
#endif

// Define a macro "function"
#define square(x) ((x)*(x))
 
int main()
{
    // Some predefined constants
    printf("Current File :%s\n", __FILE__ );
    printf("Current Date :%s\n", __DATE__ );
    printf("Current Time :%s\n", __TIME__ );
    printf("Line Number :%d\n", __LINE__ );

    printf("Value for the PI constant: %.10f\n", PI);

    // Find the area of a circle
    float radius = 4.5;
    // Use the constant and the macro to get a result
    float area = PI * square(radius);
    //float area = PI * square("radius");         // This should cause an error

    printf("The area of a circle of radius %.4f is: %.4f\n", radius, area);

#if DEBUG
    printf("This line will only be printed when debugging\n");
#endif

    return 0;
}
