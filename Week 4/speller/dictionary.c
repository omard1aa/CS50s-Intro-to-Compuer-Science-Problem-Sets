// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents number of buckets in a hash table
#define N 26
#define HASHTABLE_SIZE 65536
// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Represents a hash table
node *hashtable[HASHTABLE_SIZE];
int dict_size = 0;
// Hashes word to a number between 0 and 25, inclusive, based on its first letter
unsigned int hash(const char *word)
{
    return tolower(word[0]) - 'a';

    // unsigned int hash = 0;
    // for (int i = 0, n = strlen(word); i < n; i++)
    //     hash = (hash << 2) ^ word[i];
    // return hash % HASHTABLE_SIZE;
}
// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        hashtable[i] = NULL;
    }

    // Open dictionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        //unload();
        return false;
    }

    // Buffer for a word
    char myword[LENGTH + 1];

    // Insert words into hash table
    while (fscanf(file, "%s", myword) != EOF)
    {
        // TODO
        node* temp = malloc(sizeof(node));
        int ascii = hash(myword);
        temp->next = NULL;
        strncpy(temp->word, myword, sizeof(myword));
        if(hashtable[ascii] == NULL)
            hashtable[ascii] = temp;
        else{
            temp->next = hashtable[ascii];
            hashtable[ascii] = temp;
        }
        dict_size++;
    }

    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    // TODO
    int len = strlen(word);
    char lower_word[len + 1];
    for(int i = 0; i < len; i++){
        lower_word[i] = tolower(word[i]);
    }
    lower_word[len] = '\0';
    int ascii = hash(lower_word);
    node *cursor = hashtable[ascii];
    while(cursor != NULL){
        if(strcasecmp(cursor->word, lower_word) == 0)
            return true;
        cursor = cursor->next;
    }
    return false;
}

void destroy(node *head)
{
    if (head->next != NULL)
        destroy(head->next);
    free(head);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        if (hashtable[i] != NULL)
            destroy(hashtable[i]);
    }
    return true;
}
