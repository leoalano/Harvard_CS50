// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

//counter of words in dictionary
int countersize = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    node *cursor = table[hash(word)];

    while (cursor != NULL)
    {
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO
    //source of hash function (part of it)
    //https://cs50.stackexchange.com/questions/38753/cs50-good-hash-function-for-pset5-speller/
    //38755#38755?newreg=7c502bfe160c4698a0ae4b64063ef5a0


    int hash = 401;

    for (int i = 0; i < strlen(word); i++) 
    {
        hash = ((hash << 4) + (int)(tolower(word[i]))) % N;
    }
    
    
    return hash;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    char wordBuffer[LENGTH + 1];

    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    while (fscanf(file, "%s", wordBuffer) != EOF)
    {
        node *nod = malloc(sizeof(node));
        if (nod == NULL)
        {
            unload();
            return false;
        }

        //copiar palavra dentro do node
        strcpy(nod->word, wordBuffer);
        nod->next = NULL;
        int hash_number = hash(nod->word);


        nod->next = table[hash_number];
        table[hash_number] = nod;


        //counter to count number of words in dictionary
        countersize++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (countersize != 0)
    {
        return countersize;
    }
    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *cursor;
    node *tmp;

    for (int i = 0; i < N; i++)
    {
        cursor = table[i];

        while (cursor != NULL)
        {
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
