https://www.fi.muni.cz/pb071/hw/homework-03/#include <stdlib.h>
#include "memoryVM.h"
#define PTR_SIZE sizeof(int *)
#define INT_SIZE sizeof(size_t)

/*
int main()
{
    return 0;
}
*/

bool initVM(void)
{
    return true;
}

void clearVM(void)
{
  // too lazy to clear up something , let it clear up itself
}

void *allocate(size_t size)
{
    char *ptr = calloc(INT_SIZE + size, 1); //INT_SIZE for counter
    if (ptr == NULL) 	                    // not enough memory
        return NULL;
    *(size_t *)ptr = 1;              // set counter to 1
    return (void *)(ptr + INT_SIZE); // move pointer INT_SIZE bytes forward
}

void *allocateArray(size_t elementSize, size_t elementCount)
{
    return (void *)(allocate(elementSize * elementCount));
}

void acquire(void *memory)
{
    char *ptr = (char *)memory; 
    ptr -= INT_SIZE;     // move pointer INT_SIZE bytes backwards
    *(size_t *)ptr += 1; //  increment counter
}

void acquireArray2D(void **array)
{
    size_t **ptr = (size_t **)array;  // pointer to array of pointers
    acquire(ptr);   		      // acquire outer array
    for (size_t i = 0; ptr[i] != NULL; i++)
        acquire(ptr[i]); // acquire inner arrays
}

bool release(void *memory)
{
    char *ptr = (char *)memory;     
    if (ptr == NULL)
        return false;
    ptr -= INT_SIZE;  	  // move pointer INT_SIZE bytes backwards
    *(size_t *)ptr -= 1;  // decrement counter
    if (*(size_t *)ptr == 0){  
        free(ptr);  // if counter == 0 then free memory
	return true;
    }
    return false;
}

void **allocateArray2D(size_t elementSize, size_t subarrays, size_t *elementCounts)
{
    size_t **ptr = allocate((subarrays + 1) * PTR_SIZE);
    if (ptr == NULL) { // if allocation was NOT successful
        release(ptr);
        return NULL;
    }
    for (size_t i = 0; i < subarrays; i++) {
        if (elementCounts != NULL) { 
            ptr[i] = allocate(elementSize * elementCounts[i]); 
        } else { // squared array
            ptr[i] = allocate(elementSize * subarrays);
	  }
        if (ptr[i] == NULL) { // if allocation of inner array failed
            for (size_t j = 0; j <= i; j++)
                release(ptr[j]); // release all memory allocated till now
            release(ptr);
            return NULL;
        }
    }
    ptr[subarrays] = NULL; // to know when we reach end of our allocated mem
    return (void **)(ptr);
}

bool releaseArray2D(void **array)
{
    size_t **ptr = (size_t **)array;
    for (size_t i = 0; ptr[i] != NULL; i++)
        release(ptr[i]); // first release inner arrays
    return release(ptr); // then outer array
}

