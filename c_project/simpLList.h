/* Single node linked list library */

typedef struct nameNode* TAD_linkedList;

/* Create */
TAD_linkedList newNameNode(char* name);

/* Getters */
char* getNodeName(TAD_linkedList a);
TAD_linkedList getNextNode(TAD_linkedList a);

/* General functions */
TAD_linkedList insertToNameNode(TAD_linkedList node,TAD_linkedList list);
TAD_linkedList insertPrefixMatches(char* prefix,TAD_istruct hashtable);
void insertNodeAlphabetically(TAD_linkedList appendedWord,TAD_linkedList node);
void swapWord(TAD_linkedList n1,TAD_linkedList n2);
void freeAll(TAD_linkedList node);


