/* Main snapshot statistics strucutre library
 * LI3
 * UMINHO */

#include <stdlib.h>
#include <string.h>
#include "growingArray.h"
#include "articleLList.h"
#include "userLList.h"
#include  <stdio.h>


#define HASH_SIZE 25000

/* Main data structure, 7 query information slots to be filled during load, two hash tables */
struct hashTableDef{
		int snapsAdded;
		long size; /* how many nodes */
		long uniqueArticles; /* query answer */
		long totalRevisions; /* query answer */
		long totalArticles; /* query answer */
		long top20[20]; /* query answer */
		TAD_growingArray idtop20;  /* information to help query answer */
		TAD_growingArray topN; /* query answer */
		TAD_articleLList* table; /* Array of pointers to nodes. hash by article id */
		TAD_userLList* usertable; /* array of pointers to usernodes. hash by userid */
		TAD_userLList userList;
};
typedef struct hashTableDef TCD_istruct;

typedef TCD_istruct* TAD_istruct;

/*! Create a new hashTable
 * @param size		How many indexes to be added to hash table
 * @return			Pointer to hashTable
 */
TCD_istruct* create(int size){
		/* Memory allocation */
		TCD_istruct* newTable = malloc(sizeof(TCD_istruct));
		newTable->table = malloc(sizeof(TAD_articleLList) * size);
		newTable->usertable = malloc(sizeof(TAD_userLList) * size);
		newTable->topN = initArray(GROWING_ARRAY_INITIAL_SIZE);
		newTable->idtop20 = initArray(GROWING_ARRAY_INITIAL_SIZE);
		newTable->userList = NULL;
		/* Array initialization */
		for(int i = 0; i < size; i++){
				newTable->table[i] = NULL;
				newTable->usertable[i] = NULL;
		}
		for(int i = 0; i < GROWING_ARRAY_INITIAL_SIZE; i++){
				insertArray(newTable->topN,i,0);
				insertArray(newTable->idtop20,i,0);
		}
		/* Variables initialization */
		newTable->totalArticles = 0;
		newTable->uniqueArticles = 0;
		newTable->totalRevisions = 0;
		newTable->snapsAdded = 0;
		newTable->size = size;

		return newTable;
}

/* Getters */

TAD_growingArray getTopNArray(TAD_istruct a){ return a->topN; }
long getNumberNLargest(TAD_istruct a,int n){ return a->top20[n]; }
TAD_userLList getUserList(TAD_istruct a){ return a->userList; }
TAD_userLList getUserAtIndex(TAD_istruct a,long index){ return a->usertable[index]; }
TAD_articleLList getArticleAtIndex(TAD_istruct a,long index){ return a->table[index]; }
long getInitialSnapTableSize(){ return HASH_SIZE; }
int getSnapsAdded(TAD_istruct a){ return a->snapsAdded; }
long getSize(TAD_istruct a){ return a->size; }
long getUniqueArticles(TAD_istruct a){ return a->uniqueArticles; }
long getTotalRevisions(TAD_istruct a){ return a->totalRevisions; }
long getTotalArticles(TAD_istruct a){ return a->totalArticles; }

/* Setters */

void setSnapsAdded(TAD_istruct a,long value){ a->snapsAdded = value; }
void scaleArticleHash(TAD_istruct a,int multiplier){
	a->table = realloc(a->table,(multiplier * a->size) * sizeof(TAD_articleLList));
	a->size *= multiplier;
}
void incTotalArticles(TAD_istruct a,long value){ a->totalArticles+=value; }
void incTotalRevisions(TAD_istruct a,long value){
a->totalRevisions += value; }
void incSnapsAdded(TAD_istruct a,long value){ a->snapsAdded += value; }

/*! Generate hash key
 * @param argTable	Hash table to add into
 * @param key		String to hash with (in this case, article's ID)
 * @return			Index of hash table to add into
 */
int generateHash(TCD_istruct* argTable, char* key){
		int hash = 0;
		hash = atoi(key);

		return abs(hash % getSize(argTable)); /* abs to prevent negative indexes */
}

/*! Checks if it is a new revision
 * @return			Boolean: Has this revisionID already been added? (all_articles query)
 */
long revisionBool(TCD_istruct* hashtable,char* id,char* revId){
		long flag = 1L; /* It's a long because it'll be added with a long thus preventing possible conversion errors */
		int hashedIndex = generateHash(hashtable,id);
		TAD_articleLList nodeptr = hashtable->table[hashedIndex];
		while(!isNull(nodeptr)){
				char* nodeid = getId(nodeptr);
				char* nodeRevid = getRevId(nodeptr);
				if(!strcmp(nodeid,id) && !strcmp(nodeRevid,revId)){
						flag = 0L;
				}
				nodeptr = getNextArticleNode(nodeptr);
				free(nodeid);
				free(nodeRevid);
		}
		return flag;
}

