/*	Growing array library
 *	LI3
 *	UMINHO */

#include <stdlib.h> /* free(), realloc(), malloc() */


typedef struct {

		long *array;
		long size;
} Array;

typedef Array* TAD_growingArray;
/* Create */

/*! Initialize a growing array
* @param initialSize		How many indexes to be allocated
* @return					Pointer to growing array
*/
TAD_growingArray initArray(long initialSize) {
		TAD_growingArray a = malloc(sizeof(Array));
		a->array = (long *)malloc(initialSize * sizeof(long));
		a->size = initialSize;

		return a;
}

/* Getters */


long getArrayValue(TAD_growingArray a,long index){ return a->array[index]; }
long getArraySize(TAD_growingArray a){ return a->size; }


/* General Functions */

/*! Auxiliary function: does a value exist in a 10 slotted arrray
*/
int existArray(long* array, long val){
		for(int i = 0; i < 10; i++)
				if (array[i] == val)
						return 1;
		return 0;
}


/*! Insert an element into a growing array
* @param a				Growing array to insert into
* @param index			Index to insert the element into
* @param element		Long number to insert into growing array
*/
void insertArray(TAD_growingArray a, long index, long element) {
		long oldSize;
		if(index >= a->size){
				oldSize = a->size;
				while(index >= a->size){
						a->size *= 2L;
						a->array = (long *)realloc(a->array, a->size * sizeof(long));
				}
		/* Cleanup all new allocated space */
		for(int j = oldSize; j < a->size - 1; j++)
			insertArray(a,j,0);
		}
		a->array[index] = element;
}

void incArray(TAD_growingArray a,long index, long element){
		long oldSize;
		if(index >= a->size){
				oldSize = a->size;
				while(index >= a->size){
						a->size *= 2L;
						a->array = (long *)realloc(a->array, a->size * sizeof(long));
				}
		/* Cleanup all new allocated space */
		for(int j = oldSize; j < a->size - 1; j++)
			insertArray(a,j,0);
		}
		a->array[index] += element;

}

/*! Free all the memory allocated by growing array
* @param a				Growing array to free
*/
void freeArray(TAD_growingArray a) {

		free(a->array);
		a->array = NULL;
		a->size = 0;
}


