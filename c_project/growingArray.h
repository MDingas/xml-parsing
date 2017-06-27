/* Growing array library */

#define GROWING_ARRAY_INITIAL_SIZE 999999


typedef struct Array* TAD_growingArray;

/* Create*/
TAD_growingArray initArray(long initialSize);

/* getters */

long getArrayValue(TAD_growingArray a,long index);
long getArraySize(TAD_growingArray a);

/* General functions */
int existArray(long* array,long val);
void insertArray(TAD_growingArray a,long index,long element);
void freeArray(TAD_growingArray a);


void incArray(TAD_growingArray a,long index, long element);

