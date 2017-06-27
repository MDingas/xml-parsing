/*  String functions library
 *	LI3
 *	UMINHO */

#include <string.h>

/*! Is the argument character one that separates words?
 * @param c			character to be evaluated
 * @return			boolean int
 */
int wordVerifier (char c){

		return (c == ' ' || c == '\t' || c == '\n');
}

/*! Do two words share the same prefix?
 * @return		Boolean answer
 */
int prefixMatch(char* prefix,char* word){
		int i = 0;
		if(!word)
				return 0;
		while(prefix[i] != '\0' && word[i] != '\0'){
				if(prefix[i] != word[i])
						return 0;
				i++;
		}
		if(prefix[i] != '\0') // otherwise prefixMatch("Diana","D") is a-ok and we don't want that
				return 0;
		return 1;
}

/*! Fill the output array with : the text number of bytes and the text number of words
 * @param text           article's text
 * @param output         array to fill output with
 */
void textCounter(char* text, long* output){
		long i = 0;
		long spaces = 0;

		if (wordVerifier(text[i]))
				while(wordVerifier(text[i]))
						i++;

		for(; text[i] != '\0' ;i++){
				if(wordVerifier(text[i])){
						spaces++;
						while(wordVerifier(text[i]))
								i++;
						i--;
				}
		}
		if(wordVerifier(text[i-1]))
				output[1] = spaces;
		else
				output[1] = spaces+1;

		output[0] = i - 1;
}
