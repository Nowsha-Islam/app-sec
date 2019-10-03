#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "dictionary.h"

char *remPunct(char *str) {
    if (str == NULL){
        return NULL;
    }
    char *x = str;
    char *y = str + strlen(str) - 1;
    while (ispunct(*x)) {
    	x++;
    }
    while (ispunct(*y) && x < y) { 
    	*y = 0; 
    	y--; 
    }
    if (strlen(x) == 0){
        return NULL;
    }
    return x;
}

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
 	char *x = 0;                 
    char *y;
    int num_misspelled = 0;
    char string[85];

     if (fp == NULL){
        exit(5);
    }

    while (fgets(string, 85, fp) != NULL) {
		// get rid of newline and set it null
        if ((x = strchr(string, '\n')) != NULL) {
            *x = '\0';
        }

        y = strtok(string," ");
        if (y != NULL && strlen(y) > LENGTH) {
            y = NULL;  
        }
        y = remPunct(y);

        while (y != NULL) {
            if (check_word(y, hashtable)) {
                y = strtok(NULL, " ");
                y = remPunct(y);
            }
            else if (check_word(toLower(y), hashtable)) {
                y = strtok(NULL, " ");
                y = remPunct(y);
            }
            else {
                if (num_misspelled < MAX_MISSPELLED){
                    misspelled[num_misspelled] = (char *)malloc((LENGTH+1)*sizeof(char));
                    strncpy(misspelled[num_misspelled],y,(LENGTH+1));
                    num_misspelled++;
                    y = strtok(NULL, " ");
                    y = remPunct(y);
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

