#include <stdlib.h> /* malloc() */
#include <string.h> /* strcpy() */
#include "growingArray.h" /* existsArray() */

#define USERNAME_SIZE 90
#define USERID_SIZE 15

/* User information node */
struct userNodeDef{
		char username[USERNAME_SIZE];
		char userid[USERID_SIZE];
		long entries; /* how many unique contributions that user has */
		struct userNodeDef* next;
};
typedef struct userNodeDef userNode;

typedef userNode* TAD_userLList;

/* Create */

TAD_userLList newUserNode(char* username, char* userid){
	TAD_userLList node = malloc(sizeof(userNode));
	strcpy(node->username,username);
	strcpy(node->userid,userid);
	node->entries = 1;

	return node;
}

/* Getters */

int getUsernameSize(){ return USERNAME_SIZE; }
int getUserIdSize(){ return USERID_SIZE; }
TAD_userLList getUserNode(TAD_userLList* table, long index){ return (table[index]); }
char* getUserid(TAD_userLList a){ return(a->userid); }
char* getUsername(TAD_userLList a){ return(a->username); }
long getEntries(TAD_userLList a){ return(a->entries); }
TAD_userLList getNextUserNode(TAD_userLList a){ return a->next; }

/* Setters */

void incEntries(TAD_userLList a){ a->entries++; }
void setNextOf(TAD_userLList a1, TAD_userLList a2){ a1->next = a2; }
void setUserId(TAD_userLList a,char* userid){ strcpy(a->userid,userid); }
void setUsername(TAD_userLList a,char* username){ strcpy(a->username,username); }
void setEntries(TAD_userLList a,long e){ a->entries = e; }

/* General Functions */

/*! Given a list of users, find the top entry one that hasn't been added to the top10 yet (and do userid untie asked by the query )
 * @param userListptr	userList of usernames and nr of revisions
 * @param maxUser		dummy node to fill with biggest contributor found
 * @param output			current top10 array (prevent repeated entries
 */
TAD_userLList filterMax(TAD_userLList userListptr,TAD_userLList maxUser,long* output){

		while(userListptr){
				/* look for bigger candidate, add only if his userid isnt on top10 already */
				if(getEntries(userListptr) > getEntries(maxUser) && !existArray(output,atol(getUserid(userListptr)))){
						setEntries(maxUser,getEntries(userListptr));
						setUserId(maxUser,getUserid(userListptr));
				}
				/* if equal candidate, add only if his userid is smaller */
				else if(getEntries(userListptr) == getEntries(maxUser) && !existArray(output,atol(getUserid(userListptr)))){
						if(atol(getUserid(userListptr)) < atol(getUserid(maxUser))){
								setUserId(maxUser,getUserid(userListptr));
						}
				}

				userListptr = getNextUserNode(userListptr);
		}

		return maxUser;
}

