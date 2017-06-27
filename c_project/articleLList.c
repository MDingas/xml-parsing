#include <string.h> /* strcpy() */
#include <stdlib.h> /* malloc() */

#define ID_SIZE 15
#define REVID_SIZE 15
#define TITLE_SIZE 260
#define TIMESTMP_SIZE 30

/* Node for article entries hashtable */
struct nodeDef{

		char id[ID_SIZE];
		char revisionid[REVID_SIZE];
		char title[TITLE_SIZE];
		char timestamp[TIMESTMP_SIZE];
		struct nodeDef* next; /* pointer to next node*/
};
typedef struct nodeDef node;

typedef node* TAD_articleLList;

/* Create */

/*! Create new node to be inserted into hashTable
 * @param textSize	    Number of bytes on the text article
 * @param textWords	    Number of words on the text article
 * @return			    Node to insert to linked list
 */
TAD_articleLList newNode(char*id,char* revId,char* title,char* timestamp){
		TAD_articleLList newNode;

		newNode = malloc(sizeof(node));
		strcpy(newNode->id,id);
		strcpy(newNode->revisionid,revId);
		strcpy(newNode->title,title);
		strcpy(newNode->timestamp,timestamp);

		newNode->next = NULL;

		return newNode;
}

/* Getters */

int getIdSize(){ return ID_SIZE; }
int getRevIdSize(){ return REVID_SIZE; }
int getTitleSize(){ return TITLE_SIZE; }
int getTimeStmpSize(){ return TIMESTMP_SIZE; }
char* getId(TAD_articleLList a){
	char* output = malloc(ID_SIZE * sizeof(char));
	strcpy(output,a->id);
	return output;
	}
char* getRevId(TAD_articleLList a){
	char* output = malloc(REVID_SIZE * sizeof(char)) ;
	strcpy(output,a->revisionid);
	return output;
	}
char* getTitle(TAD_articleLList a){
	char* output = malloc(TITLE_SIZE * sizeof(char));
	strcpy(output,a->title);
	return output;
	}
char* getTimeTamp(TAD_articleLList a){
	char* output = malloc(TIMESTMP_SIZE * sizeof(char));
	strcpy(output,a->timestamp);
	return output;
	}
TAD_articleLList getNextArticleNode(TAD_articleLList a){ return a->next; }


/* Setters */

void setNextArticleNode(TAD_articleLList a1,TAD_articleLList a2){ a1->next = a2; }

/* General Functions */

int isNull(TAD_articleLList a){ return(a==NULL); }
