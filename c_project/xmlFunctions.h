/* Xml document parser library */

#include <libxml/parser.h> /* xmlNodePtr and xmlDocPtr definition */
#include "snapshotStats.h"

/* Getters */
char* xmlgetUsername(xmlDocPtr doc,xmlNodePtr cur);
char* xmlgetUserId(xmlDocPtr doc,xmlNodePtr cur);

/* General Functions */
void parseIds (xmlDocPtr doc,xmlNodePtr cur,char** info); 
void parseInformation(xmlDocPtr doc,xmlNodePtr cur,char** info,long* textInfo);
TAD_istruct parseDocument(TAD_istruct hashtable,char* docname);
