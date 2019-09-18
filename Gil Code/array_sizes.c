#include <stdio.h>

void printArray(int * array, int size)
{
    // sizeof array will give 8, since it is the size of a pointer
    printf("Size of array: %d\n", size);
    for (int i=0; i<size; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

int main()
{
    int numbers[] = {3, 4, 5, 6, 7};
    int more[6];
    int another[7] = {5, 6, 7};

    // In this function I can get the size of the arrays (in bytes)
    printArray(numbers, sizeof numbers / sizeof numbers[0]);
    printArray(more, sizeof more / sizeof more[0]);
    printArray(another, 7);

    return 0;
}
