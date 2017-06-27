#include "xmlFunctions.h" /* stdlib() and stdio() includer */
#include "simpLList.h" /* node* definision, nameNode* definition */
#include <string.h> /* strcpy() */
#include <stdlib.h> /* malloc()  */
#include <stdio.h> /* snprintf() */
#include <omp.h>



TAD_istruct init(){
		return (create(getInitialSnapTableSize()));
}

TAD_istruct load(TAD_istruct table,int nsnaps, char* snaps_paths[]){
		char* fileArg;
		for(int i = 0; i < nsnaps;i++){
				fileArg = snaps_paths[i];
				table = parseDocument(table,fileArg);

				/* Does the hashtable need to increase? */
				if(getSnapsAdded(table) > 5){ /* Check every 5 new snapshots */
						if(indexSaturation(table) > 0.8){ /* only if >80% occupation */
								scaleArticleHash(table,2); /* double its size */
								setSnapsAdded(table,0); /* reset checking counter */
						}
				}
			}
		return table;
}

/* Calculated on load */
long all_articles(TAD_istruct table){ return getTotalArticles(table); }

/* Calculated on load */
long unique_articles(TAD_istruct table){ return getUniqueArticles(table); }

/* Calculated on load */
long all_revisions(TAD_istruct table){ return getTotalRevisions(table); }

long* top_10_contributors(TAD_istruct table){
		long* output = (long*) malloc(10* sizeof(long));

		for(int i = 0; i < 10; i++) /* initializes output with zeros */
				output[i] = 0L;

		for (int i=0; i < getSize(table); i++) { /* create userList with user info */
				if(getUserAtIndex(table,i) != NULL)
						insertUserList(table,i);
		}

		TAD_userLList maxUser = newUserNode("null","0"); /* maxUser to compare to on the 10 iterations */

		int index = 0;

		/* 10 times: look for max user and add to output array */
		while(index < 10){
				setUserId(maxUser,"0");
				setUsername(maxUser,"null");
				setEntries(maxUser,0);

				maxUser = filterMax(getUserList(table),maxUser,output);

				output[index++] = atol(getUserid(maxUser));
		}


		return output;
}

/* Hash table lookup */
char* contributor_name(long contributor_id,TAD_istruct hashtable){
		char* output = malloc(getUsernameSize() * (sizeof(char*)));

		char contributorIdString[getUserIdSize()];
		snprintf(contributorIdString,getUserIdSize(),"%ld",contributor_id);

		int hashedIndex = generateHash(hashtable,contributorIdString);

		TAD_userLList nodeptr = getUserAtIndex(hashtable,hashedIndex);

		while(nodeptr && strcmp(getUserid(nodeptr),contributorIdString)){
				nodeptr = getNextUserNode(nodeptr);
		}

		if(nodeptr){
			strcpy(output,getUsername(nodeptr));
				return output;
		}

		return NULL;
}

/* Array of entries filled during load, top 20 added during load
*/
long* top_20_largest_articles(TAD_istruct hashtable){
		retrievetop20(hashtable);
		long* output = (long*) malloc(20 * sizeof(long)); // array to be returned

		for(int i = 0; i < 20; i++)
				output[i] = getNumberNLargest(hashtable,i);

		return output;
}

/* Hash table lookup */
char* article_title(long article_id,TAD_istruct table){
		char* output = malloc(getTitleSize() * (sizeof(char*)));

		char articleIdString[getIdSize()];
		snprintf(articleIdString,getIdSize(),"%ld",article_id);

		int hashedIndex = generateHash(table,articleIdString);

		TAD_articleLList nodeptr = getArticleAtIndex(table,hashedIndex);

		while(nodeptr && strcmp(getId(nodeptr),articleIdString)){
				nodeptr = getNextArticleNode(nodeptr);
		}

		if(nodeptr){
			strcpy(output,getTitle(nodeptr));
			return output;
		}

		return NULL;
}

/* Array of entries filled during load, this function allocates
 * memory and retrieves top N entries */
long* top_N_articles_with_more_words (int n,TAD_istruct table){
		long* result = (long*) malloc (n * sizeof(long));
		TAD_growingArray topN = getTopNArray(table);

		for(int k = 0; k < n; k++){
				long max = -1;
				long maxPos = 0;
				for(long i = getArraySize(topN) - 1; i >= 0; i--){
						if(getArrayValue(topN,i) >= max){
								max = getArrayValue(topN,i);
								maxPos = i;
						}
				}
				result[k] = maxPos;
				insertArray(topN,maxPos,0);
		}
		return result;
}

/* Linear navegation through hash table */
char** titles_with_prefix(char* prefix, TAD_istruct table){
		TAD_linkedList namenode = insertPrefixMatches(prefix,table);

		char** returnList = (char**) malloc(getTotalArticles(table) * sizeof(char*));
		int i = 0;
		while(strcmp(getNodeName(namenode),"null") != 0){
				returnList[i] = (char*) malloc(getTitleSize() + 1);
				strcpy(returnList[i],getNodeName(namenode));
				namenode = getNextNode(namenode);
				i++;
		}
		returnList[i] = (char*) malloc(getTitleSize() + 1);
		returnList[i] = NULL;


		freeAll(namenode);
		return returnList;
}

/* Hash table lookup */
char* article_timestamp(long article_id,long revision_id,TAD_istruct table){
		char* output = malloc(getTimeStmpSize() * (sizeof(char*)));

		char articleIdString[getIdSize()];
		snprintf(articleIdString,getIdSize(),"%ld",article_id);

		char revisionIdString[getRevIdSize()];
		snprintf(revisionIdString,getRevIdSize(),"%ld",revision_id);

		int hashedIndex = generateHash(table,articleIdString);

		TAD_articleLList nodeptr = getArticleAtIndex(table,hashedIndex);

		while(nodeptr){
				if(!strcmp(getId(nodeptr),articleIdString) && !strcmp(getRevId(nodeptr),revisionIdString)){
						strcpy(output,getTimeTamp(nodeptr));
						return output;
				}
				nodeptr = getNextArticleNode(nodeptr);
		}

		return NULL;
}

TAD_istruct clean(TAD_istruct table){ table = cleanTable(table); return NULL; }
