// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <strings.h>
#include <stdlib.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];
int hashvalue = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    //NUMBER OF WORDS IN DICTIONARY

    // TODO
    hashvalue = hash(word);

    node *cursor = table[hashvalue];

    while (cursor != 0)
    {
        if (strcasecmp(word, cursor -> word) == 0)
        {
            return true;
        }
        cursor = cursor -> next;

    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    long total = 0;

    for (int j = 0; j < strlen(word) ; j++)
    {
        total += tolower(word[j]);

    }
    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
int count = 0;
int hash_value = 0;
bool load(const char *dictionary)
{
    // TODO
    FILE *opendic = fopen(dictionary, "r");
    if (opendic == NULL)
    {
        printf("Unable to open %s\n", dictionary);
        return false;
    }
    //declare variable to store word
    char word[LENGTH + 1];

    //LOOP TO SCAN THE WORDS IN DICTIONARY
    while (fscanf(opendic, "%s", word) != EOF)
    {
        //allocating memory for each node
        node *a = malloc(sizeof(node));
        //if allocation not successful
        if (a == NULL)
        {
            return false;
        }
        //memory successfully allocated
        //copy word(string) from dictionary into the word in node(a)
        strcpy(a -> word, word);
        //obtain hash value
        hash_value = hash(word);
        a->next = table[hash_value];
        table[hash_value] = a;
        count++;
    }
    fclose(opendic);
    return true;
}


// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    if (count > 0)
    {
        return count;
    }
    return 0;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        //set cursor to start of the linked list
        node *cursor = table[i];
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor -> next;
            free(temp);
        }
    }
    return true;

}
