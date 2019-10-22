// Roberto Alejandro Gutierrez Guillen
#include <iostream>

using namespace std;

int binarySearch(int arr[], int left, int right, int num) {
    if (arr == 0){return -1;} // if array pointer null
    if (right >= left) { 
        int mid = left + (right - left) / 2; 

        if (arr[mid] == num) // Found number
            return mid; 
  
        if (arr[mid] > num)  // Number smaller than mid, then we go to left subarray
            return binarySearch(arr, left, mid - 1, num); 
  
        return binarySearch(arr, mid + 1, right, num); // Number bigger than mid, then we go to right subarray
    } 
    return -1; // Number not found
} 
  
int main() { 
    int arr[] = { 1, 2, 3, 4, 5, 10, 40 }; 
    int n = sizeof(arr) / sizeof(arr[0]); 

    int num = 10; // number to look for
    int result = binarySearch(arr, 0, n - 1, num); 
    (result == -1) ? printf("Element is not present in array\n") 
                   : printf("Element is present at index %d\n", 
                            result); 
    return 0; 
} 
