/* =============================================================================
 *
 * vector.c
 *
 * =============================================================================
 */

#include <stdlib.h>
#include <string.h>

#include "vector.h"


/* =============================================================================
 * vector_alloc
 * -- Returns NULL if failed
 * =============================================================================
 */
vector_t*
vector_alloc (long initCapacity, void (*eraseElement)(void*))
{
    vector_t* vectorPtr;
    long capacity = ((initCapacity > 1) ? (initCapacity) : 1);

    vectorPtr = (vector_t*)malloc(sizeof(vector_t));

    if (vectorPtr != NULL) {
        vectorPtr->size = 0;
        vectorPtr->capacity = capacity;
        vectorPtr->elements = (void**)malloc(capacity * sizeof(void*));
        if (vectorPtr->elements == NULL) {
            return NULL;
        }
        vectorPtr->eraseElement = eraseElement;
    }

    return vectorPtr;
}


/* =============================================================================
 * vector_free
 * =============================================================================
 */
void
vector_free (vector_t* vectorPtr)
{
	long i;
	
	if (vectorPtr->eraseElement != NULL) {
		for (i = 0; i < vectorPtr->size; i++) {
			vectorPtr->eraseElement(vectorPtr->elements[i]);
			vectorPtr->elements[i] = NULL;
		}
	}

    free(vectorPtr->elements);
    free(vectorPtr);
}


/* =============================================================================
 * vector_at
 * -- Returns NULL if failed
 * =============================================================================
 */
void*
vector_at (vector_t* vectorPtr, long i)
{
    if ((i < 0) || (i >= vectorPtr->size)) {
        return NULL;
    }

    return (vectorPtr->elements[i]);
}


/* =============================================================================
 * vector_pushBack
 * -- Returns FALSE if fail, else TRUE
 * =============================================================================
 */
bool
vector_pushBack (vector_t* vectorPtr, void* dataPtr)
{
    if (vectorPtr->size == vectorPtr->capacity) {
        long i;
        long newCapacity = vectorPtr->capacity * 2;
        void** newElements = (void**)malloc(newCapacity * sizeof(void*));
        if (newElements == NULL) {
            return false;
        }
        vectorPtr->capacity = newCapacity;
        for (i = 0; i < vectorPtr->size; i++) {
            newElements[i] = vectorPtr->elements[i];
        }
        free(vectorPtr->elements);
        vectorPtr->elements = newElements;
    }

    vectorPtr->elements[vectorPtr->size++] = dataPtr;

    return true;
}


/* =============================================================================
 * vector_popBack
 * Returns NULL if fail, else returns last element
 * =============================================================================
 */
void*
vector_popBack (vector_t* vectorPtr)
{
    if (vectorPtr->size < 1) {
        return NULL;
    }

    return (vectorPtr->elements[--(vectorPtr->size)]);
}


/* =============================================================================
 * vector_getSize
 * =============================================================================
 */
long
vector_getSize (vector_t* vectorPtr)
{
    return (vectorPtr->size);
}


/* =============================================================================
 * vector_clear
 * =============================================================================
 */
void
vector_clear (vector_t* vectorPtr)
{
    vectorPtr->size = 0;
}


/* =============================================================================
 * vector_sort
 * =============================================================================
 */
void
vector_sort (vector_t* vectorPtr, int (*compare) (const void*, const void*))
{
    qsort((void*)vectorPtr->elements, vectorPtr->size, sizeof(void**), compare);
}


/* =============================================================================
 * vector_copy
 * =============================================================================
 */
bool
vector_copy (vector_t* dstVectorPtr, vector_t* srcVectorPtr)
{
    long dstCapacity = dstVectorPtr->capacity;
    long srcSize = srcVectorPtr->size;
    if (dstCapacity < srcSize) {
        long srcCapacity = srcVectorPtr->capacity;
        void** elements = (void**)malloc(srcCapacity * sizeof(void*));
        if (elements == NULL) {
            return false;
        }
        free(dstVectorPtr->elements);
        dstVectorPtr->elements = elements;
        dstVectorPtr->capacity = srcCapacity;
    }

    memcpy(dstVectorPtr->elements,
           srcVectorPtr->elements,
           (srcSize * sizeof(void*)));
    dstVectorPtr->size = srcSize;

    return true;
}