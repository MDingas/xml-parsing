#include "articleLList.h"
#include "userLList.h"
#include "growingArray.h"

typedef struct TCD_istruct* TAD_istruct;

/* Create */
TAD_istruct create(int size);

/* Getters */
/* 		General list return */
TAD_growingArray getTopNArray(TAD_istruct a);

TAD_userLList getUserList(TAD_istruct a);
/* 		Get from array at index */
TAD_userLList getUserAtIndex(TAD_istruct a,long index);
TAD_articleLList getArticleAtIndex(TAD_istruct a,long index);
long getNumberNLargest(TAD_istruct a,int n);

/*		Snapshot statistics */
long getInitialSnapTableSize();
int getSnapsAdded(TAD_istruct a);
long getSize(TAD_istruct a);
long getUniqueArticles(TAD_istruct a);
long getTotalRevisions(TAD_istruct a);
long getTotalArticles(TAD_istruct a);

/* Setters */

void scaleArticleHash(TAD_istruct a,int multiplier);
void incTotalArticles(TAD_istruct a,long value);
void incTotalRevisions(TAD_istruct a,long value);
void incSnapsAdded(TAD_istruct a,long value);
void setSnapsAdded(TAD_istruct a,long value);

/* General functions */
int generateHash(TAD_istruct argTable,char* key);

long revisionBool(TAD_istruct hashtable,char* id,char* revId); /* TEMPORARY */
void insert(TAD_istruct hashtable,char* id,char* revId,char* title,char* timestamp);

void insertUser(TAD_istruct hashtable,char* userid,char* username);
void insertUserList (TAD_istruct hashtable, int i) ;
void retrievetop20(TAD_istruct table);
void updateTextSizeWordCountArray(TAD_istruct table,long index,long textSize,long textWordCount);
float indexSaturation(TAD_istruct table);
TAD_istruct cleanTable(TAD_istruct table);