/*! Insert a node into a hash table
 */
void insert(TCD_istruct* hashtable,char* id,char* revId,char* title,char* timestamp){
		int hashedIndex = generateHash(hashtable,id);
		TAD_articleLList nodeptr = hashtable->table[hashedIndex];

		nodeptr = newNode(id,revId,title,timestamp);
		setNextArticleNode(nodeptr,hashtable->table[hashedIndex]); /* head insertion */
		hashtable->table[hashedIndex] = nodeptr;

		return;
}


/*! add an user node into the user hash table
 * @param hashtable			the hashtable to add to
 * @param userid			contributor's id
 * @param username			contributor's username
 */
void insertUser(TCD_istruct* hashtable,char* userid,char* username){
		/* Generate hashed index and point node to there */
		int hashedIndex = generateHash(hashtable,userid);
		TAD_userLList node = getUserNode(hashtable->usertable,hashedIndex);

		while(node){
				if(!strcmp(getUserid(node),userid)){ /* if it exists just increment the number and entries and leave */
						incEntries(node);
						return;
				}
				node = getNextUserNode(node);
		}


		/* If we haven't returned yet, create an entirely new node and insert */
		TAD_userLList newNode = newUserNode(username,userid);

		setNextOf(newNode,hashtable->usertable[hashedIndex]);
		hashtable->usertable[hashedIndex] = newNode; /* head insertion */

}

/* creates new node and inserts it in the list */
void insertUserList (TCD_istruct* hashtable, int index) {
		TAD_userLList node = hashtable->usertable[index];

		while(node){ /* travel entire list and add them all */

				/* create a new node to add with the info of that node */
				TAD_userLList newNode = newUserNode(getUsername(node),getUserid(node));
				setEntries(newNode,getEntries(node));

				setNextOf(newNode,hashtable->userList);

				hashtable->userList = newNode;


				node = getNextUserNode(node);
		}
}


/*! Given a fully filled array of entries(substruct of hashtable), retrieve the top 20
 * @param table		Hashtable of id entries
 */
void retrievetop20(TCD_istruct* table){
		int j;
		for(j = 0; j < 20; j++){
				long max = 0;
				long maxPos = 0;
				for(long i = getArraySize(table->idtop20)-1; i >= 0; i--){
						if(getArrayValue(table->idtop20,i) >= max){
								max = getArrayValue(table->idtop20,i);
								maxPos = i;
						}
				}
				table->top20[j] = maxPos;
				insertArray(table->idtop20,maxPos,0);
		}
}


/* Update an array of entries of a given structure
 * @param index			The article's id
 * @param textSize		The article's number of bytes on its text
 * @param textWordCount	The article's number of words on its text
 */
void updateTextSizeWordCountArray(TCD_istruct* table, long index,long textSize,long textWordCount){
		long arrValue = getArrayValue(table->idtop20,index);


		if(index > getArraySize(table->idtop20) || arrValue == 0){ /* simply add if that index's entry is empty, or if the index doesn't even exist on the current growing array's size */
				table->uniqueArticles++;
				insertArray(table->idtop20,index,textSize);
				insertArray(table->topN,index,textWordCount);
		}

		else{ /* If there are entries already, only add if bigger (filter biggest entries per id) */
				if(arrValue < textSize){
						insertArray(table->idtop20,index,textSize);
				}
				if(getArrayValue(table->topN,index) < textWordCount)
						insertArray(table->topN,index,textWordCount);
		}

}

float indexSaturation(TCD_istruct* table){
		float filled = 0;
		for(long i = 0; i < table->size; i++)
				if(table->table[i] != NULL)
						filled++;
		return (filled / table->size) ;
}

TAD_istruct cleanTable(TAD_istruct table){
		TAD_articleLList nodeAux;
		TAD_articleLList nodeptr;
		table->totalArticles = 0;
		table->uniqueArticles = 0;
		table->totalRevisions = 0;
		table->size = 0;
		table->snapsAdded = 0;
		for(int i = 0; i < HASH_SIZE;i++)
				if(table->table[i] != NULL){
						nodeptr = table->table[i];
						while(nodeptr){
								nodeAux = nodeptr;
								nodeptr = getNextArticleNode(nodeptr);
								free(nodeAux);
						}
				}


		freeArray(table->idtop20);
		freeArray(table->topN);
		free(table);

		return NULL;

}

