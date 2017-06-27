/* Article information linked list library */

typedef struct node* TAD_articleLList;

/* Create */
TAD_articleLList newNode(char* id,char* revId,char* title,char* timestamp);

/* Getters */
int getIdSize();
int getRevIdSize();
int getTitleSize();
int getTimeStmpSize();
char* getId(TAD_articleLList a);
char* getRevId(TAD_articleLList a);
char* getTitle(TAD_articleLList a);
char* getTimeTamp(TAD_articleLList a);
TAD_articleLList getNextArticleNode(TAD_articleLList a);

/* Setters */
void setNextArticleNode(TAD_articleLList a1,TAD_articleLList a2);

/* General functions */
int isNull(TAD_articleLList a);
