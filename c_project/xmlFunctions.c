#include <string.h> /* strcpy() */
#include "xmlFunctions.h" /* libxml2 library */
#include "stringFunctions.h" /* textCounter() */
#include "snapshotStats.h" /* getTitleSize() getIdSize() getRevIdSize() getTimeStmpSize() */

/*! Retrieve username tag from the contributor tag
 * @param doc		xml document
 * @param cur		current node pointer in the libxml structure
 *
 * @return 			the contributor's username
 */
char* xmlgetUsername(xmlDocPtr doc,xmlNodePtr cur){
		xmlChar* key;
		char* stringOutput = malloc(sizeof(char) * getUsernameSize());
		cur = cur->xmlChildrenNode;
		while(cur != NULL){
				if((xmlStrEqual(cur->name,(const xmlChar*)"username"))){
						key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
						strcpy(stringOutput,(char*)key);
						xmlFree(key);
						return stringOutput;
				}
				cur = cur->next;
		}
		return "ERROR no username found";
}

/*! Retrieve id tag from the contributor tag
 * @param doc		xml document
 * @param cur		current node pointer in the libxml structure
 *
 * @return			the contributor's id
 */
char* xmlgetUserId(xmlDocPtr doc,xmlNodePtr cur){
		xmlChar* key;
		char* stringOutput = malloc(sizeof(char) * getUserIdSize());
		cur = cur->xmlChildrenNode;
		while(cur!=NULL){
				if((xmlStrEqual(cur->name,(const xmlChar*)"id"))){
						key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
						strcpy(stringOutput,(char*)key);
						xmlFree(key);
						return stringOutput;
				}
				cur = cur->next;
		}
		return "ERROR no userID found";
}


/*! Retrieve all necessary information from a xml document
 * @param doc		xml document
 * @param cur		current node pointer in the libxml structure
 * @param info		output array that the callee function will use
 */
void parseIds (xmlDocPtr doc,xmlNodePtr cur,char** info) {
		cur = cur->xmlChildrenNode;                                  /*enters page node*/
		xmlChar *key;
		char* idOutput = malloc(sizeof(char)*getIdSize());
		char* revIdOutput = malloc(sizeof(char)*getRevIdSize());
		int i = 0;

		while (i<1 && cur!=NULL) {
				if ((xmlStrEqual(cur->name, (const xmlChar *)"id"))) {
						key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
						strcpy(idOutput,(char*) key);
						xmlFree(key);
						info[1] = idOutput;
						i++;
				}
				cur = cur->next;
		}
		while(!xmlStrEqual(cur->name,(const xmlChar*)"revision")) {
				cur = cur->next;
		}

		cur = cur->xmlChildrenNode; /*enters revision node*/
		while (i < 2 && cur!=NULL) {
				if((xmlStrEqual(cur->name,(const xmlChar*)"id"))){
						key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
						strcpy(revIdOutput,(char*) key);
						xmlFree(key);
						info[2] = revIdOutput;
						i++;
				}
				cur = cur->next;
		}
		return;
}




/*! Retrieve all necessary information from a xml document
 * @param doc		xml document
 * @param cur		current node pointer in the libxml structure
 * @param info		output array that the callee function will use
 * @param textinfo	long number info about the article's text
 */
void parseInformation(xmlDocPtr doc,xmlNodePtr cur,char** info,long* textInfo) {
		cur = cur->xmlChildrenNode;                                  /*enters page node*/
		xmlChar *key;
		char* titleOutput = malloc(sizeof(char)*getTitleSize());
		char* timestampOutput = malloc(sizeof(char)*getTimeStmpSize());
		int i = 0;

		while (i<1 && cur!=NULL) {
				if ((xmlStrEqual(cur->name, (const xmlChar *)"title"))) {
						key = xmlNodeListGetString(doc, cur->xmlChildrenNode, 1);
						strcpy(titleOutput,(char*) key);
						xmlFree(key);
						info[0] = titleOutput;
						i++;
				}
				cur = cur->next;
		}
		while(!xmlStrEqual(cur->name,(const xmlChar*)"revision")) {
				cur = cur->next;
		}

		cur = cur->xmlChildrenNode; /*enters revision node*/
		while (i < 4 && cur!=NULL) {
				if(xmlStrEqual(cur->name,(const xmlChar*)"timestamp")){
						key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
						strcpy(timestampOutput,(char*) key);
						xmlFree(key);
						info[3] = timestampOutput;
						i++;
				}
				if(xmlStrEqual(cur->name,(const xmlChar*)"text")){
						key = xmlNodeListGetString(doc,cur->xmlChildrenNode,1);
						if(key){
								textCounter((char*)key,textInfo);
								i++;
						}
				}

				if((xmlStrEqual(cur->name,(const xmlChar*)"contributor"))){ /*enters contributor node*/
						info[4] = xmlgetUsername(doc,cur);
						info[5] = xmlgetUserId(doc,cur);
						i++;
				}

				cur = cur->next;
		}
		return;
}

/*! Insert the relevant xml information into the data structure
 * @param table			the data structure (hashtable in this case) to insert into
 * @param docname		the document to retrieve information from

 * @return				the new and filled data structure
 */
TAD_istruct parseDocument(TAD_istruct table, char* docname){
		xmlDocPtr doc;
		xmlNodePtr cur;
		doc = xmlParseFile(docname);

		if(doc == NULL){
				fprintf(stderr,"Document not parsed successfully\n");
				return NULL;
		}

		cur = xmlDocGetRootElement(doc);

		if(cur == NULL){
				fprintf(stderr,"Empty document\n");
				xmlFreeDoc(doc);
				return NULL;
		}

		if(!xmlStrEqual(cur->name,(const xmlChar*) "mediawiki")){
				fprintf(stderr,"document of the wrong type, root node != mediawiki");
				xmlFreeDoc(doc);
				return NULL;
		}

		cur = cur->xmlChildrenNode;

		while(cur != NULL){
				if((xmlStrEqual(cur->name,(const xmlChar*)"page"))){
						/* Output arrays to fill */
						char* articleOutput [] = { "empty","empty","empty","empty","empty","empty" };
						long textOutput[2] = {0L,0L};
						xmlNodePtr temp = cur;
						long newRevisionFlag = 0;

						parseIds (doc, cur, articleOutput);
						char* id = articleOutput[1];
						char* revisionid = articleOutput[2];
						newRevisionFlag = revisionBool (table, id,revisionid);

						if (newRevisionFlag == 1) {
							parseInformation (doc,temp,articleOutput,textOutput);

							char* title = articleOutput[0];
							char* timeStamp = articleOutput[3];
							char* username = articleOutput[4];
							char* userID = articleOutput[5];
							long textSize = textOutput[0];
							long textWordCount = textOutput[1];

							insert(table,id,revisionid,title,timeStamp); /* 1 if new, 0 if existant in hashtable already*/
							incTotalRevisions(table,newRevisionFlag); /* add to totalRevisions */


							if(strcmp(username,"ERROR no username found")) /* because in some cases there is no username, only ip*/
								insertUser(table,userID,username); /* include that user entry revision */

							/* new entry to add into filtering arrays of number of words and number of bytes*/
							long index = atol(id);
							updateTextSizeWordCountArray(table,index,textSize,textWordCount);
						}

						incTotalArticles(table,1); /* one more article seen */
				}
				cur = cur->next;
		}

		incSnapsAdded(table,1);
		xmlFreeDoc(doc);
		return table;
}
