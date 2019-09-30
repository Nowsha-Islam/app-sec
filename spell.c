#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

char *toLower(char *str) {
    char *ptr1 = str;
    char *ptr2 = str;

    if(str != NULL) {
        if(strlen(str) != 0) {
            while(*ptr1) {
                *ptr2 = tolower(*ptr1);
                ++ptr2;
                ++ptr1;
            }
            *ptr2 = '\0';
            return str;
        }              
    }
    return "";
}

char *remPunct(char *str) {
    if (str == NULL){
        return NULL;
    }
    char *p = str;
    char *t = str + strlen(str) - 1;
    while (ispunct(*p)) {
    	p++;
    }
    while (ispunct(*t) && p < t) { 
    	*t = 0; t--; 
    }
    { int i;
        for (i = 0; i <= t - p + 1; i++) {
            str[i] = p[i];
        }
        p = str;
    } /* --- */
    if (strlen(p) == 0){
        return NULL;
    }
    return p;
}

bool check_word(const char* word, hashmap_t hashtable[]){
	int index = hash_function(word);      
    node * curr = hashtable[index];     

    while (curr != NULL){
        if (strcmp(word, curr->word) == 0) {
            return true;
        } else {
            curr=curr->next;
        }
    }
    return false;
};

int check_words(FILE* fp, hashmap_t hashtable[], char * misspelled[]){
 	char *pos = 0;                 
    char *pch;
    int num_misspelled = 0;
    char string[80];

    while (fgets(string, 80, fp) != NULL) {
		// get rid of newline and set it null
        if ((pos = strchr(string, '\n')) != NULL) {
            *pos = '\0';
        }

        pch = strtok(string," ");
        if (pch != NULL && strlen(pch) > LENGTH) {
            pch = NULL;
        }
        pch = remPunct(pch);

        while (pch != NULL) {
            if (check_word(pch, hashtable)) {
                pch = strtok(NULL, " ");
                pch = remPunct(pch);
            }
            else if (check_word(toLower(pch), hashtable)) {
                pch = strtok(NULL, " ");
                pch = remPunct(pch);
            }
            else {
                if (num_misspelled < MAX_MISSPELLED){
                    misspelled[num_misspelled] = (char *)malloc((LENGTH+1)*sizeof(char));
                    strncpy(misspelled[num_misspelled],pch,(LENGTH+1));
                    num_misspelled++;
                    pch = strtok(NULL, " ");
                    pch = remPunct(pch);
                } else {
                    return num_misspelled;
                }
            }
        }
    }
    return num_misspelled;
};

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]){
    FILE *fp = fopen(dictionary_file, "r");

    if(fp){
        for(int i=0; i<=HASH_SIZE;i++){
        	hashtable[i]=NULL;
        }

        int index;
        char buff[LENGTH];

        while(fgets(buff,LENGTH,fp)){
            // get rid of newline and set it null
            if(buff[strlen(buff)-1] =='\n'){
            	buff[strlen(buff)-1]='\0';
            }
        
            for(int i=0;i<=strlen(buff);i++){
            	buff[i]= tolower(buff[i]);
            }
        
            index = hash_function(buff);
            if(hashtable[index] == NULL){
                    node * curr = malloc(sizeof(node));
                    strncpy(curr->word,buff,LENGTH);
                    curr->next = NULL;
                    hashtable[index] = curr;
            }
            else{
                    node * curr = malloc(sizeof(node));
                    strncpy(curr->word,buff,LENGTH);
                    curr->next =hashtable[index];
                    hashtable[index] = curr;
                }
        }
        fclose(fp);
        return 1;
    } else {
    	return 0;
    }
};
 
