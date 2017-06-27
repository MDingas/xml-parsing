/* Contributor information linked list library */

typedef struct userNode* TAD_userLList;

/* Create */
TAD_userLList newUserNode(char* username,char* userid);

/* Getters */
int getUsernameSize();
int getUserIdSize();
TAD_userLList getUserNode(TAD_userLList* table,long index);
char* getUserid(TAD_userLList a);
char* getUsername(TAD_userLList a);
long getEntries(TAD_userLList a);
TAD_userLList getNextUserNode(TAD_userLList a);

/* Setters */
void incEntries(TAD_userLList a);
void setNextOf(TAD_userLList a1, TAD_userLList a2);
void setUserId(TAD_userLList a,char* userid);
void setUsername(TAD_userLList a,char* username);
void setEntries(TAD_userLList a,long entries);

/* General functions */
TAD_userLList filterMax(TAD_userLList userList,TAD_userLList maxUser,long* output);
