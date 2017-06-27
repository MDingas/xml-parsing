/*	Linked list library
 *	LI3
 *	UMINHO */

#include <string.h> /* strcpy() */
#include <stdlib.h> /* free(), malloc() */
#include "snapshotStats.h" /* TAD_istruct definition */
#include "stringFunctions.h" /* prefixMatch() */


#define TITLE_SIZE 160

/* Simple node for article's title entry */
struct nameNodeDef{
    char name[TITLE_SIZE];
    struct nameNodeDef* next;
};
typedef struct nameNodeDef nameNode;

typedef nameNode* TAD_linkedList;

/* Create */

/*!	Create simple node with a single String and pointer to next
 * @param name	    Article's title
 * @return 		    New node
 */
TAD_linkedList newNameNode(char*name){
		nameNode* node = malloc(sizeof(nameNode));
		strcpy(node->name,name);
		node->next = NULL;

		return node;
}

/* Getters */

char* getNodeName(TAD_linkedList a){ return(a->name); }
TAD_linkedList getNextNode(TAD_linkedList a){ return a->next; }

/* General functions */

/*! Insert node to head of linked list
 * @param node		Node to be inserted
 * @param list	 	List to insert into
 * @return			New linked list
 */
void insertToNameNode(TAD_linkedList node,TAD_linkedList list){
		node->next = list;
		return;
}

/*! Swap the words between two nodes */
void swapWord(TAD_linkedList n1,TAD_linkedList n2){
		nameNode* temp;
		temp = newNameNode("");

		strcpy(temp->name,n1->name);

		strcpy(n1->name,n2->name);

		strcpy(n2->name,temp->name);

		free(temp);
}

/*! Insert a node into a linked list, in alphabetical order of the Title's article
 * @param appendedWord		Node to append
 * @param node				List to append to
 */
void insertNodeAlphabetically(TAD_linkedList appendedWord,TAD_linkedList node){
		if(!node){ /* empty list */
				node = appendedWord;
				return;
		}
		if(!node->next){ /* single node */
				if(strcmp(node->name,appendedWord->name) > 0){
						swapWord(node,appendedWord);
						node->next = appendedWord; /* end insertion */
				}
				else if(strcmp(node->name,appendedWord->name) == 0)
						return;
				else{
						node->next = appendedWord; /* head insertion */
				}
		}
		else{ /* need to travel a multiple node linked list */
				nameNode* prev = NULL;
				if(strcmp(appendedWord->name,node->name) > 0){ /* we need to travel it */
						while(node->next && strcmp(appendedWord->name,node->name) > 0){
								prev = node;
								node = node->next;
						}
						if(strcmp(appendedWord->name,node->name) == 0){ /* it already exists, do nothing */
								return;
						}
						if(node->next || strcmp(appendedWord->name,node->name) < 0){ /* we need to insert in the middle */
								prev->next = appendedWord;
								appendedWord->next = node;
						}
						else /* we need to insert in the end */
								node->next = appendedWord;
				}
				else
						if(strcmp(appendedWord->name,node->name) == 0) /* it already exists, do nothing */
								return;
						else{ /* we need to insert at head */
								swapWord(appendedWord,node);
								appendedWord->next = node->next;
								node->next = appendedWord;
						}
		}
}

/*!	Navegate hashTable and add to linked list all title articles that match query's prefix
 * @param prefix	 	Query's prefix argument
 * @param hashtable 	Hashtable with snapshot's articles
 * @return			Linked list
 */
TAD_linkedList insertPrefixMatches(char* prefix,TAD_istruct table){
		TAD_articleLList node;
		nameNode* namenode = newNameNode("null"); /* inicializer to linked list */
		for(int i = 0; i < getSize(table);i++)
				if(getArticleAtIndex(table,i)){
						node = getArticleAtIndex(table,i);
						while(node){
								if(prefixMatch(prefix,getTitle(node)))
										insertNodeAlphabetically(newNameNode(getTitle(node)),namenode);
								node = getNextArticleNode(node);
						}
				}
		return namenode;
}

/*! Free all the nodes of a linked list
 * @param node 		linked list to free
 */
void freeAll(TAD_linkedList node){
		nameNode* nodeptr;
		while(node){
				nodeptr = node;
				node = node->next;
				free(nodeptr);
		}
}
