#ifndef ARRAY_UTIL_H
#define ARRAY_UTIL_H

#include <stdio.h>

#define ARRMOVE_DONE 0
#define ARRMOVE_ERROR 1

/*
* c style arrays
*/
int arrmove(void* arr, size_t arrSize, size_t elemSize, int srcIdx, int destIdx, size_t src_nElem);


/*
* Dynamic Arrays
*/
// typedef struct {
//     size_t  maxArr;     // Maximum number of allocated arrays
//     size_t  alocMult;   // The multiplier which determines the number of malloc and realloc calls that will be used as the array grows. (maxElem * alocMult) / maxArr = the total number of times malloc and realloc will be called if maxElem is reached.
//     size_t  elemSize;   // The size of each element in bytes
//     size_t  maxElem;    // Maximum number of elements
//     size_t  currArr;    // Index of the array for the next element to be added to
//     size_t  currIdx;    // Index within currArr to use when adding the next element
//     size_t* sizes;      // Keeps track of the sizes of the arrays
//     void**  arrays;     // Keeps track of the arrays
// } heapArr;

// heapArr* newHeapArray(size_t, elemSize, size_t maxElem, size_t maxArr, size_t alocMult);

// void push_back(heapArr* arr, void* elem);

// void* get(const heapArr* arr, size_t idx);

// void freeHeapArray(heapArr* arr);

#endif /* ARRAY_UTIL_H */